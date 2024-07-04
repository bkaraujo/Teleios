param (
    [string]$Target="",
    [string]$Location="",
    [string]$CFlags="", 
    [string]$IFlags="", 
    [string]$DFlags=""
) 
# ##############################################################################
# script initialization
# Caches initial location
# ##############################################################################
Write-Host "Compiling $Location"
$ROOTFS = Get-Location
# ##############################################################################
# Creates build folder if it not exists
# Set location to build folder
# ##############################################################################
if (Test-Path -Path "$ROOTFS/build/$Target" ){}
else { New-Item -ItemType Directory "$ROOTFS/build/$Target" -Force | Out-Null }
Set-Location "$ROOTFS/build/$Target"
# ##############################################################################
# Blob the files that will be compiled
# Invoke the compiler
# ##############################################################################
$Files = $(Get-ChildItem -Path $Location -Filter "*.c" -Recurse -File)
Invoke-Expression "clang -std=c11 -Wall -Werror -march=x86-64 $CFlags $IFlags $DFlags -c $Files"
# ##############################################################################
# Restores the original location
# ##############################################################################
Set-Location $ROOTFS
