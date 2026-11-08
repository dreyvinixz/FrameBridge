# OptiScaler and dependency update assessment

Review and implementation branch: `codex/combined-performance-audit-2026-11-07` (2026-11-07).

## Upstream decision

The latest stable OptiScaler release is `v0.9.4` (tag commit `7534ad0`). The project snapshot has been advanced to upstream development commit `d94fb15f03800fe48d36cc31e9e042c66f508a3f`, which is 446 commits ahead of the previous pinned target `dcbabe62a939336299794e97c8dcab29f8ccd778`. This is a development snapshot, not the stable release. It was selected for this isolated integration branch to study the newest runtime and must not be described as release-ready until the build, installer, and game checks pass.

The imported snapshot includes significant FSR 3.1, FSR4, Vulkan, XeSS, DLSS, configuration, and runtime changes. The updated `Verify-Integration.ps1` passes its source/manifest checks. This does not establish compilation or runtime compatibility.

## Dependency updates made

- FidelityFX SDK v2: v2.4.0, commit `7539ae8b6568ad29ed78a753404351daf3b5cca7`.
- AMD signed FSR upscaler: 4.1.2.4555, copied from the official SDK. SHA-256 `AAD4FCCCCB29084A655F9A4F9F613BD22C8C03FB4A0F8840148CC6543B4CCACE`; Authenticode signer is AMD and signature is valid.
- Streamline headers: v2.14.16, commit `625db07d4bca5abc67d781c42af7e7fb0761f1b6`.
- DLSS SDK headers: v310.9.16, commit `9981c6350dc58c0ce21f024fd65e31e851c31689`.
- magic_enum v0.9.8, spdlog v1.17.0, unordered_dense v5.3.3.
- Vulkan-Headers pin aligned with the imported OptiScaler snapshot.
- XeSS remains v3.0.2; Nukem9 remains v0.130.
- Inno Setup is pinned to 6.7.4 in CI and contributor instructions.

FreeType headers and the tracked `freetype.lib` are both 2.15.0, built together by the pinned MSVC GitHub Actions workflow from `VER-2-15-0` at `37515cf56ba3349656049edbed89ffc6f6466f2d`. The artifact passed its version and hash checks. The complete OptiScaler Release x64 build passed in CI run `42428708468`, verifying the dependency in the combined DLL build. The library SHA-256 is `EDFC1F34383CE9B1729014825A71B3E2BCA4F754D64D51940B4C016D05FA4A5F`.

## Low VRAM performance audit

`docs/LOW_VRAM_PERFORMANCE_AUDIT.md` records the D3D12 resource-allocation inventory and measurement gates. An optional `Performance/LowVRAMMode` setting reduces OptiScaler-owned HUD capture/tracking and limits supported frame generation to 2X. It cannot change engine-owned texture residency or make a global 4 GB AAA compatibility promise. The mode remains off by default pending build and game measurements.

## Validation and release gate

`./scripts/Verify-Integration.ps1` passes locally. CI run `42428708468` passed the Release x64 build, Inno Setup 6.7.4 compilation, and silent install/payload/license inspection. The validation installer is SHA-256 `4CF273CEC1A4C473211E6278D177A637965326032794EAE05D1F9E0EE8341545`; it is a CI validation artifact, not a public release. The extracted runtime reports OptiScaler `0.10.0.1` (`0.10.0-pre1`), FSR4 `4.1.2.4555`, and XeSS DLL `2.0.2.68`; FreeType `2.15.0` headers/library and license files were checked by the integration gate and installer inspection. No game/API smoke test or VRAM/latency benchmark has run. Keep `main` and `v1.0.68` unchanged until those runtime gates pass.

## Multi Frame Generation compatibility study

FrameBridge already has native Streamline DLSSG integration. Streamline reports the provider's supported maximum and, on supported configurations, allows up to five generated frames (6X). The Streamline wrapper may report a lower ceiling, so raising that wrapper value alone cannot unlock an unsupported `nvngx_dlssg.dll` build.

The `dev_8` OptiScaler-MFG fork was evaluated as a source reference, not as a runtime replacement. A comparison from the shared OptiScaler base showed a broad fork diff, so importing its tree would replace unrelated runtime behavior. The narrower RTX 40 Ada memory patch is integrated behind `[DLSSG] AdaMfgUnlock`, which defaults off. It recognizes only reviewed DLSSG 310.7 and 310.9 code signatures, changes the in-memory capability gates and relevant interpolation kernels, and leaves unknown module versions unchanged. It does not ship a patched NVIDIA binary or proxy DLL.

The fork's RTX 20/30 SM75/SM86 route has now been selectively ported as an optional external loader. It loads a user-provided `dlssg_for_sm86` package from `OptiScaler/dlssg_sm86/`, and FrameBridge passes Streamline/DLSS-G library loads through to that package when enabled. The package is not included in the installer because it carries NVIDIA runtime/kernel assets with separate terms. The loader supports the upstream-documented RTX 20/30 target only; although the fork's README also lists GTX 16, the external runtime's own package documentation does not, and we do not enable GTX 16 without hardware evidence. GTX 10 is not supported by this SM75/SM86 path. RTX 50 and other native Streamline providers continue to use their reported native capability.

Both MFG routes remain experimental pending Release x64 compilation and game/API smoke tests. The fork records an in-game RTX 30 test, but says Turing has only unit and real-binary checks, not an in-game test. The upstream OptiScaler maintainers have publicly cited reverse-engineering and potential DMCA risk as reasons for not officially supporting RTX 30 MFG; the same caution applies to undocumented driver/runtime patching here. Do not enable this by default or describe it as a supported NVIDIA feature.

## References

- [OptiScaler v0.9.4 stable release](https://github.com/optiscaler/OptiScaler/releases/tag/v0.9.4)
- [OptiScaler development snapshot](https://github.com/optiscaler/OptiScaler/commit/d94fb15f03800fe48d36cc31e9e042c66f508a3f)
- [FidelityFX SDK v2.4.0](https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/releases/tag/v2.4.0)
- [Streamline v2.14.16](https://github.com/NVIDIA-RTX/Streamline/releases/tag/v2.14.16)
- [NVIDIA DLSS SDK](https://github.com/NVIDIA/DLSS/tree/v310.9.16)
- [Inno Setup releases](https://jrsoftware.org/isdl.php)
- [OptiScaler-MFG dev_8 reference](https://github.com/evairx/OptiScaler-MFG/releases/tag/dev_8)
- [OptiScaler maintainer discussion on reverse-engineered MFG](https://github.com/optiscaler/OptiScaler/discussions/1146)
- [NVIDIA Streamline DLSS-G programming guide](https://github.com/NVIDIA-RTX/Streamline/blob/main/docs/ProgrammingGuideDLSS_G.md)
