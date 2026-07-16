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

## ❤️ Powered by OptiScaler

FrameBridge wouldn't exist without **OptiScaler**.

OptiScaler is the graphics runtime responsible for:

- API hooking
- DLSS / FSR / XeSS translation
- Frame Generation integration
- NVAPI emulation
- In-game overlay

If you're interested in how the graphics runtime works internally, please visit the official repository:

➡️ **https://github.com/optiscaler/OptiScaler**

FrameBridge focuses on everything around that runtime: installation, automation, packaging and making it easier for everyone to use.

---

## 📦 Included Components

| Component | Source |
|-----------|--------|
| OptiScaler | https://github.com/optiscaler/OptiScaler |
| DLSSG-to-FSR3 | Nukem9 |
| Intel XeSS SDK | Intel |
| AMD FidelityFX SDK | AMD |

---

## 🚀 Installation

1. Download the latest release.
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

- **OptiScaler** — The graphics runtime that powers FrameBridge.
- **Nukem9** — DLSSG-to-FSR3 Frame Generation.
- **Intel** — XeSS SDK.
- **AMD GPUOpen** — FidelityFX SDK.

Please consider starring the **OptiScaler** repository as well if you enjoy using FrameBridge ❤️

---

## 📄 License

FrameBridge is licensed under the **GPL-3.0 License**.