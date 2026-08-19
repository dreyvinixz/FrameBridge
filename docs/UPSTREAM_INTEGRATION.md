# Upstream Integration Record

## 2026-08-18 baseline

| Field | Value |
| --- | --- |
| Status | Runtime snapshot imported; configuration snapshot ported; build and runtime validation pending |
| FrameBridge branch | `integration/optiscaler-20260818` |
| Recorded imported upstream commit | `62a45d44c2d74de07cd3bf90b6e23a74a27c2a5d` (`BAT improvements`, 2026-07-16) |
| Target upstream commit | `dcbabe62a939336299794e97c8dcab29f8ccd778` (`Remove some logging`, 2026-08-18) |
| Upstream repository | `https://github.com/optiscaler/OptiScaler.git` |
| Runtime/dependency manifest | [`manifests/runtime-dependencies.json`](../manifests/runtime-dependencies.json) |

## Integration model

FrameBridge and OptiScaler do not share Git history. The runtime must therefore be updated as a controlled vendor snapshot, not with `git merge` or `git rebase`.

`OptiScaler/` remains the vendored graphics runtime. FrameBridge owns the installer, profile and compatibility policy, release pipeline, diagnostics, launcher, documentation, and any future user-facing tooling. An upstream import must not silently replace FrameBridge-owned assets or workflow files.

## Protected local behavior

Before an import, compare the target runtime with the current tree and explicitly account for each item below:

1. Legacy DLL-name compatibility in `OptiScaler/dllmain.cpp`, documented in `docs/PATCHES.md`.
2. The FrameBridge runtime-configuration work recorded in `docs/task.md` (stage 5A), which must be ported from the removed FSR 3.1 DX12 path to the target FFX DX12 implementation.
3. FrameBridge installer, CI, test, documentation, and release changes outside the vendored runtime.

Any retained runtime delta must be documented in `docs/PATCHES.md`, include its upstream base commit, and have a validation entry. Generic improvements suitable for all OptiScaler users should be proposed upstream instead of becoming permanent local divergence.

## Import gates

- [ ] Create a disposable vendor-snapshot worktree from the target upstream commit.
- [ ] Produce a file-level delta against the current `OptiScaler/` tree.
- [ ] Classify every FrameBridge runtime delta as upstreamed, retained local, or obsolete.
- [ ] Import only the runtime and its required third-party dependencies.
- [ ] Reapply retained compatibility patches as small, reviewable commits.
- [ ] Update the configuration schema and package manifest together.
- [ ] Build Release x64 and execute the focused regression/benchmark suite.
- [ ] Test the FF7 Rebirth installation path before merging the integration branch.
- [ ] Record the final imported commit and release artefact hashes.

## 6B delta analysis

The comparison uses Git blob identity rather than branch ancestry. From the recorded import to the target upstream commit, 88 runtime files already match the target byte-for-byte and 11 match an earlier upstream revision. Those files will be replaced by the pinned target snapshot without carrying a FrameBridge delta.

Thirteen files are FrameBridge candidates. Their integration disposition is:

| Scope | Files | Disposition |
| --- | --- | --- |
| Legacy compatibility | `dllmain.cpp` | Port the documented legacy DLL-name support after the target snapshot is imported. |
| Runtime capability layer | `runtime/RuntimeCapabilities.*`, `runtime/RuntimeCapabilityTypes.h`, `proxies/FfxApi_Proxy.h`, `menu/menu_common.cpp`, `OptiScaler.vcxproj`, `OptiScaler.vcxproj.filters` | Preserve as a FrameBridge-owned runtime extension; port it against target interfaces and register it in the target project file. |
| Runtime configuration stage 5A | `runtime/RuntimeConfiguration.*`, `upscalers/fsr31/FSR31Feature_Dx12.cpp` | Preserve the behavior, but do not copy the old FSR 3.1 source file. The target has consolidated this path into `upscalers/ffx/FFXFeature_Dx12.cpp`; port the snapshot reads and refresh lifecycle to that implementation and re-benchmark it. |
| No retained FrameBridge behavior identified | `nvapi/fakenvapi/log.cpp`, `SysUtils.h` | Use the target upstream versions. |

The target also introduces eleven `framegen/nvngx/` files. They are new upstream runtime modules and must be included in the controlled snapshot import.

### Import result

`OptiScaler/` and its required external runtime headers were imported from the pinned target. The working tree matches that snapshot byte-for-byte except for the documented legacy DLL-name compatibility patch in `OptiScaler/dllmain.cpp`. The XeSS submodule is now pinned to `8fe81bdbbaf00b3c1b733fd0d830c333dc84e6f0` (v3.0.2).

Local compilation is pending because the current development environment has no MSBuild/Visual Studio toolchain installed. This is an environment limitation, not a successful build result; CI and a Windows development machine must run the build gate before this branch can merge.

## Retained FrameBridge extensions

### Runtime configuration snapshot

`runtime/RuntimeConfiguration.*` is now a FrameBridge-owned extension registered in the target project. `FFXFeature_Dx12::InitInternal()` creates the snapshot once, after `InitFFX()` has resolved its automatic values. `EvaluateInternal()` consumes a copy of that snapshot for all 25 configuration fields that the target upstream implementation formerly read directly.

The three automatic in-frame decisions that write volatile configuration values (color barrier, motion-vector barrier, and reactive-mask disablement) update both the upstream `Config` value and the local snapshot. This keeps the current frame and later frames consistent without re-reading configuration each frame.

Normal `CustomOptional` assignments and resets now publish a refreshed snapshot immediately, while `set_volatile_value()` intentionally does not. This lets UI/config changes reach the runtime without re-reading `Config` in `EvaluateInternal()` or turning automatic hot-path decisions into refresh events. `Config::LoadFromPath()` publishes once after the INI reload completes. Pipeline reconfiguration and resource-recreation event categories remain separate stage 5B work. Static validation confirms that the FFX evaluation path contains no direct configuration reads and that the snapshot covers each upstream hot-path field. A Release x64 build, benchmark, and game regression run remain mandatory.

### Runtime capability model

`runtime/RuntimeCapabilities.*` and `RuntimeCapabilityTypes.h` are retained as a FrameBridge-owned, mutex-protected diagnostics model and are explicitly registered in the target project. The prior FSR4 scanner hook and old menu integration were not copied: both depend on upstream interfaces that changed materially. Reconnecting live capability collection and UI presentation requires a separate, target-aware design and validation step; the retained model must not be presented as live telemetry yet.

### CI and release gates

All Windows build workflows now select `OptiScaler.sln` with `Configuration=Release`/`Platform=x64` (or their explicit debug configuration) and initialize submodules recursively. The integration validation workflow rejects a build that lacks the FFX snapshot connection or still contains the removed `FSR31Feature_Dx12.cpp` source.

The shared [`scripts/Verify-Integration.ps1`](../scripts/Verify-Integration.ps1) gate is executed locally and by the CI/release workflows. It checks the pinned manifest, FSR4 asset hash, FFX snapshot wiring, target project registration, absence of the removed FSR 3.1 DX12 backend, and the policy that no RAR is tracked. The release workflow additionally verifies the downloaded DLSSG archive/source assets against their recorded SHA-256 hashes before assembly. This does not validate a release by itself: the final CI artifact hashes, Release x64 build, benchmark, and game test must still be recorded before merge.

## Non-authoritative artefacts

Third-party RAR packages are useful for reconnaissance only. They are not an upstream source of record and must not be used as a release input. FrameBridge releases are assembled from the pinned upstream source and verified vendor dependencies.
