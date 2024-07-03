param (
    [string]$Location="",
    [string]$CFlags="", 
    [string]$IFlags="", 
    [string]$DFlags=""
) 

Write-Host "Compiling $Location"
$ROOTFS = Get-Location

Set-Location $Location
Invoke-Expression "clang -std=c11 -Wall -Werror -march=x86-64 $CFlags $Iflags $DFlags -c $(Get-ChildItem -Path $Location -Filter "*.c" -Recurse -File)"
Set-Location $ROOTFS