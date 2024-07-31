param (
    [string]$Target="",
    [string]$Location="",
    [string]$CFlags="", 
    [string]$IFlags="", 
    [string]$DFlags=""
) 
# ##############################################################################
# Current script globals
# ##############################################################################
$global:success=0
# ##############################################################################
# script initialization
# Caches initial location
# ##############################################################################
Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Compiling $Location"
$ROOTFS = Get-Location
$BUILDFS = "$ROOTFS\build"
$TARGETFS = "$BUILDFS\$Target"
# ##############################################################################
# Creates build folder if it not exists
# Set location to build folder
# ##############################################################################
if (Test-Path -Path "$TARGETFS" ){}
else { New-Item -ItemType Directory "$TARGETFS" -Force | Out-Null }
Set-Location "$TARGETFS"
# ##############################################################################
# Blob the files that will be compiled
# Invoke the compiler
# ##############################################################################
foreach ($folder in Get-ChildItem $Location | Where-Object {$_.PSIsContainer} | Foreach-Object {$_.Name}) {
    $TargetFolder = "$TARGETFS/$folder"
    Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")]   Compiling $TargetFolder"
    
    if (Test-Path -Path "$TargetFolder" ){}
    else { New-Item -ItemType Directory "$TargetFolder" -Force | Out-Null }
    
    Set-Location "$TargetFolder"
    $(Get-ChildItem -Path "$Location/$folder" -Filter "*.c" -Recurse -File) | Foreach-Object -ThrottleLimit 8 -Parallel {
        # ===============================================
        # Calculate the current file hash
        # ===============================================
        $Hash = $(Get-FileHash -Path "$PSItem" -Algorithm SHA1).Hash
        # ===============================================
        # Create the shafile if it dont exists
        # ===============================================
        $ShaFile = "$USING:TargetFolder/$($PSItem.BaseName).sha1"
        if (Test-Path -Path $ShaFile) {} 
        else { New-Item -ItemType File -Path $ShaFile | Out-Null }
        # ===============================================
        # If the hash changed compile the file
        # ===============================================
        if ($Hash -ne $(Get-Content $ShaFile | Select-Object -First 1)) { 
            $Hash > $ShaFile
            Write-Host "clang -std=c11 -Wall -Werror -march=x86-64 $USING:CFlags $USING:IFlags $USING:DFlags -c $PSItem"
            Set-Content -Path "$USING:TargetFolder/$($PSItem.BaseName).cmd" -Value "clang -std=c11 -Wall -Werror -march=x86-64 $USING:CFlags $USING:IFlags $USING:DFlags -c $PSItem"

            $global:LastExitCode = 0;
            Invoke-Expression -Command "clang -std=c11 -Wall -Werror -march=x86-64 $USING:CFlags $USING:IFlags $USING:DFlags -c $PSItem"
            if ($LastExitCode -ne 0) {
                Remove-Item -Path $ShaFile | Out-Null
                $global:success = 1
                Break
            }
        }
    }
}
# ##############################################################################
# Restores the original location
# ##############################################################################
Set-Location $ROOTFS
return $global:success