#define MyAppName "FrameBridge"
#define MyAppVersion "1.00.000.0"
#define MyAppPublisher "dreyvinixz"
#define MyAppExeName "my-game.exe"

[Setup]
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher={#MyAppPublisher}
DefaultDirName=c:\games\my-game\bin\x64
DisableProgramGroupPage=yes
DirExistsWarning=no
LicenseFile=assets\LICENSE.rtf
PrivilegesRequired=lowest
OutputBaseFilename=framebridge-setup-{#MyAppVersion}
AppendDefaultDirName=no
; bzip/9 is safest for AV false positives, lzma2 reduces size by ~50%
Compression=lzma2
InternalCompressLevel=ultra
LZMADictionarySize=231072
LZMAUseSeparateProcess=yes
LZMANumFastBytes=200
SolidCompression=no
WizardStyle=modern
UsePreviousAppDir=no
InfoBeforeFile=assets\Intro.txt
EnableDirDoesntExistWarning=yes
Uninstallable=yes
RestartApplications=no
RestartIfNeededByRun=no
TerminalServicesAware=no
CreateUninstallRegKey=no
LanguageDetectionMethod=none
CloseApplications=no

[Types]
Name: "full"; Description: "Preferred installation (DLL package)"
Name: "debug"; Description: "Troubleshooting installation"
Name: "custom"; Description: "Custom installation"; Flags: iscustom
Name: "experimental"; Description: "Experimental support for AMD and Intel GPUs"

[Components]
; Main module - choose one DLL injection method
Name: mainfiles; Description: Install main FrameBridge files (game dependant); Types: full
Name: mainfiles/dllversion; Description: Install as a version.dll file (optimal compatibility); Types: full; Flags: exclusive
Name: mainfiles/dllwinmm; Description: Install as a winmm.dll file (if version.dll didn't work); Types: custom; Flags: exclusive
Name: mainfiles/asiversion; Description: Install as an ASI plugin (if the game is heavily modded); Types: custom debug; Flags: exclusive
Name: mainfiles/dlldxgi; Description: Install as a dxgi.dll file (if nothing above works); Types: custom; Flags: exclusive
Name: mainfiles/dlld3d12; Description: Install as a d3d12.dll file (required for Final Fantasy 7 Rebirth); Types: custom; Flags: exclusive

; Non-NVIDIA GPU support
Name: nonnvidia; Description: Enable support for AMD and Intel GPUs (DON'T INSTALL if you have a NVIDIA GPU); Types: experimental custom
Name: nonnvidia/localdir; Description: Install NVIDIA Runtime files into game directory; Types: experimental custom; Flags: exclusive

; OptiScaler upscaler (compiled from source)
Name: upscalers; Description: Install OptiScaler upscaler with XeSS and FSR support; Flags: fixed; Types: full debug custom

; DLSSG-to-FSR3 module
Name: framegen; Description: Install DLSSG-to-FSR3 frame generation module (Nukem9 v0.130); Types: full debug custom; Flags: fixed

; Optional/debug files
Name: optional; Description: Install optional files; Flags: fixed; Types: full debug custom
Name: optional/fgdebug; Description: (optional) Debug configuration for DLSSG-to-FSR3 module; Types: debug custom

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Tasks]

[Files]
; ============================================================
; Cleanup - create placeholder files
; ============================================================
Source: "assets\configs\OptiScaler.ini"; DestDir: "{app}"; DestName: "dlss-enabler-xess.dll"; Flags: ignoreversion deleteafterinstall; Components: framegen
Source: "assets\configs\OptiScaler.ini"; DestDir: "{app}"; DestName: "dlss-enabler-fsr.dll"; Flags: ignoreversion deleteafterinstall; Components: framegen

; ============================================================
; NVIDIA Environment (runtime for non-NVIDIA GPUs)
; ============================================================
Source: "assets\nvidia-env\dxgi.dll"; DestDir: "{app}"; Components: nonnvidia/localdir mainfiles/dlldxgi
Source: "assets\nvidia-env\dlss-finder.bin"; DestName: "dlss-finder.exe"; DestDir: "{app}"; Components: nonnvidia/localdir
Source: "assets\nvidia-env\nvapi64-proxy.dll"; DestName: "nvapi64-proxy.dll"; DestDir: "{app}"; Components: nonnvidia/localdir

; ============================================================
; DLSSG-to-FSR3 module (Nukem9)
; ============================================================
Source: "assets\dlssg\dlssg_to_fsr3.ini"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist; Components: optional/fgdebug
Source: "assets\dlssg\dlssg_to_fsr3_amd_is_better.dll"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist; Components: framegen
Source: "assets\dlssg\nvngx.dll"; DestDir: "{app}"; DestName: "nvngx-wrapper.dll"; Flags: ignoreversion skipifsourcedoesntexist; Components: framegen
Source: "assets\dlssg\nvngx.dll"; DestDir: "{app}"; DestName: "_nvngx.dll"; Flags: ignoreversion skipifsourcedoesntexist; Components: nonnvidia/localdir
Source: "assets\dlssg\READ ME.txt"; DestDir: "{app}/licenses"; DestName: "READ ME (DLSSG to FSR3 mod).txt"; Flags: ignoreversion skipifsourcedoesntexist; Components: framegen
Source: "assets\dlssg\LICENSE.txt"; DestDir: "{app}/licenses"; DestName: "LICENSE (DLSSG to FSR3 mod).txt"; Flags: ignoreversion skipifsourcedoesntexist; Components: framegen

; ============================================================
; OptiScaler (compiled from source or downloaded)
; ============================================================
Source: "build\OptiScaler.dll"; DestDir: "{app}"; DestName: "dlss-enabler-upscaler.dll"; Flags: ignoreversion skipifsourcedoesntexist; Components: upscalers
Source: "assets\configs\OptiScaler.ini"; DestDir: "{app}"; DestName: "nvngx.ini"; Flags: ignoreversion skipifsourcedoesntexist; Components: upscalers

; XeSS and FidelityFX libraries
Source: "build\libxess.dll"; DestDir: "{app}"; Flags: uninsneveruninstall skipifsourcedoesntexist; Components: upscalers
Source: "build\libxess_dx11.dll"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist; Components: upscalers
Source: "build\amd_fidelityfx_*.dll"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist; Components: upscalers
Source: "build\D3D12_OptiScaler\*"; DestDir: "{app}\D3D12_OptiScaler"; Flags: ignoreversion skipifsourcedoesntexist; Components: upscalers

; Fakenvapi (bundled with OptiScaler 0.9+)
Source: "build\fakenvapi.dll"; DestDir: "{app}"; Flags: ignoreversion skipifsourcedoesntexist; Components: upscalers

; License files
Source: "build\licenses\XeSS_LICENSE.txt"; DestDir: "{app}/licenses"; Flags: ignoreversion skipifsourcedoesntexist; Components: upscalers
Source: "build\licenses\FidelityFX_LICENSE.md"; DestDir: "{app}/licenses"; Flags: ignoreversion skipifsourcedoesntexist; Components: upscalers
Source: "build\licenses\DirectX_LICENSE.txt"; DestDir: "{app}/licenses"; Flags: ignoreversion skipifsourcedoesntexist; Components: upscalers

; ============================================================
; Main module (the DLL loader/hook)
; ============================================================
Source: "build\OptiScaler.dll"; DestDir: "{app}/plugins"; DestName: "framebridge.asi"; Flags: confirmoverwrite skipifsourcedoesntexist; Components: mainfiles/asiversion
Source: "build\OptiScaler.dll"; DestDir: "{app}"; DestName: "framebridge.dll"; Flags: confirmoverwrite skipifsourcedoesntexist; Components: mainfiles/dlldxgi
Source: "build\OptiScaler.dll"; DestDir: "{app}"; DestName: "version.dll"; Flags: confirmoverwrite skipifsourcedoesntexist; Components: mainfiles/dllversion
Source: "build\OptiScaler.dll"; DestDir: "{app}"; DestName: "winmm.dll"; Flags: confirmoverwrite skipifsourcedoesntexist; Components: mainfiles/dllwinmm
Source: "build\OptiScaler.dll"; DestDir: "{app}"; DestName: "d3d12.dll"; Flags: confirmoverwrite skipifsourcedoesntexist; Components: mainfiles/dlld3d12

; ============================================================
; Documentation
; ============================================================
Source: "docs\README.txt"; DestDir: "{app}/licenses"; Flags: ignoreversion skipifsourcedoesntexist; Components: mainfiles

[Icons]

[Run]
Filename: "{app}\licenses\README.txt"; Description: "View the FrameBridge README file"; Flags: postinstall shellexec skipifsilent skipifdoesntexist
Filename: "{app}\nvngx.ini"; Description: "Edit the configuration file (optional)"; Flags: postinstall shellexec skipifsilent unchecked
Filename: "{app}\dlss-finder.exe"; Parameters: "/s"; StatusMsg: "Disabling NVIDIA signature checks for DLSS 3.7"; WorkingDir: "{app}"; Description: "DLSS 3.7 activation step"; Flags: skipifsilent skipifdoesntexist
