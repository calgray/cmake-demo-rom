# cmake-demo-rom
Generates a ROM built with CMake

Read the following manual to better understand the hardware architecture: http://ultra64.ca/files/documentation/nintendo/Nintendo_64_Intro_to_the_N64.pdf

## Development Frameworks

### Official SDK (n64sdk)


comes with the ultra OS library and various nintendo provided libraries and tools for IRIX and WIN95.

#### ultra

An OS/library that provides hardware related headers. Still requires a fair bit of boilerplate to get a game engine running. See https://github.com/jsdf/n64-sdk-demo

#### nusys

NuSystem, a framework built on libultra designed to help newcomers understand the N64 achitecture. See https://github.com/jsdf/n64-sdk-demo

### Official Mods

The official sdk's are 20+ years old and target ancient operating systems thus require 'mods' of some kind to compile on newer systems and compilers. These projects are still based on the proprietary official sdk.

#### n64sdkmod (libultra)

crashoverride95's debian packages for officual sdk with gcc on linux.

https://crashoveride95.github.io/n64hbrew/modernsdk/index.html

n64sdk - latest 2.0L of the N64 SDK library
iquesdk - iQue SDK wich is modified version of 2.0L.

recommended packages are:
`n64sdk makemask n64-demos nusys-demos`

some demos require other ported n64sdk derived libraries

For readme see: https://crashoveride95.github.io/n64hbrew/modernsdk/specifics/pkglist.html

### Unofficial

#### n64chain (libn64 with libultra support)

GCC based development toolchain that does not depend on any proprietary code.

https://github.com/tj90241/n64chain
https://github.com/Mr-Pnut/portable-n64-toolchain

For adding official, use steps in: https://github.com/trhodeos/modern-n64sdk

#### libdragon

Custom gcc based os/library for homebrew with a simplified API. Provides libraries for flash cart features such as flash memory read/write.

#### glank (gl-n64)

Custom set of tools and c library resembling the ultra64 sdk.

## Toolchains

### mips64

Note: Base toolchain, this does not include any sdk

run `cmake -B build/mips64-elf --toolchain CMake/toolchain.mips64-elf.cmake -DTOOLCHAIN_PREFIX=/usr/mips64-elf`

### mips64-libdragon

libdragon is built on mips64-gcc and provides a high-level GPL homebrew API with modern c++ support.

Note: libdragon uses a custom linker

run `cmake -B build/mips64-libdragon --toolchain CMake/toolchain.libdragon.cmake -DTOOLCHAIN_PREFIX=/usr/mips64-elf -DLIBDRAGON_PREFIX=/usr/bin`

### mips64-n64sdkmod

Uses official libraries and unofficial tools from n64sdkmod

run `cmake -B build/mips64-n64sdkmod --toolchain CMake/toolchain.n64sdkmod.cmake -DTOOLCHAIN_PREFIX=/usr/mips64-elf -DN64SDKMOD_PREFIX=external/n64sdkmod -DCMAKE_ASM_OUTPUT_EXTENSION=".o" -DCMAKE_C_OUTPUT_EXTENSION=".o"`

### mips64-n64chain (experimental)

Uses n64toolchain with

run `cmake -B build/mips64-n64chain --toolchain CMake/toolchain.n64chain.cmake -DTOOLCHAIN_PREFIX=/usr/mips64-elf -N64CHAIN_PREFIX=/usr/mips64-elf`

### mips64-libultra (experimental)

Uses n64sdkmod

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
