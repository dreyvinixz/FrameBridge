[CmdletBinding()]
param(
    [string]$RepositoryRoot = (Resolve-Path (Join-Path $PSScriptRoot ".."))
)

$ErrorActionPreference = "Stop"

function Assert-Condition {
    param(
        [bool]$Condition,
        [string]$Message
    )

    if (-not $Condition) {
        throw $Message
    }
}

function Assert-ProjectEntry {
    param(
        [string]$ProjectContents,
        [string]$Entry
    )

    Assert-Condition -Condition ($ProjectContents.Contains($Entry)) -Message "Project is missing required entry: $Entry"
}

$RepositoryRoot = (Resolve-Path $RepositoryRoot).Path
$manifestPath = Join-Path $RepositoryRoot "manifests\runtime-dependencies.json"
$projectPath = Join-Path $RepositoryRoot "OptiScaler\OptiScaler.vcxproj"
$configHeaderPath = Join-Path $RepositoryRoot "OptiScaler\Config.h"
$configSourcePath = Join-Path $RepositoryRoot "OptiScaler\Config.cpp"
$ffxPath = Join-Path $RepositoryRoot "OptiScaler\upscalers\ffx\FFXFeature_Dx12.cpp"
$removedFsr31Path = Join-Path $RepositoryRoot "OptiScaler\upscalers\fsr31\FSR31Feature_Dx12.cpp"

Assert-Condition -Condition (Test-Path $manifestPath) -Message "Missing runtime dependency manifest."
Assert-Condition -Condition (Test-Path $projectPath) -Message "Missing OptiScaler project."
Assert-Condition -Condition (Test-Path $configHeaderPath) -Message "Missing configuration header."
Assert-Condition -Condition (Test-Path $configSourcePath) -Message "Missing configuration source."
Assert-Condition -Condition (Test-Path $ffxPath) -Message "Missing active FFX DX12 backend."
Assert-Condition -Condition (-not (Test-Path $removedFsr31Path)) -Message "Removed FSR 3.1 DX12 backend is present."

$manifest = Get-Content $manifestPath -Raw | ConvertFrom-Json
Assert-Condition -Condition ($manifest.runtime.import_strategy -eq "vendor-snapshot") -Message "Runtime import strategy must be vendor-snapshot."
Assert-Condition -Condition ($manifest.runtime.target_upstream_commit -match "^[0-9a-f]{40}$") -Message "Target upstream commit must be pinned."

$releaseInputs = @{}
foreach ($asset in $manifest.release_inputs) {
    Assert-Condition -Condition ($asset.sha256 -match "^[0-9A-F]{64}$") -Message "Release asset '$($asset.id)' must have a SHA-256 hash."
    $releaseInputs[$asset.id] = $asset
}

Assert-Condition -Condition $releaseInputs.ContainsKey("amd-fsr4-upscaler") -Message "FSR4 release asset is missing from the manifest."
Assert-Condition -Condition $releaseInputs.ContainsKey("nukem-dlssg-to-fsr3") -Message "DLSSG release asset is missing from the manifest."

$fsr4 = $releaseInputs["amd-fsr4-upscaler"]
Assert-Condition -Condition ($fsr4.official_source_release -eq "https://github.com/GPUOpen-LibrariesAndSDKs/FidelityFX-SDK/releases/tag/v2.0.0") -Message "FSR4 official release source must be pinned."
Assert-Condition -Condition ($fsr4.official_sha256 -match "^[0-9A-F]{64}$") -Message "FSR4 official SHA-256 must be recorded."
Assert-Condition -Condition ($fsr4.official_signature_status -eq "valid") -Message "FSR4 official signature verification must be recorded."
Assert-Condition -Condition ($fsr4.release_eligibility -eq "approved") -Message "FSR4 release input is blocked: $($fsr4.release_eligibility)"

$dlssg = $releaseInputs["nukem-dlssg-to-fsr3"]
Assert-Condition -Condition ($dlssg.source_commit -match "^[0-9a-f]{40}$") -Message "DLSSG source commit must be pinned."
foreach ($asset in $dlssg.source_assets) {
    Assert-Condition -Condition ($asset.sha256 -match "^[0-9A-F]{64}$") -Message "DLSSG source asset '$($asset.path)' must have a SHA-256 hash."
}

$fsr4Path = Join-Path $RepositoryRoot "installer\assets\fsr4\amd_fidelityfx_upscaler_dx12.dll"
Assert-Condition -Condition (Test-Path $fsr4Path) -Message "Pinned FSR4 release asset is missing."
$fsr4Hash = (Get-FileHash $fsr4Path -Algorithm SHA256).Hash
Assert-Condition -Condition ($fsr4Hash -eq $fsr4.sha256) -Message "FSR4 release asset hash differs from the manifest."
Assert-Condition -Condition ($fsr4Hash -eq $fsr4.official_sha256) -Message "FSR4 release asset does not match the pinned official AMD binary."
$fsr4Signature = Get-AuthenticodeSignature $fsr4Path
Assert-Condition -Condition ($fsr4Signature.Status -eq "Valid") -Message "FSR4 release asset must have a valid Authenticode signature."
Assert-Condition -Condition ($fsr4Signature.SignerCertificate.Subject -eq $fsr4.signer_subject) -Message "FSR4 release asset signer differs from the manifest."

$project = Get-Content $projectPath -Raw
foreach ($entry in @(
    "runtime\RuntimeConfiguration.h",
    "runtime\RuntimeConfiguration.cpp",
    "runtime\RuntimeCapabilities.h",
    "runtime\RuntimeCapabilities.cpp",
    "runtime\RuntimeCapabilityTypes.h"
)) {
    Assert-ProjectEntry -ProjectContents $project -Entry $entry
}

$configHeader = Get-Content $configHeaderPath -Raw
$configSource = Get-Content $configSourcePath -Raw
Assert-Condition -Condition ($configHeader.Contains("void NotifyRuntimeConfigurationChanged();")) -Message "Configuration changes do not publish runtime snapshot events."
Assert-Condition -Condition ($configSource.Contains("NotifyRuntimeConfigurationChanged();")) -Message "INI loading does not publish a final runtime snapshot event."
$volatileMutationStart = $configHeader.IndexOf("constexpr void set_volatile_value")
$normalMutationStart = $configHeader.IndexOf("constexpr CustomOptional& operator=(const T& value)")
Assert-Condition -Condition ($volatileMutationStart -ge 0 -and $normalMutationStart -gt $volatileMutationStart) -Message "Cannot isolate CustomOptional mutation paths."
$volatileMutation = $configHeader.Substring($volatileMutationStart, $normalMutationStart - $volatileMutationStart)
Assert-Condition -Condition (-not $volatileMutation.Contains("NotifyRuntimeConfigurationChanged")) -Message "Volatile configuration writes must not refresh the snapshot from a hot path."

$ffxSource = Get-Content $ffxPath -Raw
Assert-Condition -Condition ($ffxSource.Contains("RuntimeConfiguration::Instance().GetSnapshot()")) -Message "FFX snapshot is not connected."
Assert-Condition -Condition (($ffxSource.Split("RuntimeConfiguration::Instance().RefreshFromConfig()").Count - 1) -eq 1) -Message "FFX snapshot refresh lifecycle is not singular."

$evaluateStart = $ffxSource.IndexOf("bool FFXFeatureDx12::EvaluateInternal")
$evaluateEnd = $ffxSource.IndexOf("bool FFXFeatureDx12::InitFFX", $evaluateStart)
Assert-Condition -Condition ($evaluateStart -ge 0 -and $evaluateEnd -gt $evaluateStart) -Message "Cannot isolate FFX evaluation path."
$evaluateSource = $ffxSource.Substring($evaluateStart, $evaluateEnd - $evaluateStart)
$directConfigReads = @($evaluateSource -split "`r?`n" | Where-Object {
    $_ -match "Config::Instance" -and $_ -notmatch "set_volatile_value"
})
Assert-Condition -Condition ($directConfigReads.Count -eq 0) -Message "FFX evaluation path contains direct Config reads."

$trackedArchives = @(& git -C $RepositoryRoot ls-files "*.rar")
Assert-Condition -Condition ($LASTEXITCODE -eq 0) -Message "Unable to inspect tracked release archives."
Assert-Condition -Condition ($trackedArchives.Count -eq 0) -Message "RAR archives must not be tracked: $($trackedArchives -join ', ')"

Write-Host "FrameBridge integration verification passed."
