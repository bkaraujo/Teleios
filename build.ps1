param (
    [string]$Build="XX"
) 

Clear-Host
Write-Host "Creating $Build Build"

$ROOTFS = $(Get-Location)

if ($Build -eq "ALPHA"){} 
elseif ($Build -eq "BETA") {}
elseif ($Build -eq "RELEASE") {}
else { Set-Location $ROOTFS; Write-Host "Unsupported build. Use one of [ALPHA, BETA, RELEASE]" ; return 0; }

# ####################################################################################################################
# Clear old build if build-profile changed
# ####################################################################################################################
if ( Test-Path -Path "$ROOTFS/build") {
    if ( Test-Path -Path "$ROOTFS/build/profile" ) {
        if ( $Build -ne $(Get-Content -Path "$ROOTFS/build/profile"  | Select-Object -First 1) ) {
            
            # Remove build directory
            Remove-Item -Path "$ROOTFS/build/" -Recurse -Force | Out-Null
            New-Item -ItemType Directory -Path "$ROOTFS/build/" | Out-Null

            # Remember current build profile
            $Build > "$ROOTFS/build/profile"
        }
    } else {
        Remove-Item -Path "$ROOTFS/build/" -Force | Out-Null
        New-Item -ItemType Directory -Path "$ROOTFS/build/" | Out-Null
    }
}

if ($Build -eq "ALPHA"){
    .\vendor\llvm\compile.ps1 -Target "engine" -Location "$ROOTFS/engine/src" -CFlags "-g -O0 -Wno-unused-but-set-variable" -IFlags "-I$ROOTFS/engine/src" -DFlags "-DCGLM_STATIC -DTL_BUILD_$Build -DTL_EXPORT -DTL_TARGET_WINDOWS -D_CRT_SECURE_NO_WARNINGS"
    if( $LastExitCode -ne 0) { Set-Location $ROOTFS; return 0; }
    
    .\vendor\llvm\compile.ps1 -Target "sandbox" -Location "$ROOTFS/sandbox/src" -CFlags "-g -O0" -IFlags "-I$ROOTFS/engine/src -I$ROOTFS/sandbox/src" -DFlags "-DTL_BUILD_$Build"
    if( $LastExitCode -ne 0) { Set-Location $ROOTFS; return 0; }
}

if ($Build -ne "BETA") {
    .\vendor\llvm\compile.ps1 -Target "engine" -Location "$ROOTFS/engine/src" -CFlags "-g -O2 -Wno-unused-but-set-variable" -IFlags "-I$ROOTFS/engine/src" -DFlags "-DCGLM_STATIC -DTL_BUILD_$Build -DTL_EXPORT -DTL_TARGET_WINDOWS -D_CRT_SECURE_NO_WARNINGS"
    if( $LastExitCode -ne 0) { Set-Location $ROOTFS; return 0; }
    
    .\vendor\llvm\compile.ps1 -Target "sandbox" -Location "$ROOTFS/sandbox/src" -CFlags "-g -O2" -IFlags "-I$ROOTFS/engine/src -I$ROOTFS/sandbox/src" -DFlags "-DTL_BUILD_$Build"
    if( $LastExitCode -ne 0) { Set-Location $ROOTFS; return 0; }
}

if ($Build -ne "RELEASE") {
    .\vendor\llvm\compile.ps1 -Target "engine" -Location "$ROOTFS/engine/src" -CFlags "-O3 -Wno-unused-but-set-variable" -IFlags "-I$ROOTFS/engine/src" -DFlags "-DCGLM_STATIC -DTL_BUILD_$Build -DTL_EXPORT -DTL_TARGET_WINDOWS -D_CRT_SECURE_NO_WARNINGS"
    if( $LastExitCode -ne 0) { Set-Location $ROOTFS; return 0; }
    
    .\vendor\llvm\compile.ps1 -Target "sandbox" -Location "$ROOTFS/sandbox/src" -CFlags "-O3" -IFlags "-I$ROOTFS/engine/src -I$ROOTFS/sandbox/src" -DFlags "-DTL_BUILD_$Build"
    if( $LastExitCode -ne 0) { Set-Location $ROOTFS; return 0; }
}

.\vendor\llvm\archive.ps1 -Target "engine" -Location "$ROOTFS" -Output "teleios.lib" 
if( $LastExitCode -ne 0) { Set-Location $ROOTFS; return 0; }

.\vendor\llvm\link.ps1  -Target "sandbox" -Location "$ROOTFS" -Output "sandbox.exe" -LFlags "-luser32 -lgdi32 -lopengl32 -l$ROOTFS/build/teleios.lib"
if( $LastExitCode -ne 0) { Set-Location $ROOTFS; return 0; }

Set-Location $ROOTFS
Write-Host "Successfuly Completed"