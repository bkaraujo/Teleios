Clear-Host
$ROOTFS = $(Get-Location)
function Invoke-LLVM-Compile {
    param (
        [string]$Location="",
        [string]$CFlags="", 
        [string]$IFlags="", 
        [string]$DFlags=""
    ) 

    $COMPILEFS = "$ROOTFS/$Location"
    Write-Host "Compiling $COMPILEFS"
    Set-Location $COMPILEFS
        
    Get-ChildItem -Path $COMPILEFS -Filter "*.o" -Recurse -File | Remove-Item
    $FILES = $(Get-ChildItem -Path $COMPILEFS -Filter "*.c" -Recurse -File)
    Invoke-Expression "clang -std=c11 -Wall -Werror -march=x86-64 $CFlags $Iflags $DFlags -c $FILES"
    
    Set-Location $ROOTFS
}

function Invoke-LLVM-Arquive {
    param (
        [string]$Location="",
        [string]$Output="", 
        [string]$Files=""
    ) 
    Write-Host "Creating arquive $Output"

    Set-Location $Location
    Invoke-Expression "llvm-ar rc $Output $Files"
    
    Set-Location $ROOTFS
}

function Invoke-LLVM-Link {
    param (
        [string]$Location="",
        [string]$Output="", 
        [string]$LFlags="", 
        [string]$Files=""
    ) 

    Write-Host "Linking $Output"

    Set-Location $Location
    Invoke-Expression "clang -g $LFlags $(Get-ChildItem -Path "$Location" -Filter "*.o" -Recurse -File) -o $Output"
    
    Set-Location $ROOTFS
}

Invoke-LLVM-Compile -Location "engine/src/cglm" -Cflags "-g" -IFlags "-I$ROOTFS/engine/src" -DFlags "-DCGLM_STATIC"
Invoke-LLVM-Compile -Location "engine/src/glad" -Cflags "-g" -IFlags "-I$ROOTFS/engine/src"
Invoke-LLVM-Compile -Location "engine/src/teleios" -CFlags "-g -Wno-unused-but-set-variable" -IFlags "-I$ROOTFS/engine/src" -DFlags "-DTL_BUILD_ALPHA -DTL_EXPORT -DTL_TARGET_WINDOWS -D_CRT_SECURE_NO_WARNINGS"
Invoke-LLVM-Compile -Location "sandbox/src" -CFlags "-g" -IFlags "-I$ROOTFS/engine/src -I$ROOTFS/sandbox/src" -DFlags "-DTL_BUILD_ALPHA -DTL_TARGET_WINDOWS -D_CRT_SECURE_NO_WARNINGS"

Invoke-LLVM-Arquive -Location "$ROOTFS" -Output "teleios.lib" -Files "$(Get-ChildItem -Path "engine/src/cglm" -Filter "*.o" -Recurse -File) $(Get-ChildItem -Path "engine/src/glad" -Filter "*.o" -Recurse -File) $(Get-ChildItem -Path "engine/src/teleios" -Filter "*.o" -Recurse -File)"

Invoke-LLVM-Link -Location "$ROOTFS" -Output "sandbox.exe" -LFlags "-luser32 -lgdi32 -lopengl32 -l$ROOTFS/teleios.lib" -Files "$(Get-ChildItem -Path $SANDBOXFS -Filter "*.o" -Recurse -File)"
