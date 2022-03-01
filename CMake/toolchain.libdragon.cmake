option(TOOLCHAIN_LIBDRAGON "Use LIBDRAGON" ON) #only set if this file is called

# variables
# TOOLCHAIN_PREFIX - directory containing target gcc compiler and binutils
# LIBDRAGON_PREFIX - directory containing libdragon
# LIBDRAGONTOOLS_PREFIX - directory containing libdragon tools

# Arch Linux
set(LIBDRAGONTOOLS_PREFIX ${LIBDRAGON_PREFIX})

include_directories(
	${LIBDRAGON_PREFIX}/include
)

link_directories(
	${LIBDRAGON_PREFIX}/lib
)

link_libraries(
	dragon
)

# set the necessary tools we need for building the rom
set(N64_TOOL	       	${LIBDRAGONTOOLS_PREFIX}/n64tool)
set(CHECKSUM_TOOL       ${LIBDRAGONTOOLS_PREFIX}/chksum64)

set(LINKER_FLAGS_START		"-ldragon")
set(LINKER_FLAGS_END		"-ldragonsys")

include(${CMAKE_CURRENT_LIST_DIR}/toolchain.mips64-elf.cmake)
