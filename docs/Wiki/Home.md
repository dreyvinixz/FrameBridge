# Welcome to the FrameBridge wiki!

Welcome to the official documentation for **FrameBridge**, a community-driven fork of OptiScaler that brings together cross-vendor upscaling and frame generation technologies into an easy-to-use, all-in-one installer!

![FrameBridge Banner](../images/banner.png)

---

## 📖 Table of Contents

1. [What is FrameBridge?](#-what-is-framebridge)
2. [Supported Technologies](#-supported-technologies)
3. [Installation Guide](#-installation-guide)
4. [Configuration (OptiScaler.ini)](#-configuration)
5. [Troubleshooting & FAQ](#-troubleshooting--faq)

---

## 🚀 What is FrameBridge?

FrameBridge takes the amazing core engine from **OptiScaler** and integrates it with a fully automated, user-friendly CI/CD pipeline and an "all-in-one installer" philosophy. 

Instead of dealing with multiple DLLs and manual file placements, FrameBridge provides a compiled installer that correctly drops the necessary files (`nvngx.dll`, `version.dll`, `dxgi.dll`, etc.) directly into your game folders, bridging upscalers and frame generation mods seamlessly.

## ⚙️ Supported Technologies

FrameBridge enables you to swap and mix the following technologies regardless of your GPU vendor (NVIDIA, AMD, or Intel):

* **Upscalers Supported:**
  * NVIDIA DLSS (Deep Learning Super Sampling)
  * AMD FSR (FidelityFX Super Resolution)
  * Intel XeSS (Xe Super Sampling)
* **Frame Generation:**
  * DLSS-G to FSR3 (via Nukem9's mod integration)
  * OptiScaler's native Frame Gen wrappers

## 💿 Installation Guide

Installing FrameBridge is straightforward thanks to the included Inno Setup package.

1. Go to the [Releases](https://github.com/dreyvinixz/FrameBridge/releases) tab.
2. Download the latest `framebridge-setup-X.X.X.exe`.
3. Run the installer and point the **Installation Directory** to your game's executable folder (where the main `.exe` or current `nvngx.dll` is located).
4. Select your preferred injection method (e.g., `version.dll` for general compatibility, or `dxgi.dll`).
5. (Optional) Check the box for "Experimental support for AMD and Intel GPUs" if you don't own an RTX card.
6. Click Install!

## 🔧 Configuration

After installation, you can tweak the settings to your liking by editing the `OptiScaler.ini` file located next to the game executable.

Some common settings you might want to adjust:
* **[UpscaleRatio]**: Override the game's default quality presets.
* **[XeSS]** or **[FSR]**: Force the use of a specific upscaler backbone.
* **[FrameGeneration]**: Toggle the frame generation module on or off.

> **Tip:** If the game crashes on startup, try changing the injection method by renaming `version.dll` to `winmm.dll` or reinstalling using a different option.

## 🛠️ Troubleshooting & FAQ

* **The game doesn't launch after installing!**
  * Try deleting `version.dll` and using the installer to inject via `dxgi.dll` or `winmm.dll`. Some games have anti-cheat or specific loading patterns that conflict with `version.dll`.

* **Can I use this on multiplayer games?**
  * **No.** We highly advise against using FrameBridge or any DLL injectors in multiplayer games with Anti-Cheat (like EAC or BattlEye). You will likely get banned.

* **DLSS Frame Generation option is greyed out in-game?**
  * Make sure you installed the "DLSSG-to-FSR3" module during the setup (enabled by default) and that your Windows Hardware Accelerated GPU Scheduling (HAGS) is turned ON.

---

*Thank you for using FrameBridge! Feel free to open an Issue on GitHub if you encounter any bugs.*
