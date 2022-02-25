# cmake-demo-rom
Generates a ROM built with CMake

## Arch Linux

### mips64

Note: Base toolchain, this does not include any sdk

run `cmake -B build/mips64-elf --toolchain CMake/toolchain.mips64-elf.cmake -DTOOLCHAIN_PREFIX=/usr/mips64-elf`

### mips64-libdragon

Note: libdragon uses a custom linker

run `cmake -B build/mips64-libdragon --toolchain CMake/toolchain.libdragon.cmake -DTOOLCHAIN_PREFIX=/usr/mips64-elf -DLIBDRAGON_PREFIX=/usr/bin`

### mips64-n64chain (exerimental)

run `cmake -B build/mips64-n64chain --toolchain CMake/toolchain.n64chain.cmake -DTOOLCHAIN_PREFIX=/usr/mips64-elf -N64CHAIN_PREFIX=/usr/mips64-elf`

### mips64-libultra (experimental)

run `cmake -B build/mips64-libultra --toolchain CMake/toolchain.libultra.cmake -DTOOLCHAIN_PREFIX=/usr/mips64-elf -LIBULTRA_PREFIX=/usr/mips64-elf`

## Windows

Requirements:
* Turning on Windows 10 developer mode Or Windows 7 with latest PowerShell version installed
* Enabling Remote Powershell execution policy (`Set-ExecutionPolicy Unrestricted`)
* https://cmake.org/download/ (tested working with 3.18.4) to be installed (make sure the PATH variable is set).
* VSCode with `CMake`, `CMake Tools` and `C/C++` extensions
![Required VSCode Extensions](vscode-extensions.png)

* Run `UpdateToolchain.ps1` to download the required toolchain and libdragon files
* Adjust .vscode/cmake-variants.json for your paths if necessary (although should work automatically out the box)
* Adjust .vscode/settings.json for your paths (although should work automatically out the box)

Building the ROM:
In VS-Code
* Click on the Bottom bar to set the "build variant": ![CMake variant](vscode-set-variant.png)
* Use any kit e.g. "Unspecified" (as this is handled by Ninja-Build)
* Hit F7 to build the ROM
