# 🏛️ FrameBridge Architecture

The diagram below illustrates how FrameBridge integrates the core OptiScaler engine and Nukem9's mod via an **all-in-one installer** to bridge the gap between native game requests and your hardware capabilities.

```mermaid
graph LR
    subgraph Input ["🎮 Native Game Requests"]
        DLSS_SR["DLSS Super Resolution"]
        XeSS_SR["XeSS Super Resolution"]
        FSR_SR["FSR Super Resolution"]
        DLSS_FG["DLSS Frame Generation"]
        REFLEX["NVIDIA Reflex"]
    end

    subgraph FrameBridge ["🌉 FrameBridge Architecture"]
        Installer{"All-in-One Installer"}
        
        subgraph Injection ["Injection Methods"]
            DLL_VER["version.dll / winmm.dll"]
            DLL_DXGI["dxgi.dll / framebridge.asi"]
        end
        
        subgraph CoreMods ["Core Engines"]
            OptiScaler["OptiScaler Engine"]
            Nukem9["DLSSG-to-FSR3 Mod"]
            FakeNvapi["FakeNvapi Module"]
        end
        
        Installer -->|Deploys to Game Dir| DLL_VER
        Installer -->|Deploys to Game Dir| DLL_DXGI
        
        DLL_VER -->|Hooks & Loads| OptiScaler
        DLL_VER -->|Hooks & Loads| Nukem9
        DLL_DXGI -->|Hooks & Loads| OptiScaler
        DLL_DXGI -->|Hooks & Loads| Nukem9
        
        OptiScaler -.->|Latency Reduction| FakeNvapi
    end

    subgraph Output ["🖥️ Hardware Output"]
        FSR_OUT["FSR / FSR Frame Gen"]
        XeSS_OUT["XeSS / XeSS Frame Gen"]
        DLSS_OUT["DLSS (NVIDIA Only)"]
        AL2["AMD Anti-Lag 2"]
    end

    DLSS_SR -->|Intercepted by| Installer
    XeSS_SR -->|Intercepted by| Installer
    FSR_SR -->|Intercepted by| Installer
    DLSS_FG -->|Intercepted by| Installer
    REFLEX -->|Intercepted by| Installer

    OptiScaler -->|Translates to| FSR_OUT
    OptiScaler -->|Translates to| XeSS_OUT
    OptiScaler -->|Translates to| DLSS_OUT
    
    Nukem9 -->|Translates to| FSR_OUT
    
    FakeNvapi -->|Translates to| AL2
    
    classDef input fill:#2c3e50,stroke:#34495e,stroke-width:2px,color:#fff;
    classDef installer fill:#8e44ad,stroke:#9b59b6,stroke-width:2px,color:#fff;
    classDef output fill:#27ae60,stroke:#2ecc71,stroke-width:2px,color:#fff;
    classDef mods fill:#c0392b,stroke:#e74c3c,stroke-width:2px,color:#fff;
    classDef injection fill:#f39c12,stroke:#f1c40f,stroke-width:2px,color:#fff;

    class DLSS_SR,XeSS_SR,FSR_SR,DLSS_FG,REFLEX input;
    class Installer installer;
    class OptiScaler,Nukem9,FakeNvapi mods;
    class FSR_OUT,XeSS_OUT,DLSS_OUT,AL2 output;
    class DLL_VER,DLL_DXGI injection;
```

### How it works
1. **Input:** The game natively attempts to load upscaling/frame generation technologies (like DLSS).
2. **Installer:** Instead of manually moving files, the FrameBridge Installer drops the required interception DLLs (like `version.dll`) directly into your game directory.
3. **Core Engines:** The injection DLL loads the powerful **OptiScaler** and **DLSSG-to-FSR3** engines in the background.
4. **Output:** These engines translate the proprietary game requests into open standards (like FSR or XeSS) that run smoothly on your specific AMD, Intel, or NVIDIA graphics card.
