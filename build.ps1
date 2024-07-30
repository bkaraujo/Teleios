param (
    [string]$Build="XX"
) 

# ##############################################################################
# Parameter Validations
# ##############################################################################
if ($Build -eq "ALPHA"){} 
elseif ($Build -eq "BETA") {}
elseif ($Build -eq "RELEASE") {}
else { Write-Host "Unsupported build. Use one of [ALPHA, BETA, RELEASE]" ; return 0; }

# ##############################################################################
# Environment Preparations
# ##############################################################################
Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Creating $Build Build"

$ROOTFS = $(Get-Location)

if ( Test-Path -Path "$ROOTFS/build") {
    # Recreate the build dir if $ROOTFS/build/profile is diferent from parameter
    if ( $Build -ne $(Get-Content -Path "$ROOTFS/build/profile"  | Select-Object -First 1) ) {
        Remove-Item -Path "$ROOTFS/build/" -Recurse -Force | Out-Null
        New-Item -ItemType Directory -Path "$ROOTFS/build/" | Out-Null
        New-Item -ItemType File -Path "$ROOTFS/build/profile" | Out-Null
    }
} else {
    # Create the build dir if necessary
    New-Item -ItemType Directory -Path "$ROOTFS/build/" | Out-Null
    New-Item -ItemType File -Path "$ROOTFS/build/profile" | Out-Null
}
# Remember current build profile
$Build > "$ROOTFS/build/profile"

$EngineCFlags = ""
$SandboxCFlags = ""
# ##############################################################################
# Source Compilation
# ##############################################################################
if ($Build -eq "ALPHA"){ $EngineCFlags = "-g -O0 -Wno-unused-function" ; $SandboxCFlags = "-g -O0" }
if ($Build -eq "BETA") { $EngineCFlags = "-g -O2 -Wno-unused-function" ; $SandboxCFlags = "-g -O2" }
if ($Build -eq "RELEASE") { $EngineCFlags = "-O3 -Wno-unused-function" ; $SandboxCFlags = "-O3" }

$ExitCode = .\vendor\llvm\compile.ps1 -Target "engine" -Location "$ROOTFS/engine/src" -CFlags $EngineCFlags -IFlags "-I$ROOTFS/engine/src" -DFlags "-DCGLM_STATIC -DTELEIOS_BUILD_$Build -DTELEIOS_EXPORT -D_CRT_SECURE_NO_WARNINGS"
if( $ExitCode -ne 0) { Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Compilation Failed"; Set-Location $ROOTFS; exit $ExitCode; }

$ExitCode = .\vendor\llvm\compile.ps1 -Target "sandbox" -Location "$ROOTFS/sandbox/src" -CFlags $SandboxCFlags -IFlags "-I$ROOTFS/engine/src -I$ROOTFS/sandbox/src"
if( $ExitCode -ne 0) { Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Compilation Failed"; Set-Location $ROOTFS; exit $ExitCode; }
# ##############################################################################
# Static Lib creating
# ##############################################################################
$ExitCode = .\vendor\llvm\archive.ps1 -Target "engine" -Location "$ROOTFS" -Output "teleios" 
if( $ExitCode -ne 0) { Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Lib creation Failed"; Set-Location $ROOTFS; exit $ExitCode; }
# ##############################################################################
# Final Binary Linkage
# ##############################################################################
$LFags = "-luser32 -lgdi32 -lopengl32 -ladvapi32 -l$ROOTFS/engine/lib/OpenAL32.lib -l$ROOTFS/build/teleios.lib"

$ExitCode = .\vendor\llvm\link.ps1  -Target "sandbox" -Location "$ROOTFS" -Output "sandbox" -LFlags "$LFags"
if( $ExitCode -ne 0) { Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Linkag Failed"; Set-Location $ROOTFS; exit $ExitCode; }

Set-Location $ROOTFS
Write-Host "[$(Get-Date -Format "dd/MM/yyyy HH:mm K")] Successfuly Completed"