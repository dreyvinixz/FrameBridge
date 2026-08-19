FrameBridge Runtime Patches

Patch 001
---------

Reason:
Maintain compatibility with legacy installers.

Upstream base:
`dcbabe62a939336299794e97c8dcab29f8ccd778`

File:
OptiScaler/dllmain.cpp

Changes:
- Accept dlss-enabler-upscaler.dll
- Accept nvngx-upscaler.dll
- Keep optiscaler.dll

Status:
Reapplied during the 2026-08-18 upstream integration. Revalidate after every upstream update.

Patch 002
---------

Reason:
Keep FrameBridge's configuration snapshot optimization after OptiScaler consolidated the FSR 3.1 DX12 path into the FFX DX12 backend.

Upstream base:
`dcbabe62a939336299794e97c8dcab29f8ccd778`

Files:
- `OptiScaler/runtime/RuntimeConfiguration.h`
- `OptiScaler/runtime/RuntimeConfiguration.cpp`
- `OptiScaler/upscalers/ffx/FFXFeature_Dx12.cpp`
- `OptiScaler/OptiScaler.vcxproj`
- `OptiScaler/OptiScaler.vcxproj.filters`

Changes:
- Capture the FFX hot-path configuration once after FFX initialization.
- Replace direct configuration reads in `EvaluateInternal()` with the snapshot.
- Mirror automatic volatile barrier/reactive-mask decisions into the snapshot.
- Publish snapshots when normal UI/config values change, while excluding volatile hot-path writes.

Status:
Static coverage, event-isolation, and project-registration checks pass. Release x64 build, benchmark, and game regression validation are pending.
