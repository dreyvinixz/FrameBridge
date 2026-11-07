[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [string] $OutputDirectory
)

$ErrorActionPreference = 'Stop'

$repositoryRoot = Split-Path -Parent $PSScriptRoot
$manifestPath = Join-Path $repositoryRoot 'manifests\runtime-dependencies.json'
$manifest = Get-Content -LiteralPath $manifestPath -Raw | ConvertFrom-Json
$dependency = @($manifest.pending_dependency_updates | Where-Object { $_.name -eq 'FreeType' })[0]
if ($null -eq $dependency -or -not $dependency.source_commit -or -not $dependency.source_tag) {
    throw 'FreeType source tag and commit must be pinned in manifests/runtime-dependencies.json.'
}

$cmakeCommand = Get-Command cmake.exe -ErrorAction Stop
$gitCommand = Get-Command git.exe -ErrorAction Stop
$outputPath = [System.IO.Path]::GetFullPath($OutputDirectory)
$workPath = Join-Path ([System.IO.Path]::GetTempPath()) ("FrameBridge-FreeType-" + [guid]::NewGuid().ToString('N'))
$sourcePath = Join-Path $workPath 'source'
$buildPath = Join-Path $workPath 'build'

New-Item -ItemType Directory -Path $workPath -Force | Out-Null
if (Test-Path -LiteralPath $outputPath) {
    throw "Output directory already exists; choose a fresh path: $outputPath"
}

& $gitCommand.Source clone --depth 1 --branch $dependency.source_tag $dependency.source_repository $sourcePath
if ($LASTEXITCODE -ne 0) { throw 'Could not clone the pinned FreeType source tag.' }

$actualCommit = (& $gitCommand.Source -C $sourcePath rev-parse HEAD).Trim()
if ($LASTEXITCODE -ne 0 -or $actualCommit -ne $dependency.source_commit) {
    throw "FreeType tag resolved to $actualCommit, expected $($dependency.source_commit)."
}

$cmakeArgs = @(
    '-S', $sourcePath,
    '-B', $buildPath,
    '-G', 'Visual Studio 17 2022',
    '-A', 'x64',
    '-D', 'BUILD_SHARED_LIBS=OFF',
    '-D', 'FT_DISABLE_ZLIB=ON',
    '-D', 'FT_DISABLE_BZIP2=ON',
    '-D', 'FT_DISABLE_PNG=ON',
    '-D', 'FT_DISABLE_HARFBUZZ=ON',
    '-D', 'FT_DISABLE_BROTLI=ON',
    '-D', 'FT_DISABLE_HVF=ON'
)
& $cmakeCommand.Source @cmakeArgs
if ($LASTEXITCODE -ne 0) { throw 'FreeType CMake configuration failed.' }

& $cmakeCommand.Source --build $buildPath --config Release --target freetype --parallel
if ($LASTEXITCODE -ne 0) { throw 'FreeType Release x64 build failed.' }

& $cmakeCommand.Source --install $buildPath --config Release --prefix $outputPath
if ($LASTEXITCODE -ne 0) { throw 'FreeType install/staging failed.' }

$libraryPath = Join-Path $outputPath 'lib\freetype.lib'
$headerPath = Join-Path $outputPath 'include\freetype2\freetype\freetype.h'
if (-not (Test-Path -LiteralPath $libraryPath) -or -not (Test-Path -LiteralPath $headerPath)) {
    throw 'The staged FreeType library or public headers are missing.'
}

$versionText = Get-Content -LiteralPath $headerPath -Raw
$versionMajor = [regex]::Match($versionText, '(?m)^#define FREETYPE_MAJOR\s+(\d+)').Groups[1].Value
$versionMinor = [regex]::Match($versionText, '(?m)^#define FREETYPE_MINOR\s+(\d+)').Groups[1].Value
$versionPatch = [regex]::Match($versionText, '(?m)^#define FREETYPE_PATCH\s+(\d+)').Groups[1].Value
$actualVersion = "$versionMajor.$versionMinor.$versionPatch"
if ($actualVersion -ne $dependency.target) {
    throw "Staged FreeType headers report $actualVersion, expected $($dependency.target)."
}

$disabledDependencies = @('zlib', 'bzip2', 'png', 'harfbuzz', 'brotli', 'hvf') |
    ForEach-Object { "disabled: $_" }
$buildInfo = [ordered]@{
    name = 'FreeType'
    version = $actualVersion
    source_repository = $dependency.source_repository
    source_tag = $dependency.source_tag
    source_commit = $actualCommit
    architecture = 'x64'
    configuration = 'Release'
    linkage = 'static'
    optional_dependencies = $disabledDependencies
    library_sha256 = (Get-FileHash -LiteralPath $libraryPath -Algorithm SHA256).Hash
}
$buildInfo | ConvertTo-Json -Depth 4 | Set-Content -LiteralPath (Join-Path $outputPath 'build-info.json') -Encoding utf8

Write-Host "Built FreeType $actualVersion for x64 Release."
Write-Host "Static library SHA256: $($buildInfo.library_sha256)"
Write-Host "Staged package: $outputPath"
