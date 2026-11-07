# FrameBridge onboarding

## What this repository does

FrameBridge is a Windows installer and packaging project built around a locally maintained OptiScaler runtime. It is not just a wrapper around an upstream installer: the runtime source lives in `OptiScaler/`, is built from `OptiScaler.sln`, then selected outputs and third-party files are assembled by the GitHub Actions workflows and `installer/setup.iss`.

## Start here

1. Read `.agents/AGENTS.md` for repository rules.
2. Read `.agents/AI_HANDOFF.md` for the current checked-in state and pending decisions.
3. Read `README.md`, `Features.md`, and `Config.md` for the current user-facing description and runtime configuration.
4. Read `docs/OPTISCALER_UPSTREAM_ASSESSMENT.md` before changing upstream runtime code or installer payloads.

## Important paths

| Path | Purpose |
| --- | --- |
| `OptiScaler/` | Vendored and locally integrated runtime source |
| `OptiScaler.sln` | Windows x64 build solution |
| `manifests/runtime-dependencies.json` | Pinned runtime provenance, submodules, and external assets |
| `scripts/Verify-Integration.ps1` | Integration and release-input guardrails |
| `installer/setup.iss` | Installer components, payloads, and install behavior |
| `.github/workflows/` | CI, builds, and release automation |
| `external/` | Third-party source and SDK snapshots; several are Git submodules |

## Safe first checks

```powershell
git status --short --branch
git log -1 --format="%h %cs %s"
git submodule status
.\scripts\Verify-Integration.ps1
```

The submodule status may begin with `-` when a submodule checkout is not initialized. Initialize recursively only when you intend to build and can access the upstream repositories.

## Upstream update workflow

1. Choose a specific upstream tag or commit; record whether it is stable or nightly.
2. Compare source changes against the pinned `target_upstream_commit` in `manifests/runtime-dependencies.json`.
3. Identify local patches, changed build files, config defaults, API/ABI changes, assets, and licenses before importing.
4. Import into a dedicated branch and preserve FrameBridge-specific integration and installer behavior.
5. Update the provenance manifest and integration checks to match the new source and payload.
6. Build, run the repository checks, inspect the generated installer contents, and smoke-test supported game/API paths before release.

Never treat a successful compile alone as proof of game compatibility or release readiness.
