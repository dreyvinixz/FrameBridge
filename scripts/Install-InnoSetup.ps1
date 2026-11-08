[CmdletBinding()]
param()

$ErrorActionPreference = "Stop"
$version = "6.7.4"
$url = "https://github.com/jrsoftware/issrc/releases/download/is-6_7_4/innosetup-$version.exe"
$expectedSha256 = "A549929319F9120716677C685535CA4671E4B937B5FDC87E2585F32D8EB30E8C"
$installerPath = Join-Path $env:RUNNER_TEMP "innosetup-$version.exe"

Invoke-WebRequest -Uri $url -OutFile $installerPath
$actualSha256 = (Get-FileHash -LiteralPath $installerPath -Algorithm SHA256).Hash
if ($actualSha256 -ne $expectedSha256) {
    throw "Official Inno Setup $version installer SHA-256 mismatch: $actualSha256"
}

$signature = Get-AuthenticodeSignature -LiteralPath $installerPath
if ($signature.Status -ne "Valid" -or $signature.SignerCertificate.Subject -notlike "*Pyrsys B.V.*") {
    throw "Official Inno Setup $version installer has an invalid or unexpected Authenticode signature."
}

$installRoot = Join-Path $env:RUNNER_TEMP "InnoSetup-$version"
$install = Start-Process -FilePath $installerPath -ArgumentList @(
    "/VERYSILENT", "/SUPPRESSMSGBOXES", "/NORESTART", "/SP-", "/DIR=$installRoot"
) -Wait -PassThru -WindowStyle Hidden
if ($install.ExitCode -ne 0) {
    throw "Inno Setup $version installation failed with exit code $($install.ExitCode)."
}

$iscc = Join-Path $installRoot "ISCC.exe"
if (-not (Test-Path -LiteralPath $iscc -PathType Leaf)) { throw "ISCC.exe was not found after installing Inno Setup $version." }

if ($env:GITHUB_OUTPUT) {
    "iscc=$iscc" | Add-Content -LiteralPath $env:GITHUB_OUTPUT -Encoding utf8
}
Write-Host "Installed signed Inno Setup $version from the pinned official release."
