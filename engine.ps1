Clear-Host
$ROOTFS = $(Get-Location)

.\vendor\llvm\compile.ps1 -Target "engine" -Location "$ROOTFS/engine/src/cglm" -Cflags "-g" -IFlags "-I$ROOTFS/engine/src" -DFlags "-DCGLM_STATIC"
.\vendor\llvm\compile.ps1 -Target "engine" -Location "$ROOTFS/engine/src/glad" -Cflags "-g" -IFlags "-I$ROOTFS/engine/src"
.\vendor\llvm\compile.ps1 -Target "engine" -Location "$ROOTFS/engine/src/teleios" -CFlags "-g -Wno-unused-but-set-variable" -IFlags "-I$ROOTFS/engine/src" -DFlags "-DTL_BUILD_ALPHA -DTL_EXPORT -DTL_TARGET_WINDOWS -D_CRT_SECURE_NO_WARNINGS"
.\vendor\llvm\archive.ps1 -Target "engine" -Location "$ROOTFS" -Output "teleios.lib" 

.\vendor\llvm\compile.ps1 -Target "sandbox" -Location "$ROOTFS/sandbox/src" -CFlags "-g" -IFlags "-I$ROOTFS/engine/src -I$ROOTFS/sandbox/src" -DFlags "-DTL_BUILD_ALPHA -DTL_TARGET_WINDOWS -D_CRT_SECURE_NO_WARNINGS"
.\vendor\llvm\link.ps1  -Target "sandbox" -Location "$ROOTFS" -Output "sandbox.exe" -LFlags "-luser32 -lgdi32 -lopengl32 -l$ROOTFS/build/teleios.lib"

Set-Location $ROOTFS
