# Low VRAM and performance audit

Audit branch: `codex/freetype-vram-audit-2026-11-07`  
Scope: OptiScaler-owned allocations and processing that FrameBridge can control. Game-specific texture pools, streaming, and engine budgets are outside this DLL's control.

## Findings

### D3D12 HUD capture is the clearest controllable memory cost

`Hudfix_Dx12` keeps four capture-resource slots (`BUFFER_COUNT`) and may create full-size copies of HUD-less input resources. The copies are allocated on demand when the HUD fix captures a resource; they are not a permanent allocation in every title. A four-slot RGBA16F copy set would be about 63 MiB at 1080p, 113 MiB at 1440p, or 253 MiB at 4K, before format-conversion resources and driver allocation overhead. RGBA8 would use about half those amounts. Actual use depends on the game's resource format, dimensions, and HUD-fix path.

The existing `OptiFG/DisableHUDFix` setting avoids installing the D3D12 resource-tracking hook. The new opt-in `Performance/LowVRAMMode` activates that setting to avoid capture copies and tracking work.

### Multi-frame generation trades additional output for memory and work

OptiScaler exposes frame-generation counts for DLSS-G, XeFG, and the external RTX 20/30 runtime. The external runtime has a separate `MaxGeneratedFrames` cap. Lowering these limits to 2X is the conservative choice for a constrained card; 5X/6X is not a VRAM optimization and may increase backend memory and GPU work. Exact deltas depend on the runtime and game and must be measured.

`Performance/LowVRAMMode=true` caps the supported OptiScaler frame-generation paths at 2X, disables Dynamic MFG forcing, and disables the HUD capture path. It is off by default. The user's configured values are preserved when OptiScaler saves the INI.

The D3D12 resource-creation scan also found per-frame-generation input copies in `IFGFeature_Dx12` for resources that require a copy or format/ordering adjustment, plus per-feature allocations in FSR, XeSS, shader, and HUD code. Those are conditional on selected features, active game inputs, and resolution. Removing them generically can break synchronization or frame-generation inputs. Upscaler and frame-generation SDKs can allocate additional internal resources that this source tree cannot count without runtime instrumentation.

### FreeType does not materially affect game VRAM

FreeType is statically linked for OptiScaler's ImGui font rendering. Updating it matters for dependency maintenance and UI font handling, but it does not change a game's texture residency or materially free game VRAM. The repository now contains the matching 2.15.0 headers and static x64 library, rebuilt by GitHub Actions run `42416241659` from tag `VER-2-15-0`, commit `37515cf56ba3349656049edbed89ffc6f6466f2d`. The library SHA-256 is `EDFC1F34383CE9B1729014825A71B3E2BCA4F754D64D51940B4C016D05FA4A5F`. FreeType build and artifact checks passed; the complete OptiScaler Release x64 build failed on unrelated source/API compile errors, so the combined runtime has not yet been validated.

## 4 GB usage guidance

The low-VRAM mode only reduces work and resources owned by OptiScaler. It does not add MFG support to GTX 16/10 cards; the external MFG route in this branch is limited to RTX 20/30. It also does not lower the game's texture quality, render resolution, ray-tracing settings, or engine streaming budget. For a 4 GB card, users still need to lower in-game texture quality first, disable ray tracing, use a lower output resolution with a supported upscaler, and avoid high-resolution texture packs. Start with 2X frame generation where the GPU/runtime supports it, then raise it only after checking the game's VRAM use and frametime stability.

Frame generation cannot make every AAA title fit in 4 GB. If the base game already exceeds available VRAM, Windows can move resources to shared memory, causing stutter even when an upscaler or MFG is active. No global 4 GB compatibility claim should be made without per-game measurements.

## Validation still required

- Repair the OptiScaler compile errors and rebuild the Release x64 DLL with the updated FreeType headers and matching static library.
- Build the installer after the FreeType update and dependency/license checks.
- Compare each supported game/API with LowVRAMMode off and on. Record dedicated and shared VRAM, GPU time, 1% lows, frametime spikes, and HUD artifacts.
- Test 2X, 3X/4X, and 6X MFG separately on supported hardware; do not infer VRAM savings from generated FPS.
- Include at least one 4 GB GPU and one higher-memory GPU in runtime checks before making a compatibility claim.
