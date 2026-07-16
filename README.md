# FrameBridge

<p align="center">
  <img src="images/banner.png" alt="FrameBridge Banner">
</p>

<p align="center">
  <a href="https://github.com/dreyvinixz/FrameBridge/actions/workflows/build.yml">
    <img src="https://github.com/dreyvinixz/FrameBridge/actions/workflows/build.yml/badge.svg" alt="Build Status">
  </a>
</p>

**FrameBridge** is an open-source graphics compatibility platform built around the **OptiScaler Runtime**.

It provides a clean, unified, and hassle-free way to install modern upscaling and Frame Generation technologies. Say goodbye to manually copying DLLs, renaming files, or fighting with configuration files!

FrameBridge does **not** replace OptiScaler—it builds upon it by providing automation, packaging, configuration, and a consistent installation experience.

---

# Architecture

FrameBridge follows a simple design philosophy:

> **One Runtime. One Responsibility.**

Only **OptiScaler** is responsible for:

- Graphics API hooking
- NVAPI emulation
- Backend routing
- Runtime initialization
- In-game overlay

Everything else belongs to FrameBridge:

- Installer
- Release pipeline
- Automatic configuration
- Profiles
- Documentation
- Future Launcher
- Future Telemetry

```
               FrameBridge
──────────────────────────────────────
 Installer
 Release Pipeline
 Profiles
 Launcher (future)
 Telemetry (future)
──────────────────────────────────────
          OptiScaler Runtime
──────────────────────────────────────
 DXGI / D3D12 / NVAPI
──────────────────────────────────────
   DLSS • FSR • XeSS • Frame Generation
```

This architecture avoids wrapper chains, duplicate hooks and unnecessary DLL forwarding, resulting in a cleaner and more maintainable ecosystem.

---

# Features

- Native OptiScaler Runtime
- Automatic installer
- DXGI injection by default
- Automatic OptiScaler configuration
- Built entirely from source
- GitHub Actions CI/CD
- Native Nukem9 Frame Generation integration
- NVIDIA, AMD and Intel GPU support
- Open-source (GPLv3)

---

# Included Components

| Component | Version |
|------------|---------|
| OptiScaler | main branch (compiled from source) |
| DLSSG-to-FSR3 | v0.130 |
| Intel XeSS SDK | 3.0.1 |
| AMD FidelityFX SDK | 2.2 |

OptiScaler performs NVAPI emulation natively. No external FakeNvapi layer is required.

---

# Installation

1. Download the latest automated installer from our [Releases](https://github.com/dreyvinixz/FrameBridge/releases) page.
2. Select your game's executable folder.
3. Keep **DXGI** as the default injection method unless your game requires another proxy.
4. Launch the game.
5. Open the OptiScaler overlay with **INSERT**.

---

# Building

## Requirements

- Visual Studio 2022
- Windows SDK
- Git
- Inno Setup
- 7-Zip

Clone the repository:

```bash
git clone --recursive https://github.com/dreyvinixz/FrameBridge.git
cd FrameBridge
```

Build:

```powershell
msbuild OptiScaler.sln ^
    /p:Configuration=Release ^
    /p:Platform=x64
```

The GitHub Actions workflow automatically:

- Compiles OptiScaler
- Downloads external SDKs
- Packages the installer
- Publishes Releases

---

# Roadmap

The current focus is establishing a stable graphics runtime.

Future development includes:

- FrameBridge Launcher
- Telemetry Collector
- Automatic game detection
- Profile management
- Runtime diagnostics
- Intelligent recommendations

---

# Contributing

Contributions are welcome.

Please read **CONTRIBUTING.md** before opening a Pull Request.

Code style:

- `pch.h` must be included first in every `.cpp`
- Never include `pch.h` from header files
- Follow the project's `.clang-format`

---

# Credits

FrameBridge would not exist without these amazing projects.

- [OptiScaler](https://github.com/optiscaler/OptiScaler) — The incredible core graphics runtime engine
- [Nukem9 DLSSG-to-FSR3](https://github.com/Nukem9/dlssg-to-fsr3) — Frame generation module
- [Intel XeSS](https://github.com/intel/xess) — XeSS SDK
- [AMD FidelityFX SDK](https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK) — FSR SDK

---

# License

Licensed under **GPL-3.0**.

See **LICENSE** for details.