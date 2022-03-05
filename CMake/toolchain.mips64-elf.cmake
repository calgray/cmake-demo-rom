include(CMakeForceCompiler)

# set toolchain directories
set(TOOLCHAIN_BIN_DIR ${TOOLCHAIN_PREFIX}/bin)

# the name of the operating system for which CMake is to build

# WARNING: setting this seems to default to windows .obj output extensions
# set(CMAKE_SYSTEM_NAME nintendo64)

# name of the CPU CMake is building for
set(CMAKE_SYSTEM_PROCESSOR mips64)

# set the language standard (gnu-99)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# macro to setup compilers
macro(SET_COMPILER_VAR var name)
   find_program(CMAKE_${var} mips64-elf-${name} HINTS ${TOOLCHAIN_BIN_DIR} DOC "${name} tool")
endmacro()

# setup C preprocessor
if(NOT CMAKE_C_PREPROCESSOR)
    SET_COMPILER_VAR(C_PREPROCESSOR cpp)
endif()

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

# include_directories(
# 	${TOOLCHAIN_PREFIX}/include
# 	${TOOLCHAIN_PREFIX}/mips64-elf/include
# )

# link_directories(
# 	${TOOLCHAIN_PREFIX}/lib
# 	${TOOLCHAIN_PREFIX}/mips64-elf/lib
# )

# link_libraries(
# 	libc.a
# 	libm.a
# )

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

# this makes the test compiles use static library option so that we don't need to pre-set linker flags and scripts
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# use `mips64-elf-gcc --help=target`

# set the flags for an N64's architecture
set(MCPU_FLAGS "-march=vr4300 -mtune=vr4300 -mabi=32 -ffreestanding -mfix4300")

set(CMAKE_C_CXX_FLAGS "-G0 ${MCPU_FLAGS} -mno-long-calls -Wall -Wno-pointer-sign")
set(CMAKE_C_CXX_FLAGS_DEBUG   "-O0 -g -ggdb3") #add option -ggdb3 

set(CMAKE_ASM_FLAGS "${CMAKE_C_CXX_FLAGS} -x assembler-with-cpp" CACHE INTERNAL "asm compiler flags")
#set(CMAKE_ASM_FLAGS_DEBUG   "" )
#set(CMAKE_ASM_FLAGS_RELEASE "" )

set(CMAKE_C_FLAGS "${CMAKE_C_CXX_FLAGS}" CACHE INTERNAL "c compiler flags")
set(CMAKE_C_FLAGS_DEBUG   "${CMAKE_C_CXX_FLAGS_DEBUG}" )

set(CMAKE_CXX_FLAGS "${CMAKE_C_CXX_FLAGS}" CACHE INTERNAL "cxx compiler flags")
set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_C_CXX_FLAGS_DEBUG}" )

#set(CMAKE_EXE_LINKER_FLAGS "-G0 ${LINKER_FLAGS_START} -lc -lm ${LINKER_FLAGS_END}" CACHE INTERNAL "exe link flags")
#set(CMAKE_EXE_LINKER_FLAGS "-G0 ${LINKER_FLAGS_START} ${LINKER_FLAGS_END}" CACHE INTERNAL "exe link flags")
set(CMAKE_EXE_LINKER_FLAGS "${LINKER_FLAGS_START} ${LINKER_FLAGS_END}" CACHE INTERNAL "exe link flags")