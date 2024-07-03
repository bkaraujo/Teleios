param (
    [string]$Location="",
    [string]$CFlags="", 
    [string]$IFlags="", 
    [string]$DFlags=""
) 

Write-Host "Compiling $Location"
$ROOTFS = Get-Location

Set-Location $Location

Get-ChildItem -Path $Location -Filter "*.o" -Recurse -File | Remove-Item
$FILES = $(Get-ChildItem -Path $Location -Filter "*.c" -Recurse -File)
Invoke-Expression "clang -std=c11 -Wall -Werror -march=x86-64 $CFlags $Iflags $DFlags -c $FILES"

Set-Location $ROOTFS