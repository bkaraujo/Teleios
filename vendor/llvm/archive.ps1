param (
    [string]$Location="",
    [string]$Output="", 
    [string]$Files=""
) 
Write-Host "Creating arquive $Output"
$ROOTFS = Get-Location

Set-Location $Location
Invoke-Expression "llvm-ar rc $Output $Files"

Set-Location $ROOTFS