[CmdletBinding()]
param(
    [string]$RepositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot ".."))
)

$ErrorActionPreference = "Stop"
$RepositoryRoot = (Resolve-Path $RepositoryRoot).Path
Push-Location $RepositoryRoot
try {
    function Assert-Hash {
        param([string]$Path, [string]$Expected, [string]$Label)
        $actual = (Get-FileHash -LiteralPath $Path -Algorithm SHA256).Hash
        if ($actual -ne $Expected) {
            throw "$Label SHA-256 mismatch: expected $Expected, got $actual"
        }
    }

    $manifest = Get-Content "manifests\runtime-dependencies.json" -Raw | ConvertFrom-Json
    $fsr4 = @($manifest.release_inputs | Where-Object { $_.id -eq "amd-fsr4-upscaler" })[0]
    $fsr4Path = "installer\assets\fsr4\amd_fidelityfx_upscaler_dx12.dll"
    if (-not (Test-Path -LiteralPath $fsr4Path)) { throw "Pinned FSR4 asset is missing: $fsr4Path" }
    Assert-Hash -Path $fsr4Path -Expected $fsr4.sha256 -Label "FSR4 asset"

    $outputRoot = "x64\Release\a"
    foreach ($required in @("OptiScaler.dll", "libxess.dll", "libxess_dx11.dll", "OptiScaler.ini")) {
        if (-not (Test-Path -LiteralPath (Join-Path $outputRoot $required) -PathType Leaf)) {
            throw "Required Release output is missing: $(Join-Path $outputRoot $required)"
        }
    }

    New-Item -ItemType Directory -Force -Path "installer\build", "installer\build\licenses", `
        "installer\assets\configs", "installer\assets\dlssg" | Out-Null

    Copy-Item -LiteralPath (Join-Path $outputRoot "OptiScaler.dll") -Destination "installer\build\OptiScaler.dll" -Force
    Copy-Item -LiteralPath (Join-Path $outputRoot "libxess.dll") -Destination "installer\build\libxess.dll" -Force
    Copy-Item -LiteralPath (Join-Path $outputRoot "libxess_dx11.dll") -Destination "installer\build\libxess_dx11.dll" -Force

    foreach ($pattern in @("amd_fidelityfx_*.dll")) {
        $files = @(Get-ChildItem -Path (Join-Path $outputRoot $pattern) -File -ErrorAction SilentlyContinue)
        if ($files.Count -eq 0) { throw "Required runtime output pattern matched no files: $pattern" }
        Copy-Item -Path (Join-Path $outputRoot $pattern) -Destination "installer\build" -Force
    }

    foreach ($license in @("XeSS_LICENSE.txt", "FidelityFX_v1_LICENSE.md", "FidelityFX_v2_LICENSE.md", "DirectX_LICENSE.txt")) {
        $source = Join-Path $outputRoot "Licenses\$license"
        if (-not (Test-Path -LiteralPath $source -PathType Leaf)) { throw "Build license is missing: $source" }
        Copy-Item -LiteralPath $source -Destination "installer\build\licenses\$license" -Force
    }

    $agility = Join-Path $outputRoot "D3D12_Optiscaler"
    if (Test-Path -LiteralPath $agility -PathType Container) {
        New-Item -ItemType Directory -Force -Path "installer\build\D3D12_OptiScaler" | Out-Null
        Copy-Item -Path (Join-Path $agility "*") -Destination "installer\build\D3D12_OptiScaler" -Recurse -Force
    }

    Copy-Item -LiteralPath (Join-Path $outputRoot "OptiScaler.ini") -Destination "installer\assets\configs\OptiScaler.ini" -Force
    Copy-Item -LiteralPath "Config.md" -Destination "installer\assets\configs\Config.md" -Force
    Copy-Item -LiteralPath "LICENSE" -Destination "installer\assets\LICENSE.rtf" -Force
    Copy-Item -LiteralPath "README.md" -Destination "installer\assets\README.md" -Force

    foreach ($license in @("LICENSE.TXT", "FTL.TXT", "GPLv2.TXT")) {
        if (-not (Test-Path -LiteralPath (Join-Path "external\freetype" $license) -PathType Leaf)) {
            throw "FreeType license file is missing: $license"
        }
    }
    if (-not (Test-Path -LiteralPath "external\fakenvapi\LICENSE" -PathType Leaf)) {
        throw "FakeNVAPI license file is missing."
    }

    $dlssg = @($manifest.release_inputs | Where-Object { $_.id -eq "nukem-dlssg-to-fsr3" })[0]
    $archivePath = Join-Path $env:RUNNER_TEMP "dlssg-to-fsr3.zip"
    Invoke-WebRequest -Uri $dlssg.current_ci_download -OutFile $archivePath
    Assert-Hash -Path $archivePath -Expected $dlssg.sha256 -Label "Nukem9 DLSSG archive"
    $extractPath = Join-Path $env:RUNNER_TEMP "dlssg-to-fsr3"
    Expand-Archive -LiteralPath $archivePath -DestinationPath $extractPath -Force
    Copy-Item -Path (Join-Path $extractPath "*") -Destination "installer\assets\dlssg" -Recurse -Force

    foreach ($asset in $dlssg.source_assets) {
        $destination = switch ($asset.path) {
            "resources/dlssg_to_fsr3.ini" { "installer\assets\dlssg\dlssg_to_fsr3.ini" }
            "resources/read_me_generic_dll.txt" { "installer\assets\dlssg\READ ME.txt" }
            default { throw "Unmapped pinned DLSSG asset: $($asset.path)" }
        }
        $uri = "https://raw.githubusercontent.com/Nukem9/dlssg-to-fsr3/$($dlssg.source_commit)/$($asset.path)"
        Invoke-WebRequest -Uri $uri -OutFile $destination
        Assert-Hash -Path $destination -Expected $asset.sha256 -Label $asset.path
    }

    foreach ($required in @(
        "installer\assets\dlssg\dlssg_to_fsr3_amd_is_better.dll",
        "installer\assets\dlssg\READ ME.txt",
        "installer\assets\dlssg\LICENSE.txt"
    )) {
        if (-not (Test-Path -LiteralPath $required -PathType Leaf)) { throw "Installer asset is missing: $required" }
    }

    Write-Host "Installer payload prepared from Release x64 outputs and manifest-pinned assets."
}
finally {
    Pop-Location
}
