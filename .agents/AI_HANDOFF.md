# AI handoff — FrameBridge

Last reviewed: 2026-11-07

## Current state

- Active work is isolated on `codex/update-audit-2026-11-07`; `main` and the `v1.0.68` baseline were left unchanged. No commit, push, merge, or release was made.
- The runtime source tree is based on OptiScaler development commit `d94fb15f03800fe48d36cc31e9e042c66f508a3f` (446 commits after the old target). This is not the stable v0.9.4 release and remains a candidate pending build and runtime validation.
- Updated pins: FidelityFX SDK v2.4.0, magic_enum v0.9.8, spdlog v1.17.0, unordered_dense v5.3.3, plus Vulkan-Headers aligned to upstream. XeSS v3.0.2 and Nukem9 v0.130 remain unchanged.
- Official AMD FSR DLL 4.1.2.4555 is installed at `installer/assets/fsr4/amd_fidelityfx_upscaler_dx12.dll`; manifest hash/signature checks pass.
- Streamline headers are v2.14.16 and NVIDIA DLSS SDK headers are v310.9.16.
- Inno Setup CI install is pinned to 6.7.4.
- FreeType remains 2.13.3 because the tracked static library must be rebuilt alongside upgraded headers. Do not change headers alone.
- The repository-local `github-actions-efficiency` skill exists under `.agents/skills/`.

## Validation and outstanding work

- `scripts/Verify-Integration.ps1` passes locally.
- Release x64 and installer builds, plus game/API smoke tests, have not run. MSBuild, C++ compiler, and ISCC were not found on PATH in this environment.
- Before merge/release: run the native build with Visual Studio Build Tools; confirm new output paths and license payload against `installer/setup.iss` and `.github/workflows/build.yml`; build installer; exercise supported game/API scenarios; record artifact hashes. Upgrade FreeType source and library together when a suitable MSVC environment is available.
- Preserve this as a candidate branch until all gates are evidenced. Do not represent static integration verification as build/runtime validation.

## Read order

1. `.agents/AGENTS.md`
2. This handoff
3. `docs/OPTISCALER_UPSTREAM_ASSESSMENT.md`
4. `manifests/runtime-dependencies.json`
5. `scripts/Verify-Integration.ps1`
