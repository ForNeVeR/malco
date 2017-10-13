param (
    $Configuration = 'Release',

    $Root = "$PSScriptRoot/..",
    $TestScripts = "$Root/tests/scripts",
    $malco = "$Root/build/$Configuration/malco.exe"
)

$ErrorActionPreference = 'Stop'

$success = $true

function Run-Test($test) {
    Write-Output "[tests] Compiling test $($test.Name)..."
    & $malco -c $test.FullName
    echo $LASTEXITCODE
    if ($LASTEXITCODE -eq 0) {
        Write-Output "[tests] Test $($test.Name) success"
    } else {
        Write-Output "[tests] Test $($test.Name) error"
        $script:success = $false
    }
}

Set-Location $Root
Get-ChildItem $TestScripts/*.rasm | ForEach-Object { Run-Test $_ }

if (!$success) {
    exit 1
}
