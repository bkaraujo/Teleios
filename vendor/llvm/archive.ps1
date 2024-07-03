param (
    [string]$Location="",
    [string]$Output="", 
    [string]$Files=""
) 
Write-Host "Creating arquive $Output"
$ROOTFS = Get-Location

Set-Location $Location
Invoke-Expression "llvm-ar rc $Output $Files"
foreach ($file in $Files.Split(" ")) { Remove-Item -Path $file }

Set-Location $ROOTFS