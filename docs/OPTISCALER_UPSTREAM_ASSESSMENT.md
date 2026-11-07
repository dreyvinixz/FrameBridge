# OptiScaler and dependency update assessment

Review and implementation branch: `codex/update-audit-2026-11-07` (2026-11-07).

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

FreeType is still 2.13.3. The project links a tracked `freetype.lib`; only replacing its headers with 2.15.0 would produce an ABI mismatch. Upgrade the source and rebuild the static library together using a Windows MSVC toolchain, then validate the output before changing this pin.

## Validation and release gate

`./scripts/Verify-Integration.ps1` passes locally. The AMD DLL version, SHA-256, and Authenticode signature were checked. No MSBuild/Visual Studio compiler or Inno Setup compiler was available on PATH during this pass, so no Release x64 build, installer build, or game test is claimed. Keep `main` and `v1.0.68` unchanged; review this branch and run those checks before merge or release.

## Multi Frame Generation compatibility study

FrameBridge already has native Streamline DLSSG integration. Streamline reports the provider's supported maximum and, on supported configurations, allows up to five generated frames (6X). The Streamline wrapper may report a lower ceiling, so raising that wrapper value alone cannot unlock an unsupported `nvngx_dlssg.dll` build.

The `dev_8` OptiScaler-MFG fork was evaluated as a source reference, not as a runtime replacement. It is an archived development fork with a broad diff from our vendored OptiScaler revision, so importing its tree would replace unrelated runtime behavior. The narrower RTX 40 Ada memory patch is integrated behind `[DLSSG] AdaMfgUnlock`, which defaults off. It recognizes only reviewed DLSSG 310.7 and 310.9 code signatures, changes the in-memory capability gates and relevant interpolation kernels, and leaves unknown module versions unchanged. It does not ship a patched NVIDIA binary or proxy DLL. See `OptiScaler/framegen/dlssg/THIRD_PARTY_NOTICES.md` for provenance and notices.

RTX 20/30 support from the fork was excluded: it depends on a `version.dll` sidecar, which conflicts with FrameBridge's proxy selection and has a separate install/runtime footprint. RTX 50 and other native Streamline providers continue to use their reported native capability.

This remains experimental pending Release x64 compilation and RTX 40 game/API smoke tests. The upstream OptiScaler maintainers have publicly cited reverse-engineering and potential DMCA risk as reasons for not officially supporting RTX 30 MFG; the same caution applies to undocumented driver/runtime patching here. Do not enable this by default or describe it as a supported NVIDIA feature.

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
