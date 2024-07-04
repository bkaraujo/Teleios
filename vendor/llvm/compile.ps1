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
foreach ($file in Get-ChildItem -Path $Location -Filter "*.c" -Recurse -File) {
    $Hash = $(Get-FileHash -Path $file -Algorithm SHA1).Hash
    $ShaFile = "$ROOTFS/build/$Target/$($file.BaseName).sha1"

    if (Test-Path -Path $ShaFile) {} 
    else { New-Item -ItemType File -Path $ShaFile | Out-Null }

    if ($Hash -ne $(Get-Content $ShaFile | Select-Object -First 1)) {
        $Hash > $ShaFile
        Write-Host "Compiling $file"
        Invoke-Expression "clang -std=c11 -Wall -Werror -march=x86-64 $CFlags $IFlags $DFlags -c $file"
    }
}

# ##############################################################################
# Restores the original location
# ##############################################################################
Set-Location $ROOTFS
