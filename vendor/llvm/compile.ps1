param (
    [string]$Target="",
    [string]$Location="",
    [string]$CFlags="", 
    [string]$IFlags="", 
    [string]$DFlags=""
) 

$BUILDFS = "$ROOTFS\build"
$TARGETFS = "$BUILDFS\$Target"

# ##############################################################################
# script initialization
# Caches initial location
# ##############################################################################
Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Compiling $Location"
$ROOTFS = Get-Location
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
    $FDir = "$TARGETFS/$folder"
    Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")]   Compiling $FDir"
    
    if (Test-Path -Path "$FDir" ){}
    else { New-Item -ItemType Directory "$FDir" -Force | Out-Null }
    
    Set-Location "$FDir"
    $(Get-ChildItem -Path "$Location/$folder" -Filter "*.c" -Recurse -File) | Foreach-Object -ThrottleLimit 8 -Parallel {
        $Hash = $(Get-FileHash -Path "$PSItem" -Algorithm SHA1).Hash
        $ShaFile = "$USING:FDir/$($PSItem.BaseName).sha1"

        # Create the shafile for the first execution
        if (Test-Path -Path $ShaFile) {} 
        else { New-Item -ItemType File -Path $ShaFile | Out-Null }

        if ($Hash -ne $(Get-Content $ShaFile | Select-Object -First 1)) { 
            $Hash > $ShaFile

            "clang -std=c11 -Wall -Werror -march=x86-64 $USING:CFlags $USING:IFlags $USING:DFlags -c $PSItem" > "$FDir/$($PSItem.BaseName).cmd"
            # Perform the compilation and store the result code
            $global:LastExitCode = 0;
            Invoke-Expression -Command "clang -std=c11 -Wall -Werror -march=x86-64 $USING:CFlags $USING:IFlags $USING:DFlags -c $PSItem"
            if ($LastExitCode -ne 0) {
                Remove-Item -Path $ShaFile | Out-Null
                return 1
            }
        }
    }
}
# ##############################################################################
# Restores the original location
# ##############################################################################
Set-Location $ROOTFS
