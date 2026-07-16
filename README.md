# FrameBridge

<p align="center">
  <img src="images/banner.png" alt="FrameBridge Banner">
</p>

<p align="center">
  <a href="https://github.com/dreyvinixz/FrameBridge/actions/workflows/build.yml">
    <img src="https://github.com/dreyvinixz/FrameBridge/actions/workflows/build.yml/badge.svg" alt="Build Status">
  </a>
</p>

FrameBridge is an open-source project that makes **OptiScaler** easier to install, configure, and use.

Instead of manually downloading DLLs, editing configuration files, and figuring out which files belong where, FrameBridge provides a simple installer that prepares everything for you.

It builds directly on top of the amazing **[OptiScaler](https://github.com/optiscaler/OptiScaler)** project while adding automation, packaging, configuration, and future tools like telemetry and a launcher.

---

## ✨ Features

- 🚀 Easy installer for OptiScaler
- ⚙️ Pre-configured settings for most games
- 🎮 Native support for Frame Generation
- 🖥️ Supports NVIDIA, AMD and Intel GPUs
- 🔄 Built automatically from source with GitHub Actions
- 📦 Ready-to-use releases
- 📖 Open source and community driven

---

## 🏛️ Architecture & OptiScaler

FrameBridge follows a simple design philosophy: **One Runtime. One Responsibility.**

It is built entirely around **[OptiScaler](https://github.com/optiscaler/OptiScaler)**, which handles all graphics rendering, API hooking, Frame Generation integration, and the in-game overlay. FrameBridge focuses purely on the ecosystem around it: installation, automation, packaging, and future tools.

```text
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
   DLSS • FSR • XeSS • Frame Gen
```

---

## 📦 Included Components

| Component | Source |
|-----------|--------|
| OptiScaler | [Official Repository](https://github.com/optiscaler/OptiScaler) |
| DLSSG-to-FSR3 | [Nukem9](https://github.com/Nukem9/dlssg-to-fsr3) |
| Intel XeSS SDK | [Intel](https://github.com/intel/xess) |
| AMD FidelityFX SDK | [AMD](https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK) |

---

## 🚀 Installation

1. Download the latest automated installer from our [Releases](https://github.com/dreyvinixz/FrameBridge/releases) page.
2. Run the installer.
3. Select your game's folder.
4. Keep **DXGI (Recommended)** selected.
5. Launch your game.
6. Press **INSERT** to open the OptiScaler overlay.

That's it. 🎉

---

## 🔨 Building

```bash
git clone --recursive https://github.com/dreyvinixz/FrameBridge.git
cd FrameBridge
```

Build with Visual Studio 2022 or let GitHub Actions do it automatically.

---

## 🤝 Contributing

Contributions, bug reports and suggestions are always welcome.

If you'd like to help improve FrameBridge, feel free to open an Issue or Pull Request.

---

## 🙏 Credits

This project stands on the shoulders of some incredible open-source work.

Special thanks to:

- **[OptiScaler](https://github.com/optiscaler/OptiScaler)** — The graphics runtime that powers FrameBridge.
- **[Nukem9](https://github.com/Nukem9/dlssg-to-fsr3)** — DLSSG-to-FSR3 Frame Generation.
- **[Intel](https://github.com/intel/xess)** — XeSS SDK.
- **[AMD GPUOpen](https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK)** — FidelityFX SDK.

Please consider starring the **OptiScaler** repository as well if you enjoy using FrameBridge ❤️

---

## 📄 License

FrameBridge is licensed under the **[GPL-3.0 License](LICENSE)**.