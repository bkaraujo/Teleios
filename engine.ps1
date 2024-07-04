Clear-Host
$ROOTFS = $(Get-Location)

.\vendor\llvm\compile.ps1 -Target "engine" -Location "$ROOTFS/engine/src" -CFlags "-g -Wno-unused-but-set-variable" -IFlags "-I$ROOTFS/engine/src" -DFlags "-DCGLM_STATIC -DTL_BUILD_ALPHA -DTL_EXPORT -DTL_TARGET_WINDOWS -D_CRT_SECURE_NO_WARNINGS"
if( $LastExitCode -ne 0) { Set-Location $ROOTFS; return 0; }

.\vendor\llvm\compile.ps1 -Target "sandbox" -Location "$ROOTFS/sandbox/src" -CFlags "-g" -IFlags "-I$ROOTFS/engine/src -I$ROOTFS/sandbox/src" -DFlags "-DTL_BUILD_ALPHA -DTL_TARGET_WINDOWS -D_CRT_SECURE_NO_WARNINGS"
if( $LastExitCode -ne 0) { Set-Location $ROOTFS; return 0; }

.\vendor\llvm\archive.ps1 -Target "engine" -Location "$ROOTFS" -Output "teleios.lib" 
if( $LastExitCode -ne 0) { Set-Location $ROOTFS; return 0; }

.\vendor\llvm\link.ps1  -Target "sandbox" -Location "$ROOTFS" -Output "sandbox.exe" -LFlags "-luser32 -lgdi32 -lopengl32 -l$ROOTFS/build/teleios.lib"
if( $LastExitCode -ne 0) { Set-Location $ROOTFS; return 0; }

Set-Location $ROOTFS
Write-Host "Successfuly Completed"