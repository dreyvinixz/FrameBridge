# AI handoff — FrameBridge

Last reviewed: 2026-11-07

## Current state

- **Current audit branch:** `codex/low-latency-audit-2026-11-07`, created from `codex/freetype-vram-audit-2026-11-07` at `6e141f6`. Audit and benchmark plan are in `docs/LOW_LATENCY_MFG_AUDIT.md`. This branch currently adds documentation only; no runtime performance change or latency improvement is claimed. `main` and release `v1.0.68` remain untouched. This branch has documentation commits on GitHub; no merge or release was made.
- Previous integration work was on `codex/freetype-vram-audit-2026-11-07` (based on `codex/update-audit-2026-11-07`).
- The runtime source tree is based on OptiScaler development commit `d94fb15f03800fe48d36cc31e9e042c66f508a3f` (446 commits after the old target). This is not the stable v0.9.4 release and remains a candidate pending build and runtime validation.
- Updated pins: FidelityFX SDK v2.4.0, magic_enum v0.9.8, spdlog v1.17.0, unordered_dense v5.3.3, plus Vulkan-Headers aligned to upstream. XeSS v3.0.2 and Nukem9 v0.130 remain unchanged.
- Official AMD FSR DLL 4.1.2.4555 is installed at `installer/assets/fsr4/amd_fidelityfx_upscaler_dx12.dll`; manifest hash/signature checks pass.
- Streamline headers are v2.14.16 and NVIDIA DLSS SDK headers are v310.9.16.
- Inno Setup CI install is pinned to 6.7.4.
- FreeType is currently 2.13.3. The latest official stable target is 2.15.0 (`VER-2-15-0`, `37515cf56ba3349656049edbed89ffc6f6466f2d`). `scripts/Build-FreeType.ps1` and the manual Windows CI workflow now build a pinned static x64 library and upload the matching headers/library; the workflow has not yet run. See the dependency manifest and `docs/LOW_VRAM_PERFORMANCE_AUDIT.md`.
- Optional `Performance/LowVRAMMode` was added. It disables OptiScaler's D3D12 HUD capture/tracking path, caps the supported frame-generation routes at 2X, and turns off forced Dynamic MFG. It is off by default and cannot change game-owned texture pools, resolution, or engine budgets.
- The repository-local `github-actions-efficiency` skill exists under `.agents/skills/`.
- Native DLSSG config/state wiring was restored for the imported runtime. Optional RTX 40 Ada 6X unlock and an RTX 20/30 SM75/SM86 external-runtime loader are integrated under `OptiScaler/framegen/dlssg/`; both default off. The external runtime is user supplied and not packaged. GTX 16/10 are not enabled by this path. Read `docs/OPTISCALER_UPSTREAM_ASSESSMENT.md` and the adjacent `THIRD_PARTY_NOTICES.md` before changing or distributing it.

## Low-latency audit findings (2026-11-07)

- Static code review identified synthetic GPU/driver/OS latency fields in `OptiScaler/low_latency/input/input_common.cpp` and a missing Reflex backend in the low-latency translation layer (native Reflex pathways are separate). Do not report those synthetic fields as measured performance.
- Further hypotheses: repeated DLSS-G/Reflex SetOptions in Dispatch; possible FPS-cap mismatch with high MFG multipliers; LatencyFlex sleep/reset and lock contention; FSR-FG frame pacing; DXGI waitable-object ownership in Streamline. All require measurement; do not prematurely remove pacing sleeps or force DXGI latency limits.
- Prioritize instrumentation, marker correctness, input-to-photon/FrameView measurements, then isolated A/B optimizations. See `docs/LOW_LATENCY_MFG_AUDIT.md` for evidence, line ranges, experiment matrix, and release gates.

## Validation and outstanding work

- `scripts/Verify-Integration.ps1` passes locally.
- Release x64 and installer builds, plus game/API smoke tests, have not run. The local Visual Studio installer was stopped when the user requested GitHub Actions; use the branch's manual CI run to build FreeType and the DLL.
- Before merge/release: download the successful FreeType artifact and install the matching headers/library into the repository; run CI again to confirm the tracked dependency and Release x64 build; confirm output paths and license payload against `installer/setup.iss` and `.github/workflows/build.yml`; build installer; exercise supported game/API scenarios; record artifact hashes.
- Preserve this as a candidate branch until all gates are evidenced. Do not represent static integration verification as build/runtime validation.
- The Ada and SM75/SM86 MFG integrations have not yet been compiled or exercised in a game. Treat them as experimental; obtain RTX 40 and RTX 30 smoke-test evidence before considering any default change or release claim. Upstream has not reported an in-game Turing test.
- The low-VRAM audit identifies on-demand full-resolution HUD capture copies as a controllable allocation, with a potential 4-slot RGBA16F footprint near 253 MiB at 4K before conversion resources. Game-specific VRAM benchmarking is still needed; MFG may add VRAM use and does not guarantee AAA playability on 4 GB cards. Read `docs/LOW_VRAM_PERFORMANCE_AUDIT.md`.

## Read order

1. `.agents/AGENTS.md`
2. This handoff
3. `docs/LOW_LATENCY_MFG_AUDIT.md`
4. `docs/OPTISCALER_UPSTREAM_ASSESSMENT.md`
5. `manifests/runtime-dependencies.json`
6. `scripts/Verify-Integration.ps1`
