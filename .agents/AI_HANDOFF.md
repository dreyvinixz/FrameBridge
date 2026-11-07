# AI handoff — FrameBridge

Last reviewed: 2026-11-07

## Current state

- Active work is isolated on `codex/freetype-vram-audit-2026-11-07`, created from `codex/update-audit-2026-11-07`; `main` and the `v1.0.68` baseline were left unchanged. The branch has pushed commits through `06a3774e`; the FreeType import and compile fixes from the current continuation are not yet committed.
- The runtime source tree is based on OptiScaler development commit `d94fb15f03800fe48d36cc31e9e042c66f508a3f` (446 commits after the old target). This is not the stable v0.9.4 release and remains a candidate pending build and runtime validation.
- Updated pins: FidelityFX SDK v2.4.0, magic_enum v0.9.8, spdlog v1.17.0, unordered_dense v5.3.3, plus Vulkan-Headers aligned to upstream. XeSS v3.0.2 and Nukem9 v0.130 remain unchanged.
- Official AMD FSR DLL 4.1.2.4555 is installed at `installer/assets/fsr4/amd_fidelityfx_upscaler_dx12.dll`; manifest hash/signature checks pass.
- Streamline headers are v2.14.16 and NVIDIA DLSS SDK headers are v310.9.16.
- Inno Setup CI install is pinned to 6.7.4.
- FreeType headers and `external/freetype/freetype.lib` are now 2.15.0 (`VER-2-15-0`, `37515cf56ba3349656049edbed89ffc6f6466f2d`), imported from successful CI artifact run `42416241659`; matching license texts were added and wired into installer packaging. SHA-256: `EDFC1F34383CE9B1729014825A71B3E2BCA4F754D64D51940B4C016D05FA4A5F`. FreeType's own build and artifact checks passed, but the complete OptiScaler build did not.
- Optional `Performance/LowVRAMMode` was added. It disables OptiScaler's D3D12 HUD capture/tracking path, caps the supported frame-generation routes at 2X, and turns off forced Dynamic MFG. It is off by default and cannot change game-owned texture pools, resolution, or engine budgets.
- The repository-local `github-actions-efficiency` skill exists under `.agents/skills/`.
- Native DLSSG config/state wiring was restored for the imported runtime. Optional RTX 40 Ada 6X unlock and an RTX 20/30 SM75/SM86 external-runtime loader are integrated under `OptiScaler/framegen/dlssg/`; both default off. The external runtime is user supplied and not packaged. GTX 16/10 are not enabled by this path. Read `docs/OPTISCALER_UPSTREAM_ASSESSMENT.md` and the adjacent `THIRD_PARTY_NOTICES.md` before changing or distributing it.

## Validation and outstanding work

- `scripts/Verify-Integration.ps1` passes locally.
- Latest CI run `42419227213`: formatting passed; FreeType 2.15.0 was rebuilt and uploaded; `Verify-Integration.ps1` passed against the tracked headers/library and license payload; Release x64 MSBuild still failed. The missing `fakenvapi_inc.h`, MFG version API calls, duplicate project entries, and `FSR4Support` include conflict are resolved. Remaining compile failures include the OptiScaler snapshot's incomplete `NVSDK_NGX_Handle` use and missing Vulkan NGX init declaration, NVAPI duplicate definitions across headers, and absent Vulkan CUDA launch declarations. Resolve these compatibility groups before rebuilding. No installer or game/API smoke test has run.
- Before merge/release: get a clean Release x64 build with the tracked FreeType pair; confirm output paths and license payload against `installer/setup.iss` and `.github/workflows/build.yml`; build installer; exercise supported game/API scenarios; record final artifact hashes.
- Preserve this as a candidate branch until all gates are evidenced. Do not represent static integration verification as build/runtime validation.
- The Ada and SM75/SM86 MFG integrations have not yet been compiled or exercised in a game. Treat them as experimental; obtain RTX 40 and RTX 30 smoke-test evidence before considering any default change or release claim. Upstream has not reported an in-game Turing test.
- The low-VRAM audit identifies on-demand full-resolution HUD capture copies as a controllable allocation, with a potential 4-slot RGBA16F footprint near 253 MiB at 4K before conversion resources. Game-specific VRAM benchmarking is still needed; MFG may add VRAM use and does not guarantee AAA playability on 4 GB cards. Read `docs/LOW_VRAM_PERFORMANCE_AUDIT.md`.

## Read order

1. `.agents/AGENTS.md`
2. This handoff
3. `docs/OPTISCALER_UPSTREAM_ASSESSMENT.md`
4. `manifests/runtime-dependencies.json`
5. `scripts/Verify-Integration.ps1`
