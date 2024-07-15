param (
    [string]$Target="",
    [string]$Location="",
    [string]$Output=""
) 
# ##############################################################################
# script initialization
# Caches initial location
# ##############################################################################
Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Creating arquive $Output"
$ROOTFS = Get-Location
# ##############################################################################
# Creates build folder if it not exists
# Set location to build folder
# ##############################################################################
if (Test-Path -Path "$ROOTFS/build" ){}
else { New-Item -ItemType Directory "$ROOTFS/build" -Force | Out-Null }

Set-Location "$ROOTFS/build/$Target"
# ##############################################################################
# Delete the previous archive
# Invoke the archive command
# ##############################################################################
if (Test-Path -Path "$ROOTFS/build/$Output" ){
    Remove-Item -Path "$ROOTFS/build/$Output" -Force | Out-Null
}

Invoke-Expression "llvm-ar rc $ROOTFS/build/$Output *.o"
# ##############################################################################
# Restores the original location
# ##############################################################################
Set-Location $ROOTFS