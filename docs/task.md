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
- `[ ]` Establish reproducible benchmark scenario
- `[ ]` Measure legacy Config path
- `[ ]` Measure RuntimeConfiguration path
- `[ ]` Measure average Evaluate() CPU time
- `[ ]` Measure P50
- `[ ]` Measure P95
- `[ ]` Measure P99
- `[ ]` Measure maximum
- `[ ]` Measure GetSnapshot() overhead
- `[ ]` Compare results
- `[ ]` Verify allocation behavior
- `[ ]` Re-measure after optimization
- `[ ]` Commit: perf: cache FSR3.1 runtime configuration

## Etapa 5B - Event-Driven Runtime Updates
- `[ ]` Define configuration change categories
- `[ ]` Define hot parameter updates
- `[ ]` Define pipeline reconfiguration events
- `[ ]` Define resource recreation events
- `[ ]` Connect UI/config changes to runtime updates
- `[ ]` Remove unnecessary per-frame configuration resolution
- `[ ]` Build
- `[ ]` Benchmark
- `[ ]` Validate
- `[ ]` Commit: `refactor: make runtime configuration event-driven`
