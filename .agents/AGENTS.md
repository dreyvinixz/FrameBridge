# FrameBridge agent instructions

## Project scope

FrameBridge packages and installs a customized OptiScaler runtime. The repository vendors the runtime source under `OptiScaler/`, builds it with `OptiScaler.sln`, and creates the Windows installer from `installer/setup.iss` and `.github/workflows/`.

## Before changing code

1. Read `.agents/AI_HANDOFF.md` and the relevant project documentation.
2. Check `git status --short --branch`; preserve user changes and never reset or overwrite them.
3. For OptiScaler imports, review `manifests/runtime-dependencies.json`, `scripts/Verify-Integration.ps1`, and the upstream release/commit. Keep source commit, dependency commits, downloaded release assets, hashes, signatures, and installer mappings aligned.
4. Do not replace the vendored runtime with an upstream archive or binary as a shortcut. This project carries local integration changes and release packaging assumptions.

## Change and validation rules

- Keep changes focused and explain user-visible installer/runtime effects.
- Preserve license notices and update shipped licenses when changing bundled components.
- Treat nightly OptiScaler builds as experimental. Do not make them the default release input without compatibility evidence and explicit product direction.
- Run the relevant existing checks when asked to verify: `scripts/Verify-Integration.ps1`, MSBuild Release x64, and relevant GitHub Actions. Do not claim an installer/game validation unless it actually ran.
- Do not commit, push, publish, or change a release unless explicitly requested.
- Update `.agents/AI_HANDOFF.md` when architecture, integration state, or operational gates change.

## Architecture guide

- Runtime source: `OptiScaler/`
- Runtime provenance and third-party release inputs: `manifests/runtime-dependencies.json`
- Integration invariants: `scripts/Verify-Integration.ps1`
- Build solution: `OptiScaler.sln`
- Installer composition and install/uninstall behavior: `installer/setup.iss`
- CI/release automation: `.github/workflows/`
- Current repo onboarding: `.agents/ONBOARDING.md`
- Current upstream compatibility assessment: `docs/OPTISCALER_UPSTREAM_ASSESSMENT.md`
