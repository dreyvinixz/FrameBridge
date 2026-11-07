# AI handoff — FrameBridge

Last reviewed: 2026-11-07

## Current state

- Working tree was clean on branch `main` (`origin/main`) at review time.
- Latest local commit: `05ba9320945b1d47c062f29b171b60776c0bed10`, dated 2026-08-18, subject `fix: align FSR4 installer eligibility`.
- This is about 11 weeks since the last recorded commit, rather than three full months.
- `external/` submodules are not initialized in this checkout (`git submodule status` prefixes each with `-`).
- `OptiScaler/` is a vendored runtime source tree and is built as part of this repository. `manifests/runtime-dependencies.json` is the provenance record; `scripts/Verify-Integration.ps1` enforces local integration and asset invariants.
- Latest official stable OptiScaler release checked on 2026-11-07: `v0.9.4` (published 2026-07-18; upstream tag commit `7534ad0`). Official release notes describe FFX SDK 2.3 / FSR 4.1.1, official RDNA3 desktop INT8 support, changes to installer/setup behavior, and FSR3 fallback detection. Nightly builds are active and include ongoing compatibility changes; they are not stable release candidates by default.
- The local runtime changelog still has a `v0.7.8` heading. The repository manifest pins a distinct upstream target commit (`dcbabe62a939336299794e97c8dcab29f8ccd778`) and a FrameBridge imported commit (`62a45d44c2d74de07cd3bf90b6e23a74a27c2a5d`). Do not infer exact ancestry or patch parity from the version heading; establish it with a source diff before importing.
- GitHub's compare endpoint reports the manifest target and upstream `v0.9.4` tag as diverged (325 commits ahead, 844 behind, 254 files changed). This confirms that the migration is a substantial source reconciliation, not a simple version bump.

## Main integration risk

The installer expects specific root-level proxy DLL names and separately places some runtime files under `D3D12_OptiScaler`. Upstream 0.9.x changed bundled components and FSR4 behavior; nightly work continues to change plugin/upscaler loading and compatibility. A direct binary swap could omit assets, invalidate provenance/hash checks, break installer mappings, or lose local runtime changes.

## Recommended next work

1. Inspect upstream `v0.9.4` source and its changes from the exact pinned commit; also inspect a dated nightly separately if a nightly feature is desired.
2. Produce a patch inventory and an import plan on a dedicated branch; preserve local FrameBridge runtime/config changes.
3. Reconcile the version/changelog and `manifests/runtime-dependencies.json` with the actual imported source only after that comparison.
4. Validate `scripts/Verify-Integration.ps1`, MSBuild Release x64, CI, installer payloads, and representative game/API smoke tests.

No OptiScaler source, dependency pin, binary payload, or installer behavior was changed during this review. No build or game compatibility check was run.

## Read order for the next session

1. `.agents/AGENTS.md`
2. This handoff
3. `docs/OPTISCALER_UPSTREAM_ASSESSMENT.md`
4. `manifests/runtime-dependencies.json`
5. `scripts/Verify-Integration.ps1`
