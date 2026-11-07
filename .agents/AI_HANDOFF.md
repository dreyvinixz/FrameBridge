# AI handoff — FrameBridge

Last reviewed: 2026-11-07

## Current state

- **Current integration branch:** `codex/combined-performance-audit-2026-11-07`, combining `codex/freetype-vram-audit-2026-11-07` and `codex/low-latency-audit-2026-11-07`. It is under validation; do not open the PR until all agreed checks pass. `main` and release `v1.0.68` remain untouched.
- The FreeType/VRAM work is documented in `docs/LOW_VRAM_PERFORMANCE_AUDIT.md`; low-latency/MFG findings, experimental code and benchmark gates are in `docs/LOW_LATENCY_MFG_AUDIT.md`. No measured input-latency or 4 GB game compatibility improvement is claimed.
- The runtime source tree is based on OptiScaler development commit `d94fb15f03800fe48d36cc31e9e042c66f508a3f` (446 commits after the old target). This is not the stable v0.9.4 release and remains a candidate pending build and runtime validation.
- Updated pins: FidelityFX SDK v2.4.0, magic_enum v0.9.8, spdlog v1.17.0, unordered_dense v5.3.3, plus Vulkan-Headers aligned to upstream. XeSS v3.0.2 and Nukem9 v0.130 remain unchanged.
- Official AMD FSR DLL 4.1.2.4555 is installed at `installer/assets/fsr4/amd_fidelityfx_upscaler_dx12.dll`; manifest hash/signature checks pass.
- Streamline headers are v2.14.16 and NVIDIA DLSS SDK headers are v310.9.16.
- Inno Setup CI install is pinned to 6.7.4.
- FreeType headers and `external/freetype/freetype.lib` are now 2.15.0 (`VER-2-15-0`, `37515cf56ba3349656049edbed89ffc6f6466f2d`), imported from successful CI artifact run `42416241659`; matching license texts were added and wired into installer packaging. SHA-256: `EDFC1F34383CE9B1729014825A71B3E2BCA4F754D64D51940B4C016D05FA4A5F`. FreeType's own build and artifact checks passed, but the complete OptiScaler build did not.
- Optional `Performance/LowVRAMMode` was added. It disables OptiScaler's D3D12 HUD capture/tracking path, caps the supported frame-generation routes at 2X, and turns off forced Dynamic MFG. It is off by default and cannot change game-owned texture pools, resolution, or engine budgets.
- The repository-local `github-actions-efficiency` skill exists under `.agents/skills/`.
- Native DLSSG config/state wiring was restored for the imported runtime. Optional RTX 40 Ada 6X unlock and an RTX 20/30 SM75/SM86 external-runtime loader are integrated under `OptiScaler/framegen/dlssg/`; both default off. The external runtime is user supplied and not packaged. GTX 16/10 are not enabled by this path. Read `docs/OPTISCALER_UPSTREAM_ASSESSMENT.md` and the adjacent `THIRD_PARTY_NOTICES.md` before changing or distributing it.
- `MfgGpuInfo.cpp` provides the MFG path's GPU query using DXGI and system NVAPI because the legacy `misc/IdentifyGpu.cpp` is incompatible with this runtime snapshot. It has compiled in CI but still needs hardware smoke testing, especially adapter selection on hybrid-GPU systems.

## Low-latency audit findings (2026-11-07)

- The synthetic GPU/driver/OS timing fields in `OptiScaler/low_latency/input/input_common.cpp` have now been removed from fallback reports (zero means unavailable); `Reflex_Hooks.cpp` guards missing/zero-duration timing data. The missing Reflex output backend in the translation layer is still an open research task; native Reflex pathways are separate.
- Further hypotheses: repeated DLSS-G/Reflex SetOptions in Dispatch; possible FPS-cap mismatch with high MFG multipliers; LatencyFlex sleep/reset and lock contention; FSR-FG frame pacing; DXGI waitable-object ownership in Streamline. All require measurement; do not prematurely remove pacing sleeps or force DXGI latency limits.
- Prioritize instrumentation, marker correctness, input-to-photon/FrameView measurements, then isolated A/B optimizations. See `docs/LOW_LATENCY_MFG_AUDIT.md` for evidence, line ranges, experiment matrix, and release gates.

- Experimental `[Performance] OptimizeStreamlineOptions=true` (default `auto`/false) caches successful DLSS-G static and Reflex options, invalidates on lifecycle events, retries errors, and logs counts on deactivation. Dynamic MFG DLSS-G options are still applied on every dispatch. The cache is only a candidate optimization, not a proven input-to-photon fix. Test with OFF/ON captures and restart between edits.
- Portable unit tests: `g++ -std=c++20 -Wall -Wextra -Werror tests/applied_options_cache_test.cpp -o cache_test && ./cache_test`; `python3 -m unittest discover -s tests -p 'compare_latency_csv_test.py'`. The CI workflow now runs them for `codex/**` and builds Windows independently of formatting; no release pipeline is triggered. Compare captured data with `scripts/compare_latency_csv.py`, specifying the measured CSV column. For FrameView `MsBetweenDisplayChange` describes display cadence, NOT input-to-photon.

## Validation and outstanding work

- `scripts/Verify-Integration.ps1` passes locally. Combined branch CI run `42424665358` for commit `51cf7d2321a650f405a383b0c2b6765ae26abc83` passed the portable C++ and Python checks, clang-format, FreeType rebuild/artifact, integration verification, and Release x64 MSBuild.
- The build fixes reconciled runtime config snapshots and NGX Vulkan declarations; completed the internal synthetic NGX handle used by this interposer; unified NVAPI include resolution; restored the compatible DllMain baseline while keeping Ampere MFG setup outside loader lock; aligned NGX function declarations and legacy menu preset values with the pinned SDK; and added a compatible GPU query specifically for MFG instead of compiling the obsolete `misc/IdentifyGpu.cpp`.
- The CI did not previously have an installer compile/extract gate. The validation gate is being added to the existing read-only CI to reuse the Release x64 build; do not dispatch `.github/workflows/build.yml`, which is the release workflow and has `contents: write`. Installer artifacts belong only in Actions artifacts.
- No installer compile/extract inspection, game/API smoke test, or input-to-photon/VRAM benchmark has run. Local hardware runtime validation and measured 4 GB compatibility remain open gates. Do not claim improvements without A/B evidence.
- Before merge/release: compile the installer with Inno Setup 6.7.4 in a non-publishing workflow; extract/install it in an isolated directory and verify the payload/licenses and artifact hashes; exercise supported game/API scenarios; then capture VRAM and latency A/B measurements on compatible hardware.
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
