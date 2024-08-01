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
if (Test-Path -Path "$TARGETFS" ) {} else { 
    New-Item -ItemType Directory "$TARGETFS" -Force 1>$null 2>$null 
}

Set-Location "$TARGETFS"
# ##############################################################################
# Delete the previous archive
# Invoke the archive command
# ##############################################################################
Remove-Item -Path "$BUILDFS/$Output.lib" -Force 1>$null 2>$null

$global:LastExitCode = 0;
Invoke-Expression "llvm-ar rc $BUILDFS/$Output.lib $(Get-ChildItem -Path "$TARGETFS" -Filter "*.o" -Recurse -File)"
if ($LastExitCode -ne 0) { return $LastExitCode }
# ##############################################################################
# Restores the original location
# ##############################################################################
Set-Location $ROOTFS

return 0