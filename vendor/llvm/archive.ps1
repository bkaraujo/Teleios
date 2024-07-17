param (
    [string]$Target="",
    [string]$Location="",
    [string]$Output=""
) 

$BUILDFS = "$ROOTFS/build"
$TARGETFS = "$BUILDFS/$Target"

# ##############################################################################
# script initialization
# Caches initial location
# ##############################################################################
Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Archive $TARGETFS into $Output"
$ROOTFS = Get-Location
# ##############################################################################
# Creates build folder if it not exists
# Set location to build folder
# ##############################################################################
if (Test-Path -Path "$TARGETFS" ){}
else { New-Item -ItemType Directory "$TARGETFS" -Force | Out-Null }

Set-Location "$TARGETFS"
# ##############################################################################
# Delete the previous archive
# Invoke the archive command
# ##############################################################################
if (Test-Path -Path "$BUILDFS/$Output" ){
    Remove-Item -Path "$BUILDFS/$Output" -Force | Out-Null
}

Invoke-Expression "llvm-ar rc $BUILDFS/$Output $(Get-ChildItem -Path "$TARGETFS" -Filter "*.o" -Recurse -File)"
# ##############################################################################
# Restores the original location
# ##############################################################################
Set-Location $ROOTFS