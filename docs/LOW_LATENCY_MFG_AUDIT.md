# FrameBridge — auditoria de baixa latência, responsividade e MFG

Data: 2026-11-07  
Branch: codex/low-latency-audit-2026-11-07  
Base inicial: 6e141f6fa0f16d070e2f9bcab6040b24a0f2a331 (codex/freetype-vram-audit-2026-11-07)  
Estado: auditoria estática; **nenhuma alteração no runtime, build Release ou benchmark executado**.

## 1. Objetivo e escopo

Priorizar **latência de entrada até a exibição da imagem (input-to-photon)**, consistência de frametimes e tempo de resposta percebido, mesmo quando isso implicar menor FPS gerado. O interesse principal é o caminho OptiScaler + Frame Generation / Multi Frame Generation (DLSS-G, MFG experimental, FSR-FG e XeFG), sem presumir que um aumento de FPS interpolado melhore a responsividade.

A geração de quadros intercala imagens sintetizadas entre os quadros efetivamente simulados pelo jogo. Um multiplicador de 4X NÃO quadruplica a frequência de leitura de entrada/simulação. A origem do atraso pode estar na simulação, fila de render, waits CPU/GPU, implementação do interpolador, frame pacing, composição, sincronização vertical ou monitor. Este projeto não controla o motor do jogo nem o algoritmo proprietário de interpolação.

Este relatório distingue **evidências diretas do código** de **hipóteses a medir**; NÃO afirma ganho de latência nem compatibilidade ampla com GPUs/jogos.

## 2. Mapa dos caminhos relevantes

1. Jogo lê entrada -> inicia simulação -> envia marcadores Reflex/XeLL/Anti-Lag -> produz quadro real.
2. Fila CPU/D3D12 e GPU produzem recursos: profundidade, vetores de movimento, cor/HUD.
3. OptiScaler rastreia/prepara recursos, seleciona FG e chama dispatch/callbacks.
4. DLSS-G, FSR-FG ou XeFG gera quadros adicionais e aplica seu frame pacer.
5. Swapchain, DXGI, driver, DWM/flip e monitor exibem quadros reais/interpolados.

Pontos de entrada: OptiScaler/low_latency/, OptiScaler/hooks/Reflex_Hooks.cpp, OptiScaler/hooks/FG_Hooks.cpp, OptiScaler/wrapped/wrapped_swapchain.cpp, OptiScaler/framegen/{dlssg,ffx,xefg,nvngx}/.

## 3. Achados e oportunidades priorizadas

### LAT-01 — Medição de latência parcialmente sintética (P0, evidência confirmada)

Arquivo: [input_common.cpp, add_marker_to_report](../OptiScaler/low_latency/input/input_common.cpp), aproximadamente linhas 250–299 e get_latency nas linhas 451–550.

O código preenche gpuActiveRenderTimeUs com **100** e define driver/GPU/OS render queue com timestamps fabricados adicionando 100 microssegundos. Essas entradas não constituem medição real do dispositivo, driver ou fila. A interface de get_latency pode repassar esse FrameReport para consumidores que esperam dados Reflex; há outro caminho que usa dados nativos XeLL. **Não usar esses campos para quantificar o efeito de otimizações.**

Proposta: classificar explicitamente métricas em reais, derivadas de marcadores ou indisponíveis; eliminar preenchimentos fictícios ou sinalizar ausência sem violar o contrato com os consumidores; validar ABI/tamanho dos relatórios e fallback seguro. Adicionar testes para marcadores ausentes, fora de ordem, wrap-around, frame IDs e disponibilidade do backend. Validar a origem dos valores antes de mostrar qualquer “latência X ms”.

### LAT-02 — Backend Reflex do tradutor não implementado (P0/P1, evidência confirmada com ressalva)

Arquivo: [input_common.cpp, init_tech e update_low_latency_tech](../OptiScaler/low_latency/input/input_common.cpp), aproximadamente linhas 33–77 e 177–198.

No seletor LowLatencyMode::Reflex, a inicialização do backend está comentada, e o modo automático de saída da ponte em NVIDIA usa LatencyFlex. Isso **não significa** que o Reflex nativo do jogo esteja desligado: ReflexHooks e a integração Streamline DLSS-G têm caminhos próprios. Há risco de o *tradutor* entre entradas e backends usar heurísticas diferentes das nativas.

Proposta: mapear, por combinação GPU/FG/jogo, quem de fato executa Sleep e recebe markers; detectar duplos sleeps e perda de markers; testar pass-through Reflex nativo versus LatencyFlex sem ligar dois pacers na mesma swapchain. Só considerar backend Reflex da ponte após confirmar suporte e ordenação corretos.

### LAT-03 — Configuração DLSS-G/Reflex reaplicada a cada dispatch (P1, evidência confirmada)

Arquivo: [DLSSG_Dx12.cpp, Dispatch](../OptiScaler/framegen/dlssg/DLSSG_Dx12.cpp), aproximadamente linhas 334–383.

A função monta DLSSGOptions, chama DLSSGSetOptions, depois monta ReflexOptions e chama ReflexSetOptions no caminho de dispatch. Os próprios comentários do SDK Streamline recomendam não repetir ReflexSetOptions quando as opções não mudam. **O custo específico ainda é desconhecido**; a chamada pode ter efeitos internos e requer avaliação de thread/ordenação.

Proposta: instrumentar tempo das chamadas e contagem por segundo; prototipar cache de “últimas opções efetivas” para atualizar apenas em mudança, reinitialização, alteração de viewport, swapchain ou fallback. Conforme Streamline 2.14.16, DLSSGSetOptions tem efeito no próximo Present e requer ordem previsível com a thread que apresenta. Testar 2X–6X, toggle On/Off, Dynamic MFG, resize, alt-tab e troca de provider antes de aceitar o cache.

### LAT-04 — Limitador FPS e política de pacing precisam de reconciliação (P1, hipótese fundamentada)

Arquivo: [Reflex_Hooks.cpp, update/setFPSLimit](../OptiScaler/hooks/Reflex_Hooks.cpp), aproximadamente linhas 338–447.

A lógica tem detecção de marcadores, fallback após 20 atualizações sem marker, estado reflexLimitsFps e uma divisão fixa currentFps /= 2 em cenários FSR-FG ou DLSS-G detectado com fakenvapi. Há comentário apontando input lag muito alto em combinação de Reflex desabilitado com OptiFG.

Proposta: confirmar se o limite solicitado representa **FPS simulados/host** ou **FPS exibidos**, e verificar a transformação para 2X, 3X, 4X, 5X e 6X sem assumir divisão por multiplicador universal. Registrar quais mecanismos limitam FPS (jogo, Reflex, XeLL, AL2, FrameBridge, driver, RTSS, FG pacer) para evitar limitadores concorrentes. Não forçar um frame cap global em jogos desconhecidos.

### LAT-05 — Espera e contenção no LatencyFlex (P1/P2, hipótese a medir)

Arquivo: [ll_latencyflex.cpp, lfx_sleep](../OptiScaler/low_latency/low_latency_tech/ll_latencyflex.cpp), linhas 6–85.

A rotina segura deinit_mutex durante o cálculo e a espera e executa eepy(200000000ULL) (~200 ms) no caminho de reset de modo. Ela também usa locks internos e sleeps para pacing — isso pode ser **correto e necessário**. Não remover sleeps indiscriminadamente: elimina pacing, pode elevar consumo e piorar p95/p99 de latência.

Proposta: medir tempo de bloqueio em cada mutex, frequência de resets, duração dos sleeps e motivo (FPS cap vs pacing vs recuperação). Examinar o isolamento de reset fora do caminho crítico e ordem lock/condição, com testes de transição e sem data races. Inspecionar entrada do pipeline antes de mover o local do sleep.

### LAT-06 — FSR-FG: opção assíncrona e frame-pacing tuning (P1, hipótese a medir)

Arquivo: [FSRFG_Dx12.cpp](../OptiScaler/framegen/ffx/FSRFG_Dx12.cpp), linhas ~249–287 e ~416–427. Arquivo: [OptiScaler.ini](../OptiScaler.ini), seção FSRFG.

Existem parâmetros para safetyMarginInMs, varianceFactor, hybrid spin, hybridSpinTime, espera por fence e allowAsyncWorkloads. Configuração inadequada pode melhorar FPS médio mas causar jitter, busy spin, atraso de display, stutter e gasto de CPU.

Proposta: comparar parâmetros **padrão do SDK**, async on/off e grupos pequenos de tuning, uma variável por rodada. Instrumentar dispatch/present/fence (GPU e CPU) e p99. Manter default inalterado e não oferecer valores “universais”.

### LAT-07 — Locks, Present e trabalho por quadro (P1/P2, hipótese a medir)

Arquivos: [FG_Hooks.cpp, FGPresent](../OptiScaler/hooks/FG_Hooks.cpp), ~1153–1250, e [wrapped_swapchain.cpp](../OptiScaler/wrapped/wrapped_swapchain.cpp), ~145–401.

Há Mutex no caminho FGPresent (dependendo de FGUseMutexForSwapchain), callbacks, preparação de HUD, QueryInterface e leitura de descrição da swapchain no wrapped Present. Verificar tempo gasto em cada componente, bloqueio em Mutex e chamadas redundantes sem comprometer a correção dos recursos. A função WaitForGPUIdle vista nesses arquivos é acionada por **resize/teardown/recriação**, não foi identificada como espera permanente de cada quadro; não tratá-la como gargalo contínuo sem novos dados.

Proposta: temporização leve por fase (entrada do Present, preparo, espera de lock, callback, chamada real, saída), com instrumentação desativável e sem logging textual por quadro em Release.

### LAT-08 — Swapchain waitable object e proprietário do pacer (P0 de correção, hipótese/risco de integração)

Arquivos: [FG_Hooks.cpp, hkGetFrameLatencyWaitableObject](../OptiScaler/hooks/FG_Hooks.cpp), ~475, [wrapped_swapchain.cpp](../OptiScaler/wrapped/wrapped_swapchain.cpp), e Streamline 2.14.16.

O SDK DLSS-G alerta: quando o proxy Streamline **possui** o frame-latency waitable object, fazer outro wait nele ou chamar SetMaximumFrameLatency do lado do jogo pode competir por sinais e prejudicar o pacing. Quando o jogo criou a swapchain com a flag de propriedade, a política é diferente. Não aplicar “SetMaximumFrameLatency(1)” globalmente: pode conflitar com o pacer e quebrar MFG. Inspecionar quem cria a swapchain, qual flag foi passada e quem espera, por provider.

Proposta: matriz de ownership de swapchain para DLSS-G / XeFG / FSR-FG / FG off; auditável por logs somente em criação; não mudar flags sem teste com VSync/VRR/HAGS e alt-tab.

### LAT-09 — XeFG/XeLL, markers e transições (P2, hipótese a medir)

Arquivos: [XeFG_Dx12.cpp](../OptiScaler/framegen/xefg/XeFG_Dx12.cpp), ~70–96, [ll_xell.cpp](../OptiScaler/low_latency/low_latency_tech/ll_xell.cpp), ~130–240.

XeFG habilita XeLL e chama SetLatencyReduction, coerente com a recomendação Intel. Há heurísticas de frame ID na ponte (comentário de risco sobre simulation_start_last_id + 1) e cache estático de opções. Avaliar desalinhamento entre frame ID de simulação, frame gerado e present e efetividade de marcadores em mudanças de janela/FG. Não desabilitar XeLL para “ganhar FPS”.

### LAT-10 — MFG experimental RTX 20/30 e RTX 40 (P0 validação, não candidato a otimização cega)

Arquivos: [AmpereMfgLoader.cpp](../OptiScaler/framegen/dlssg/AmpereMfgLoader.cpp) e [MfgUnlock.cpp](../OptiScaler/framegen/dlssg/MfgUnlock.cpp).

As rotas experimentais dependem de versão de runtime, patch em memória ou pacote externo fornecido pelo usuário. Exigem validação independente de pacing e responsividade; nenhum ganho de latência está demonstrado. Não ampliar assinatura de patch, contornar verificações ou distribuir DLL de terceiro nesta auditoria.

## 4. Metodologia de benchmark

### Métricas principais

- **Input-to-photon (ms)**: p50, p95 e p99 por captura externa (por exemplo, LDAT), quando disponível. Métrica mais próxima da reclamação do usuário.
- **Latência da pipeline**: timestamps de entrada, simulação, render submit, início/fim GPU, Present e exibição, **somente quando realmente medidos** e com proveniência.
- **Cadência de exibição**: distribuição de intervalos entre mudanças de imagem (não apenas duração entre chamadas de Present), stutter e percentis; NVIDIA FrameView com MsBetweenDisplayChange é adequado para DLSS-G.
- **Produção real vs quadros gerados**: FPS host/simulação, multiplicador configurado/efetivo, FPS exibidos e 1% lows.
- **Overhead do FrameBridge**: duração/p99 de mutex, dispatch, callbacks, pacer, SetOptions, waits, bloqueios CPU/GPU e contagem de resets.

### Matriz experimental mínima

1. Mesma cena/jogo/configuração e GPU: sem OptiScaler; OptiScaler sem FG; FG 2X; MFG 3X/4X/6X **apenas quando suportado**.
2. Para cada FG: opções padrão primeiro. Depois comparar uma mudança isolada (Reflex native/fallback; cap; async; tuning; cache).
3. Fixar resolução, qualidade, render scale, driver, HAGS, modo janela, monitor Hz, VSync, VRR, FPS cap e estado de overlays. Quando variar VSync/VRR, criar **rodada separada**.
4. Em cada cenário, 60 s de aquecimento + pelo menos 3 capturas de 120 s; registrar logs, resultados brutos, hash do binário e condições.
5. Plataformas: RTX 30 com runtime externo quando disponível, RTX 40 Ada quando disponível, RTX 50 nativa se disponível, AMD com FSR-FG, Intel com XeFG; sem presumir suporte em placas não testadas.
6. Casos de regressão: início do jogo, alt-tab, resize, pausa, toggle FG, mudança de multiplicador, cena limitada por CPU/GPU, transições de menu, fechamento do jogo e descarregamento de DLL.

Sem LDAT, produzir relatório **de frame pacing e tempos internos**, não afirmar medida absoluta input-to-photon. Exigir comparação A/B com mesma fonte de dados; não usar gpuActiveRenderTimeUs e campos sintéticos atuais do InputCommon como verdade de GPU.

### Critérios propostos para uma otimização

- Pelo menos 5% de redução replicável no p95 de latência input-to-photon **quando medível**, ou ganho sólido em métrica interna claramente identificada.
- Sem piora acima de 5% do p99 de cadência/frametime no mesmo cenário, sem stutter perceptível adicional e sem perda de sincronização de frames.
- Sem regressão de HUD, VSync/VRR, swapchain, crash, compatibilidade de jogos ou consumo de CPU.
- Fallback por provider e configuração opt-in; regressões tornam o experimento NO-GO.
- Percentuais acima são **critérios experimentais**, não promessas de benefício.

## 5. Plano de execução por mudanças pequenas

**L0 — Medição e observabilidade primeiro**: corrigir/proteger relatórios sintéticos, mapear IDs de frames, adicionar traces de timestamp com sobrecarga conhecida e exportação para CSV/ETW sem logs de texto por frame em Release.

**L1 — Sleep, markers e ownership**: mapear contexto real Reflex/XeLL/AL2/LatencyFlex por provider; identificar duplo pacing, frequência de resets e relatórios de marker faltando; não tocar em flags DXGI sem ownership confirmado.

**L2 — Custo de chamadas/configuração**: instrumentar DLSSGSetOptions/ReflexSetOptions por frame; ensaiar cache de estado efetivo garantindo ordem com Present e transições/teardown.

**L3 — Frame limiter host versus exibido**: medir a divisão por 2, a interação com 3X–6X, VSync e Dynamic MFG e comparar cap da engine x Reflex x provider.

**L4 — Backends FSR-FG e XeFG**: ajuste fino de async, safety margin e suporte XeLL somente após dados L0–L3.

**L5 — Proposta opcional de modo de baixa latência**: apenas após evidência A/B, considerar configuração Performance/LowLatencyMode=auto|on|off ou perfis equivalentes, sem defaults agressivos. Não confundir com Performance/LowVRAMMode, que tem objetivo diferente.

Não realizar merge ou release automática. O workflow de release do repositório publica artefatos; preferir workflow de CI/build sem release na branch. A atualização de FreeType/compilação x64 ainda é uma dependência operacional da base recebida.

## 6. Evidências externas e restrições

- NVIDIA Streamline DLSS-G Programming Guide 2.14.16: https://github.com/NVIDIA-RTX/Streamline/blob/main/docs/ProgrammingGuideDLSS_G.md  
  Reflex, opções, timing de Present, ownership de waitable object, FrameView e latência alta com multiplicadores elevados.
- NVIDIA Streamline Reflex Programming Guide 2.14.16: https://github.com/NVIDIA-RTX/Streamline/blob/main/docs/ProgrammingGuideReflex.md  
  Sleep e markers, frame limiter e evitar reconfiguração desnecessária por quadro.
- Microsoft DXGI: https://learn.microsoft.com/en-us/windows/uwp/gaming/reduce-latency-with-dxgi-1-3-swap-chains  
  Frame-latency waitable object em swapchain normal, com cautela especial quando há proxy do Streamline.
- Intel XeSS-FG Guide: https://www.intel.com/content/www/us/en/developer/articles/technical/xess-fg-developer-guide.html  
  XeLL como pré-requisito do XeFG para pacing e redução de latência.
- AMD FSR3 integration (histórico): https://gpuopen.com/presentations/2023/AMD_FidelityFX_Super_Resolution_3-Overview_and_Integration.pdf  
  Interpolação assíncrona pode sobrepor trabalho de GPU, mas deve ser mensurada na implementação atual.

## 7. Estado desta auditoria

- [x] Branch isolada criada a partir da base de desenvolvimento mais recente encontrada.
- [x] Mapeamento inicial de código, principais hipóteses e riscos de sincronização.
- [x] Plano de medição, critérios de aceitação e protótipos candidatos.
- [ ] Instrumentação implementada e testada.
- [ ] Build Release x64 e validação de CI.
- [ ] Benchmarks com hardware e jogos reais.
- [ ] Implementação de modo baixa latência.
- [ ] PR de implementação / merge / release.

**Conclusão:** a prioridade não deve ser “forçar 6X e diminuir sleeps”, mas observar a pipeline correta, preservar o provider de pacing e reduzir trabalho/filas desnecessários com evidência objetiva. O primeiro PR técnico recomendado é de observabilidade e correção da proveniência das métricas.
