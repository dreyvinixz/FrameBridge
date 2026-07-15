# Spoofing
Except from first generation of DLSS2 games they all have some kind of NVidia verifying to enable DLSS option.   
To bypass these checks modders have developed some tools.

## Windows
### Nvapi
For spoofing Nvapi calls, **Fakenvapi** is used. This is needed for enabling DLSS support for some games (like Shadow of the Tomb Raider etc.). 

Also as a **big bonus**, FakeNvapi adds support for AMD's AntiLag 2 & LatencyFlex to reduce input latency in games that support Nvidia's Reflex.

> [!TIP]
> **FrameBridge bundles Fakenvapi natively.** You do not need to download or install it separately. The installer automatically places `fakenvapi.dll` in your game directory and FrameBridge loads it out of the box when required.

### DXGI
OptiScaler have built-in DXGI spoofing option which is enabled by default when working as non-nvngx (not as `nvngx.dll`).  

#### d3d12-proxy
Alternatively for spoofing DXGI adapter checks d3d12-proxy can be used. This mod reports your GPU as RTX 4090.   
##### Usage 
Just place dxgi.dll file next to games executable.  
##### Link 
[d3d12-proxy](https://github.com/cdozdil/d3d12-proxy/releases)   

### Vulkan
OptiScaler have built-in Vulkan spoofing options when working as non-nvngx (not as `nvngx.dll`).  
Vulkan spoofing by default disabled and should be enabled from `OptiScaler.ini` when needed.
```ini
; Enables Nvidia GPU spoofing for Vulkan
; true or false - Default (auto) is false
Vulkan=auto

; Enables Nvidia extension spoofing for Vulkan
; true or false - Default (auto) is false
VulkanExtensionSpoofing=auto
```

#### vulkan-spoofer
Alternatively for spoofing `GetPhysicalDeviceProperties` checks vulkan-spoofer can be used. This mod reports your GPU as RTX 4090.   
Compatiblity is a bit hit and miss, works for No Man's Sky (not working with latest streamline patch) but not working with Doom Eternal.  
##### Usage 
Just place version.dll file next to games executable.  
##### Link 
[vulkan-spoofer](https://github.com/cdozdil/vulkan-spoofer/releases)   

## Linux
On Linux with you can use Wine & DXVK's embedded spoofing mechanisms. 

### DirectX & Vulkan
For DXGI & Vulkan spoofing just create a `dxvk.conf` file next to game's executable with this content or just download it from [here](https://raw.githubusercontent.com/cdozdil/CyberXeSS/imgui-intergration/dxvk.conf).
```ini
dxgi.customVendorId = 10de
dxgi.hideAmdGpu = True
dxgi.hideNvidiaGpu = False
dxgi.customDeviceId = 2684
dxgi.customDeviceDesc = "NVIDIA GeForce RTX 4090"
```
### NVAPI
For spoofing NVAPI with Proton set this envvar `PROTON_FORCE_NVAPI=1`

## Goghor's DLSS Unlocker's
Goghor have created DLSS Unlocker mods for a lot of games which can be found on his [Nexus](https://www.nexusmods.com/spidermanmilesmorales/users/12564231?tab=user+files&BH=0) profile.   
For example as far as I know for Doom Eternal still only way to enable DLSS is his mod.
