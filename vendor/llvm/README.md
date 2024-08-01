# LLVM 

This folder contains the custom scripts to operate the LLVM compilation operations.

## compile.ps1

This script is responsible for compiling *.c files into *.o files.

```
usage:

compile.ps1 -Location "#" -Target "engine" -CFlags "##" -IFlags "##" -DFlags "##"
```

## archive.ps1

This script is responsible for bundleing *.o files into a single *.lib file

```
usage:

archive.ps1 -Location "#" -Target "engine" -Output "#" 
``` 

## link.ps1

This script is responsible for generating the final executable

```
usage:

link.ps1 -Location "##" -Target "##" -LFlags "##" -Output "##" 
```
