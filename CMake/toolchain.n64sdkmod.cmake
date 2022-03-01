option(TOOLCHAIN_N64SDKMOD "Use N64SDKMOD" ON)

# variables
# TOOLCHAIN_PREFIX - directory containing target gcc compiler and binutils
# N64SDKMOD_PREFIX - directory containing n64sdkmod

include_directories(
    ${N64SDKMOD_PREFIX}/include
)

link_directories(
    ${N64SDKMOD_PREFIX}/lib/n64
)

include(${CMAKE_CURRENT_LIST_DIR}/toolchain.mips64-elf.cmake)
