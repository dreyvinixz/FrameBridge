# FrameBridge Architecture

O **FrameBridge** não é um mero *fork* do OptiScaler nem uma coleção de wrappers empilhados. O FrameBridge é uma **plataforma de gerenciamento, automação e inteligência para mods gráficos**. 

Sua arquitetura foi desenhada para separar claramente as responsabilidades entre a camada de injeção (Graphics Runtime) e as ferramentas que operam ao redor dela.

## O Princípio do Runtime Único

Para evitar a complexidade, conflitos e imprevisibilidade gerados pelo empilhamento de múltiplos interceptadores (wrappers sobre wrappers), o FrameBridge adota a seguinte regra de ouro:

> **Princípio:** O FrameBridge terá apenas um Runtime de Injeção Gráfica responsável pela comunicação direta com a engine do jogo e o driver de vídeo.

Atualmente, esse runtime é o **OptiScaler**. 

Todos os demais componentes do FrameBridge — incluindo o instalador automatizado, o futuro Launcher, o coletor de telemetria, gerenciador de perfis e funcionalidades de inteligência artificial — são construídos **ao redor** desse runtime, sem duplicar suas responsabilidades primárias.

### Papel do Runtime (OptiScaler)
Sendo o único orquestrador da camada gráfica, o OptiScaler é inteiramente responsável por:
* Realizar o hook na API gráfica (DXGI, D3D12, Vulkan).
* Emular a API proprietária (Spoofing do NVAPI).
* Carregar os backends de upscaling (FSR, XeSS, DLSS).
* Direcionar chamadas de Frame Generation (ex: repassar os inputs para o mod do Nukem9 `dlssg_to_fsr3`).
* Exibir e gerenciar a interface gráfica de configuração (Overlay / ImGui) dentro do jogo.

Dessa forma, eliminamos intermediários como o DLSS Enabler ou renomeações agressivas de DLLs (ex: `dlss-enabler-upscaler.dll`), mantendo a arquitetura limpa, previsível e fácil de ser mantida e depurada. Se no futuro surgir um runtime mais eficiente que o OptiScaler, a substituição será feita na base do ecossistema, mantendo intactas todas as ferramentas de gerenciamento do FrameBridge.
