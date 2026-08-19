# C++ Runtime Performance Audit

## Etapa 1 - Hot Path Mapping
- `[x]` Map `Present()` execution path
- `[x]` Map `Evaluate()` execution path
- `[x]` Map upscaler execution path
- `[x]` Map frame generation execution path
- `[x]` Identify per-frame functions
- `[x]` Identify initialization-only functions

## Etapa 2 - CPU Cost Audit
- `[x]` Search per-frame allocations
- `[x]` Search mutexes in hot paths
- `[x]` Search string construction
- `[x]` Search filesystem operations
- `[x]` Search repeated configuration lookups
- `[x]` Search repeated capability detection
- `[x]` Search unnecessary copies

## Etapa 3 - GPU Synchronization Audit
- `[x]` Search fences
- `[x]` Search waits
- `[x]` Search flushes
- `[x]` Search readbacks
- `[x]` Search Map/Unmap operations
- `[x]` Analyze resource barriers

## Etapa 4 - Resource Lifetime Audit
- `[x]` Identify per-frame allocations
- `[x]` Identify reusable resources
- `[x]` Identify redundant resource creation
- `[x]` Analyze resize paths
- `[x]` Analyze descriptor allocation

## Etapa 5A - Runtime Configuration Snapshot
### Data Model
- `[x]` Create `RuntimeConfigurationSnapshot`
- `[x]` Create feature-specific configuration models
- `[x]` Include only values consumed by the target hot path
- `[x]` Avoid duplicating the entire `Config` structure

### Manager
- `[x]` Create `RuntimeConfiguration`
- `[x]` Implement `GetSnapshot()`
- `[x]` Implement `RefreshFromConfig()`
- `[x]` Ensure refresh is not called from `Evaluate()`
- `[x]` Define initial update lifecycle

### FSR3.1
- `[x]` Audit all `Config` reads inside `FSR31Feature_Dx12::Evaluate()`
- `[x]` Classify each read (Hot parameter, Pipeline config, Resource config, Debug/UI)
- `[x]` Add only required values to the snapshot
- `[x]` Replace direct Config reads
- `[x]` Preserve existing behavior

### Validation
- `[x]` Build
- `[x]` Verify FSR3.1 behavior
- `[x]` Implement A/B benchmark path
- `[x]` Establish reproducible benchmark scenario
- `[x]` Measure legacy Config path
- `[x]` Measure RuntimeConfiguration path
- `[x]` Measure average Evaluate() CPU time
- `[x]` Measure P50
- `[x]` Measure P95
- `[x]` Measure P99
- `[x]` Measure maximum
- `[x]` Measure GetSnapshot() overhead
- `[x]` Compare results
- `[x]` Verify allocation behavior
- `[x]` Re-measure after optimization
- `[x]` Commit: perf: cache FSR3.1 runtime configuration

## Etapa 5B - Event-Driven Runtime Updates
- `[x]` Define configuration change categories
- `[x]` Define hot parameter updates
- `[x]` Define pipeline reconfiguration events
- `[x]` Define resource recreation events
- `[x]` Connect UI/config changes to runtime updates
- `[x]` Remove unnecessary per-frame configuration resolution
- `[ ]` Build
- `[ ]` Benchmark
- `[ ]` Validate
- `[ ]` Commit: `refactor: make runtime configuration event-driven`

## Etapa 6 - Upstream Integration & FrameBridge Separation

Referência operacional: [`UPSTREAM_INTEGRATION.md`](UPSTREAM_INTEGRATION.md).

### 6A - Baseline e Governança
- `[x]` Atualizar a referência local de `upstream/master`
- `[x]` Criar branch dedicada: `integration/optiscaler-20260818`
- `[x]` Registrar commits de origem, escopo e gates de integração
- `[x]` Confirmar que a sincronização exige importação de snapshot, não `merge`/`rebase`
- `[x]` Definir manifesto versionado para runtime e dependências distribuídas

### 6B - Importação Controlada do Runtime
- `[x]` Criar worktree/snapshot descartável do commit upstream alvo
- `[x]` Comparar o runtime atual ao snapshot upstream no nível de arquivo e comportamento
- `[x]` Classificar todos os deltas locais: upstreamed, manter localmente ou descartar
- `[x]` Portar a etapa 5A de `FSR31Feature_Dx12` para `FFXFeature_Dx12`, preservando o snapshot e o ciclo de refresh
- `[x]` Importar `OptiScaler/` e apenas as dependências de runtime exigidas
- `[x]` Reaplicar patches FrameBridge em commits pequenos e documentados
- `[x]` Atualizar `docs/PATCHES.md` com commit-base e justificativa de cada delta retido

### 6C - Separação de Produto e Distribuição
- `[x]` Preservar instalador, CI, perfis, diagnósticos, launcher e documentação como domínios FrameBridge
- `[ ]` Atualizar schema/configuração e o manifesto de empacotamento para o novo runtime
- `[ ]` Garantir que releases sejam montados de fontes fixadas e dependências verificadas
- `[x]` Atualizar os workflows Windows para `OptiScaler.sln` x64 e submódulos recursivos
- `[x]` Validar no CI a conexão do snapshot FFX e a ausência do backend DX12 FSR 3.1 removido
- `[x]` Verificar os assets FSR4 e DLSSG com os hashes do manifesto antes de montar o instalador
- `[x]` Centralizar os gates estáticos de integração em `scripts/Verify-Integration.ps1`
- `[x]` Não utilizar RARs de terceiros como insumo de release

### 6D - Validação de Integração
- `[ ]` Compilar Release x64
- `[ ]` Executar a suíte focada de regressão e benchmark da configuração de runtime
- `[ ]` Validar APIs e backends suportados no escopo da release
- `[ ]` Testar instalação e execução no FINAL FANTASY VII REBIRTH
- `[ ]` Registrar hashes dos artefatos e o commit upstream final
- `[ ]` Commit: `chore: integrate pinned OptiScaler upstream snapshot`

### 6E - Execução Contínua da Integração
- `[ ]` Conduzir iterativamente as pendências 6B–6D até a integração estar validada para merge
- `[ ]` Monitorar a execução do CI Windows quando a branch for publicada e corrigir cada falha encontrada
- `[ ]` Reexecutar a validação estática, build e benchmark após cada correção de integração
- `[ ]` Manter o RAR de referência apenas para auditoria; nunca incluí-lo no Git ou no pacote de release
- `[ ]` Encerrar a integração somente após evidência registrada de build, benchmark e teste no FINAL FANTASY VII REBIRTH
