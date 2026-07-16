# FrameBridge

[![Build & Release](https://github.com/dreyvinixz/FrameBridge/actions/workflows/build.yml/badge.svg)](https://github.com/dreyvinixz/FrameBridge/actions/workflows/build.yml)

**FrameBridge** is an open-source platform built around the OptiScaler Graphics Runtime. It simplifies the installation of modern graphics enhancements by automatically configuring upscaling and Frame Generation out-of-the-box across all GPUs. Built from source — compiled, packaged, and released automatically.

## 🌉 The Single Graphics Runtime Principle

FrameBridge is designed with a strict architectural philosophy: **A single graphics runtime handles all API hooking, spoofing, and backend routing.** 

By adopting **OptiScaler** natively as our exclusive injection runtime, we eliminate the need for complex proxy chains, intermediate wrappers, or aggressive DLL renaming. FrameBridge acts as the management and automation platform built *around* this runtime, ensuring a clean, stable, and highly compatible ecosystem.

![FrameBridge Banner](images/banner.png)

## 🚀 Key Features

- 🛠 **Native DXGI Hooking** — Bypasses system DLL conflicts by hooking graphics initialization natively.
- ⚙️ **Automated OptiScaler Setup** — No more manual INI editing; the installer pre-configures the best defaults.
- 📦 **Built from source** — Full C++ source code, compiled via CI/CD.
- 🤖 **Frame Generation** — Seamless native routing to Nukem9's DLSSG-to-FSR3 mod.
- 🌐 **Multi-GPU support** — NVIDIA, AMD, and Intel GPUs supported natively.
- 🔄 **Smart Uninstaller** — Manifest-based uninstallation prevents accidental deletion of user mods.

### Bundled Components

| Component | Version | Source |
|-----------|---------|--------|
| OptiScaler | 0.10.0-pre | Compiled from source (main branch) |
| DLSSG-to-FSR3 | 0.130 | Nukem9's mod |
| FFX SDK | 2.2 | FSR 4.1 + FSR-FG 4.0.0 |
| XeSS SDK | 3.0.1 | Intel |

*Note: Fakenvapi is no longer bundled as OptiScaler handles NVAPI spoofing natively.*

## 📥 Download

Download the latest automated installer from the [Releases](../../releases) page.

## 🏗️ Building from Source

### Prerequisites

- **Visual Studio 2022** with C++ Desktop workload
- **Windows SDK 10.0+**
- **Git** with submodule support
- **Inno Setup 6.2.0** (for installer generation)
- **7-Zip** (for packaging)

### Build Steps

```powershell
# Clone with submodules
git clone --recurse-submodules https://github.com/dreyvinixz/FrameBridge.git
cd FrameBridge

# Build OptiScaler DLL
msbuild OptiScaler.sln /p:Configuration=Release /p:Platform=x64

# Generate installer (Automatically handled by CI/CD)
```

### CI/CD Pipeline

Our automated GitHub Actions pipeline handles the heavy lifting:

1. **Compile** OptiScaler from C++ source
2. **Download** latest external dependencies (XeSS, FidelityFX, Nukem9)
3. **Package** all components with Inno Setup (generating the smart manifest)
4. **Release** as a GitHub release with the ready-to-use installer

## 🎮 Installation & Usage

1. Download the installer from [Releases](../../releases).
2. Run the setup and point it exactly to your game's executable (`.exe`) directory.
3. Choose the **DXGI (Recommended)** installation type.
4. Launch the game, select DLSS / DLSS-G in the settings.
5. Press **INSERT (INS)** to open the OptiScaler overlay and configure your backend.

## 🤝 Contributing

Contributions are welcome! Please read [CONTRIBUTING.md](CONTRIBUTING.md) before submitting PRs.

### Code Guidelines

- Every `.cpp` file must include `"pch.h"` as the **first** non-comment line.
- Never include `"pch.h"` inside header files.
- Follow the `.clang-format` style.

## 📄 License & Acknowledgments

This project is licensed under the **GNU General Public License v3.0** — see [LICENSE](LICENSE) for details.

- [OptiScaler](https://github.com/optiscaler/OptiScaler) — Core graphics runtime engine (GPLv3).
- [Nukem9's DLSSG-to-FSR3](https://github.com/Nukem9/dlssg-to-fsr3) — Frame generation module.
- [Intel XeSS](https://github.com/intel/xess) — XeSS SDK.
- [AMD FidelityFX](https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK) — FSR SDK.
