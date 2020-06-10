include(CMakeForceCompiler)

# set toolchain directories
set(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_PREFIX}/bin)
set(TOOLCHAIN_LIB_DIR ${TOOLCHAIN_PREFIX}/lib)

# the name of the operating system for which CMake is to build
set(CMAKE_SYSTEM_NAME Generic)

# name of the CPU CMake is building for
set(CMAKE_SYSTEM_PROCESSOR mips64)

# set the language standard (gnu-99)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# macro to setup compilers
macro(SET_COMPILER_VAR var name)
   find_program(CMAKE_${var} mips64-elf-${name} HINTS ${TOOLCHAIN_BIN_DIR} DOC "${name} tool")
endmacro()

# setup C compiler
if(NOT CMAKE_C_COMPILER)
    SET_COMPILER_VAR(C_COMPILER gcc)
endif()

# setup C++ compiler
if(NOT CMAKE_CXX_COMPILER)
    SET_COMPILER_VAR(CXX_COMPILER g++)
endif()

# setup Assembler compiler
if(NOT CMAKE_ASM_COMPILER)
    SET_COMPILER_VAR(ASM_COMPILER gcc)
    SET_COMPILER_VAR(ASM-ATT_COMPILER as)
endif()

# set the necessary tools we need for building the rom
set(N64_TOOL	       	${LIBDRAGON_PREFIX}/tools/n64tool) #TODO - Should be part of the libdragon cmake
set(CHECKSUM_TOOL       ${LIBDRAGON_PREFIX}/tools/chksum64) #TODO - Should be part of the libdragon cmake

include_directories( ${ELF_NAME}
	PUBLIC
	${LIBDRAGON_PREFIX}/include #TODO - Should be part of the libdragon cmake
	${TOOLCHAIN_PREFIX}/mips64-elf/include
	${TOOLCHAIN_PREFIX}/include
)

link_directories( ${ELF_NAME}
	PUBLIC
	${LIBDRAGON_PREFIX}/lib/ #TODO - Should be part of the libdragon cmake
	${TOOLCHAIN_LIB_DIR}
	${TOOLCHAIN_PREFIX}/mips64-elf/lib
)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# this makes the test compiles use static library option so that we don't need to pre-set linker flags and scripts
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# set the flags for an N64's architecture
set(MCPU_FLAGS "-march=vr4300 -mtune=vr4300")

set(CMAKE_C_CXX_FLAGS "${MCPU_FLAGS} -mno-long-calls -Wall -Wno-pointer-sign")
set(CMAKE_C_CXX_FLAGS_DEBUG   "-O0") # future: -Og -g -ggdb3
set(CMAKE_C_CXX_FLAGS_RELEASE "-O2") 

set(CMAKE_C_FLAGS "${CMAKE_C_CXX_FLAGS}" CACHE INTERNAL "c compiler flags")
set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_CXX_FLAGS_DEBUG}" )
set(CMAKE_C_FLAGS_RELEASE " ${CMAKE_C_CXX_FLAGS_RELEASE}" )

set(CMAKE_CXX_FLAGS "${CMAKE_C_CXX_FLAGS}" CACHE INTERNAL "cxx compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_C_CXX_FLAGS_DEBUG}" )
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_CXX_FLAGS_RELEASE}" )

set(CMAKE_ASM_FLAGS "${MCPU_FLAGS}" CACHE INTERNAL "asm compiler flags")
set(CMAKE_ASM_FLAGS_DEBUG   "" )
set(CMAKE_ASM_FLAGS_RELEASE "" )

set(CMAKE_EXE_LINKER_FLAGS "-G0 -ldragon -lc -lm -ldragonsys" CACHE INTERNAL "exe link flags")
