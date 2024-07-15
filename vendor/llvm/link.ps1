param (
    [string]$Target="",
    [string]$Location="",
    [string]$Output="", 
    [string]$LFlags=""
) 

# ##############################################################################
# script initialization
# Caches initial location
# ##############################################################################
Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Linking $Output"
$ROOTFS = Get-Location
# ##############################################################################
# Creates build folder if it not exists
# Set location to build folder
# ##############################################################################
if (Test-Path -Path "$ROOTFS/build" ){}
else { Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Link source folder $ROOTFS/build/$Target not found" ; return 1 }

Set-Location "$ROOTFS/build/$Target"
# ##############################################################################
# Invoke the compiler
# Remove all files that have been archived
# ##############################################################################
if (Test-Path -Path "$ROOTFS/build/$Output" ){
    Remove-Item -Path $ROOTFS/build/$Output -Force | Out-Null
}

Invoke-Expression "clang -g $LFlags *.o -o $ROOTFS/build/$Output"
# ##############################################################################
# Restores the original location
# ##############################################################################
Set-Location $ROOTFS