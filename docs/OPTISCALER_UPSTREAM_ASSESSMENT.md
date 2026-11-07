# OptiScaler upstream assessment for FrameBridge

Review date: 2026-11-07

## Finding

The latest official stable upstream release found is [OptiScaler v0.9.4](https://github.com/optiscaler/OptiScaler/releases/tag/v0.9.4), published 2026-07-18 at tag commit `7534ad0`. Upstream's nightly repository has newer, frequently changing builds, including a 2026-11-07 nightly reported in the release feed. FrameBridge's checked-in runtime changelog still presents a `v0.7.8` line, so a meaningful upstream review is warranted.

The stable 0.9.4 release updates to FFX SDK 2.3 / FSR 4.1.1, adds official INT8 support for RDNA3 desktop GPUs, and changes FSR4 selection/fallback behavior. The release notes caution that unsupported hardware may fall back to FSR3 and recommend checking the runtime watermark. Upstream also reports setup improvements and other game-specific compatibility changes. Nightly notes include further ongoing compatibility work, which makes nightly adoption higher risk.

## What FrameBridge currently does

- Builds `OptiScaler/OptiScaler.vcxproj` through `OptiScaler.sln`.
- Packages the compiled runtime outputs via `.github/workflows/build.yml` and `installer/setup.iss`.
- Maps the runtime DLL to proxy names such as `dxgi.dll`, `version.dll`, `winmm.dll`, `d3d12.dll`, or `OptiScaler.asi`.
- Places selected support files into the game directory and `D3D12_OptiScaler`.
- Pins upstream runtime provenance and selected third-party assets in `manifests/runtime-dependencies.json`; integration assumptions are checked by `scripts/Verify-Integration.ps1`.

The manifest currently records FrameBridge import `62a45d44c2d74de07cd3bf90b6e23a74a27c2a5d` and target upstream commit `dcbabe62a939336299794e97c8dcab29f8ccd778`. These are the correct comparison baseline until verified against upstream history.

The GitHub compare endpoint for that target and release tag `7534ad0` reports **diverged**, with 325 commits ahead, 844 behind, and 254 changed files. Of these, 245 are under `OptiScaler/`; the rest include three external files, two images, and changes to `.github/workflows/build.yml`, the root `OptiScaler.ini`, `README.md`, and `setup_windows.bat`. The runtime diff includes `Config.cpp`/`Config.h`, FFX/FSR code, and XeSS proxy files. The comparison does not classify which changes are FrameBridge-specific. It does establish that an upstream update needs a deliberate source reconciliation and cannot be treated as a small version bump.

## Incorporation recommendation

Use stable `v0.9.4` as the first migration target. Do not base a production installer on a moving nightly. Before implementation, compare the exact pinned source commit to upstream `7534ad0` and catalogue:

1. FrameBridge-only patches and source files.
2. Build/project-file and submodule changes.
3. `OptiScaler.ini`, configuration docs, and default behavior changes.
4. New or renamed runtime DLLs, plugin directories, and support assets.
5. FFX/XeSS SDK versions, license files, source/binary provenance, hashes, and signatures.
6. Installer file mappings, compatibility tasks, uninstall behavior, and payload completeness.
7. Integration guardrails and CI/tests that encode assumptions about the older runtime.

Then port the runtime changes while retaining FrameBridge packaging logic, update the source commit and dependency manifest, adjust verification checks, and validate build plus installer/game behavior. Keep nightly evaluation in a separate experimental branch until specific changes and regressions are understood.

## Decision status

**Study: GO. Direct runtime replacement: NO-GO pending source diff and payload mapping.** The repository has not been built or game-tested as part of this assessment. No source or release inputs were updated.

## Sources

- [Official OptiScaler v0.9.4 release](https://github.com/optiscaler/OptiScaler/releases/tag/v0.9.4)
- [Official OptiScaler nightly releases](https://github.com/optiscaler/OptiScaler-nightly/releases)
- [GitHub comparison: pinned runtime target to v0.9.4](https://github.com/optiscaler/OptiScaler/compare/dcbabe62a939336299794e97c8dcab29f8ccd778...7534ad0)
- Local evidence: `manifests/runtime-dependencies.json`, `scripts/Verify-Integration.ps1`, `.github/workflows/build.yml`, `installer/setup.iss`, `Changelog.md`.
