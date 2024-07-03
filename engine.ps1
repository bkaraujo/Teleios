Clear-Host
$ROOTFS = $(Get-Location)
# ==========================================================================================
#
#                                           CGLM
#
# ==========================================================================================
Write-Host "======= CGLM ======="
$CGLMFS = "$ROOTFS/engine/src/cglm"
Set-Location $CGLMFS

$CFlags = "-std=c11 -g -Wall -Werror -march=x86-64"
$IFlags = "-I$ROOTFS/engine/src"
$DFlags = "-DCGLM_STATIC"

Get-ChildItem -Path $CGLMFS -Filter "*.o" -Recurse -File | Remove-Item
$FILES = $(Get-ChildItem -Path $CGLMFS -Filter "*.c" -Recurse -File)
Write-Host "clang $CFlags $IFlags $DFlags -c $FILES"
Invoke-Expression "clang $CFlags $IFlags $DFlags -c $FILES"
# ==========================================================================================
#
#                                           GLAD
#
# ==========================================================================================
Write-Host "======= GLAD ======="
$GLADFS = "$ROOTFS/engine/src/glad"
Set-Location $GLADFS

$CFlags = "-std=c11 -g -Wall -Werror -march=x86-64"
$IFlags = "-I$ROOTFS/engine/src"
$DFlags = ""

Get-ChildItem -Path $GLADFS -Filter "*.o" -Recurse -File | Remove-Item
$FILES = $(Get-ChildItem -Path $GLADFS -Filter "*.c" -Recurse -File)

Write-Host "clang $CFlags $IFlags $DFlags -c $FILES"
Invoke-Expression "clang $CFlags $IFlags $DFlags -c $FILES"
# ==========================================================================================
#
#                                           TELEIOS
#
# ==========================================================================================
Write-Host "======= TELEIOS ======="
$TELEIOSFS = "$ROOTFS/engine/src/teleios"
Set-Location $TELEIOSFS

$CFlags = "-std=c11 -g -Wall -Werror -march=x86-64 -Wno-unused-but-set-variable -fcomplete-member-pointers"
$IFlags = "-I$ROOTFS/engine/src"
$DFlags = "-DTL_BUILD_ALPHA -DTL_EXPORT -DTL_TARGET_WINDOWS -D_CRT_SECURE_NO_WARNINGS"

Get-ChildItem -Path $TELEIOSFS -Filter "*.o" -Recurse -File | Remove-Item
$FILES = $(Get-ChildItem -Path $TELEIOSFS -Filter "*.c" -Recurse -File)

Write-Host "clang $CFlags $IFlags $DFlags -c $FILES"
Invoke-Expression "clang $CFlags $IFlags $DFlags -c $FILES"

Set-Location $ROOTFS

Write-Host "======= llvm-ar rc engine.lib ======="
Write-Host "llvm-ar rc teleios.lib $(Get-ChildItem -Path $TELEIOSFS -Filter "*.o" -Recurse -File) $(Get-ChildItem -Path $GLADFS -Filter "*.o" -Recurse -File)"
Invoke-Expression "llvm-ar rc teleios.lib $(Get-ChildItem -Path $TELEIOSFS -Filter "*.o" -Recurse -File) $(Get-ChildItem -Path $GLADFS -Filter "*.o" -Recurse -File) $(Get-ChildItem -Path $CGLMFS -Filter "*.o" -Recurse -File)"

# ==========================================================================================
#
#                                           SANDOBX
#
# ==========================================================================================
Write-Host "======= SANDBOX ======="
$SANDBOXFS = "$ROOTFS/sandbox/src"
Set-Location $SANDBOXFS

$CFlags = "-std=c11 -g -Wall -Werror -march=x86-64"
$IFlags = "-I$ROOTFS/engine/src -I$ROOTFS/sendbox/src"
$LFlags = "-luser32 -lgdi32 -lopengl32 -l$ROOTFS/teleios.lib"
$DFlags = "-DTL_BUILD_ALPHA -DTL_TARGET_WINDOWS -D_CRT_SECURE_NO_WARNINGS"

Get-ChildItem -Path $SANDBOXFS -Filter "*.o" -Recurse -File | Remove-Item
$FILES = $(Get-ChildItem -Path $SANDBOXFS -Filter "*.c" -Recurse -File)

Write-Host "clang $CFlags $IFlags $DFlags -c $FILES"
Invoke-Expression "clang $CFlags $IFlags $DFlags -c $FILES"

Set-Location $ROOTFS

Write-Host "======= clang -o sandbox.exe ======="
Write-Host "clang -g $LFlags $(Get-ChildItem -Path $SANDBOXFS -Filter "*.o" -Recurse -File) -o sandbox.exe"
Invoke-Expression "clang -g $LFlags $(Get-ChildItem -Path $SANDBOXFS -Filter "*.o" -Recurse -File) -o sandbox.exe"

Get-ChildItem -Path $CGLMFS -Filter "*.o" -Recurse -File | Remove-Item
Get-ChildItem -Path $GLADFS -Filter "*.o" -Recurse -File | Remove-Item
Get-ChildItem -Path $TELEIOSFS -Filter "*.o" -Recurse -File | Remove-Item
Get-ChildItem -Path $SANDBOXFS -Filter "*.o" -Recurse -File | Remove-Item
