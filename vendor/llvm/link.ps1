param (
    [string]$Location="",
    [string]$Output="", 
    [string]$LFlags="", 
    [string]$Files=""
) 

Write-Host "Linking $Output"
$ROOTFS = Get-Location

Set-Location $Location
Invoke-Expression "clang -g $LFlags $(Get-ChildItem -Path "$Location" -Filter "*.o" -Recurse -File) -o $Output"
foreach ($file in $Files.Split(" ")) { Remove-Item -Path $file }

Set-Location $ROOTFS