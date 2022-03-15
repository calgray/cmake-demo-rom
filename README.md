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

### Official SDK (Modded)

The official SDKs are 20+ years old and target ancient operating systems thus requires mods of some kind to compile on newer systems and compilers. These projects are based the proprietary official SDK.

#### n64sdkmod (libultra, libnusys)

crashoverride95's debian packages for officual sdk with gcc on linux.

https://crashoveride95.github.io/n64hbrew/modernsdk/index.html

n64sdk - latest 2.0l of the N64 SDK library
iquesdk - iQue SDK wich is modified version of 2.0L.

recommended packages are:
`n64sdk makemask n64-demos nusys-demos`

some demos require other ported n64sdk derived libraries

For ReadMe see: https://crashoveride95.github.io/n64hbrew/modernsdk/specifics/pkglist.html

### Unofficial SDKs

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

For better compatibility for each sdk has been configured with seperate toolchains that should be selected 

### mips64

Note: Base toolchain, this only specifies the latest mips64 gcc tools and does not include any sdk.

### mips64-n64sdkmod

Provides official libraries and tools from n64sdkmod adapted for configurable installation location.

Note: Tested only with RelWithDebInfo build configuration

run `cmake -B build/mips64-n64sdkmod --toolchain CMake/toolchain.n64sdkmod.cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DTOOLCHAIN_PREFIX=/usr/mips64-elf -DN64SDKMOD_PREFIX=external/n64sdkmod -DCMAKE_C_OUTPUT_EXTENSION_REPLACE=1 -DCMAKE_ASM_OUTPUT_EXTENSION_REPLACE=1`

### mips64-libdragon

libdragon is built on mips64-gcc and provides a high-level GPL homebrew API with modern c++ support.

Note: libdragon uses a custom linker

run `cmake -B build/mips64-libdragon --toolchain CMake/toolchain.libdragon.cmake -DTOOLCHAIN_PREFIX=/usr/mips64-elf -DLIBDRAGON_PREFIX=/usr/bin`

### mips64-n64chain (experimental)

Uses n64toolchain with

run `cmake -B build/mips64-n64chain --toolchain CMake/toolchain.n64chain.cmake -DTOOLCHAIN_PREFIX=/usr/mips64-elf -N64CHAIN_PREFIX=/usr/mips64-elf`

### mips64-libultra (experimental)

Uses n64sdkmod

run `cmake -B build/mips64-libultra --toolchain CMake/toolchain.libultra.cmake -DTOOLCHAIN_PREFIX=/usr/mips64-elf -LIBULTRA_PREFIX=/usr/mips64-elf`

## Tools

### Audio

See http://n64devkit.square7.ch/kantan/step2/3/3_4.htm


#### Sound Sample Tables (.aiff -> .table)

Lookup table containing location of sound data.

sm64:
`aiff_extract_codebook <in .aiff> ><out .table>`

#### Sound Samples (.aiff + .table -> .aifc)

Sound samples that contain a raw signal that can be played back at different sample rates.

`assemble_sound.py <in samples dir> <in sound bank dir> <out .ctl> <out .tbl> <inputs e.g. sample.aiff>`

Official:

`vadpcm_enc -c <in .aiff> <out .aifc>`


#### Sound Sequences (.s and .m64 -> .bin)

Sequences are commonly known as MIDI format. Type1 midi files must first be converted to Type0 using `midicvt`

Compress the midi file using `midicomp`

`assemble_sound.py --sequences $@ <sequences_header> <bank_sets> <sequences .json>`

#### Sequence Bank File (.sbk)

official:
`sbc`

#### Bank Files (.tbl, .ctl, .sym)

Output files to be passed to the linker:

.tbl - store for ADPCM-comppressed files
.ctl - stores control data for .tbl
.sym - stores symbol data for .tbl

`ic`

### Images

### (.png ->)

### Models

## Setup

## RSP

RSP blobs (namely F3D)

### Arch

install AUR in order: `mips64-elf-binutils mips64-elf-gcc-stage0 mips64-elf-gcc`
install: `cmake`

### Windows

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
