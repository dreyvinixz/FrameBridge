## FrameBridge Features

**FrameBridge** is built on the rock-solid foundation of OptiScaler but drastically improves the user experience by providing a unified, automated installer and a compiled-from-source pipeline.

### 🌟 FrameBridge Exclusive Features

- **Automated All-in-One Installer**: No more manually dragging and dropping `.dll` or `.reg` files. The Inno Setup-based installer handles everything, ensuring the correct files are placed in your game directory.
- **Compiled from Source**: FrameBridge doesn't just repackage binaries. The C++ core is compiled directly via our automated GitHub Actions CI/CD pipeline, ensuring maximum security and transparency.
- **Pre-Bundled Fakenvapi**: Spoofing NVAPI is fully automated. The installer natively bundles [FakeMichau's Fakenvapi](https://github.com/FakeMichau/fakenvapi), which is required for DLSS support in several games and unlocks Reflex hooking (Anti-Lag 2 / LatencyFlex) out of the box.
- **Nukem9's FSR FG Mod Integrated**: Full support for [dlssg-to-fsr3](https://github.com/Nukem9/dlssg-to-fsr3) is built into the installation package.

### ⚙️ Upscaling & Frame Generation Core Capabilities

Because FrameBridge is powered by the OptiScaler core, it inherits an incredible set of technical features:

* Supports multiple upscaling backends: **XeSS**, **FSR 2.1.2**, **FSR 2.2.1**, **FSR 3.1**, and **DLSS**
* Experimental support for frame generation (OptiFG by FSR)
* Supports DLSS 3.7 and above (signature checks bypassed automatically by the installer)
* Supports DLSS-D (Ray Reconstruction) on Nvidia cards with the ability to change presets on the fly
* Supports XeSS v1.3.x's Ultra Performance and NativeAA modes
* An [in-game menu](Config.md) for tuning and saving settings in real-time (Shortcut key: **INSERT**)
* **RCAS** support with **MAS** (Motion Adaptive Sharpening) for all Dx12 & Dx11 upscalers
* **Output Scaling** option (0.5x to 3.0x) for backends running on Dx12 & Dx11
* Built-in **DXGI spoofing** (when running as `dxgi.dll`) to spoof Nvidia GPUs, enabling XMX on Intel Arc cards
* Built-in **Vulkan spoofing** (enabled via `OptiScaler.ini`)
* Supports overriding scaling ratios and DRS range
* Autofixes for [colored lights](Config.md#resource-barriers-dx12-only) on Unreal Engine & AMD graphics cards 
* Autofixes for [missing exposure texture](Config.md#init-flags) information
* Ability to modify [Mipmap Lod Bias](Config.md#mipmap-lod-bias-override-dx12-only) game values

### 🛡️ Signature Bypass

To overcome DLSS 3.7's signature check requirements without manual registry edits, FrameBridge utilizes the proven methods developed by **Artur** (creator of [DLSS-Enabler](https://github.com/artur-graniszewski/DLSS-Enabler)). The installer automatically sets up the environment so games accept the modified DLLs seamlessly.
