param (
    [string]$Target="",
    [string]$Location="",
    [string]$Output=""
) 
# ##############################################################################
# script initialization
# Caches initial location
# ##############################################################################
Write-Host "Creating arquive $Output"
$ROOTFS = Get-Location
# ##############################################################################
# Creates build folder if it not exists
# Set location to build folder
# ##############################################################################
if (Test-Path -Path "$ROOTFS/build/$Target" ){}
else { New-Item -ItemType Directory "$ROOTFS/build/$Target" -Force | Out-Null }
Set-Location "$ROOTFS/build/$Target"
# ##############################################################################
# Invoke the compiler
# Remove all files that have been archived
# ##############################################################################
Invoke-Expression "llvm-ar rc $ROOTFS/build/$Output *.o"
# ##############################################################################
# Restores the original location
# ##############################################################################
Set-Location $ROOTFS