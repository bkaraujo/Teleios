param (
    [string]$Target="",
    [string]$Location="",
    [string]$Output="", 
    [string]$LFlags=""
) 

$BUILDFS = "$ROOTFS\build"
$TARGETFS = "$BUILDFS\$Target"

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
if (Test-Path -Path "$TARGETFS" ){}
else { Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Link source folder $TARGETFS not found" ; return 1 }

Set-Location "$TARGETFS"
# ##############################################################################
# Invoke the compiler
# Remove all files that have been archived
# ##############################################################################
if (Test-Path -Path "$BUILDFS/$Output" ){
    Remove-Item -Path $BUILDFS/$Output -Force | Out-Null
}

Invoke-Expression "clang -g $LFlags $(Get-ChildItem -Path "$TARGETFS" -Filter "*.o" -Recurse -File) -o $BUILDFS/$Output"
# ##############################################################################
# Restores the original location
# ##############################################################################
Set-Location $ROOTFS