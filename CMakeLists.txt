cmake_minimum_required(VERSION 3.15)

include(CMakeToolsHelpers OPTIONAL)
include(ExternalProject)

set(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "ON" FORCE) # debug helper

# the following prevents launching a build in the source tree
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

message(STATUS "Building for hardware: ${HARDWARE_TYPE} with SDK: ${SDK_TYPE}")

# make sure the version is set
if(NOT BUILD_VERSION)
    set(BUILD_VERSION 0.0.1)
endif()

string(REPLACE "." ";" VERSION_LIST ${BUILD_VERSION})
list(GET VERSION_LIST 0 BUILD_VER_MAJOR)
list(GET VERSION_LIST 1 BUILD_VER_MINOR)
list(GET VERSION_LIST 2 BUILD_VER_REV)

project(BUILD-DEMO #rename for your ROM name
        VERSION ${BUILD_VERSION}
        LANGUAGES C CXX ASM)

# build ROM
set( ELF_NAME             ${PROJECT_NAME}.elf )
set( BIN_NAME             ${PROJECT_NAME}.bin )
set( ROM_NAME             ${PROJECT_NAME}.v64 )

set(ROM_HEADER ${TOOLCHAIN_PREFIX}/lib/header)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
message(${CMAKE_PROJECT_DIR}/CMake)

#if the toolchain is different, we need to handle it here, or push it to a module, or even the toolchain?!
if(TOOLCHAIN_LIBDRAGON)

    # find libdragon
    find_package(libdragon REQUIRED)

    # find libreultra - doesn't build with libdragon do to conflicting sysytem libs
    # add_subdirectory(external/libreultra)

    # find fatfs
    add_subdirectory(external/fatfs)

    add_executable( ${ELF_NAME})

    target_sources( ${ELF_NAME}
        PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
        ${CMAKE_CURRENT_LIST_DIR}/src/libdragonwrapper.cpp
    )

    # target_compile_options( ${ELF_NAME}
    #     PRIVATE -nostdinc
    # )

    # unset(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES)
    # unset(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)
    # include_directories(AFTER SYSTEM
    #     /usr/mips64-elf/include/c++/11.2.0
    #     /usr/mips64-elf/include/c++/11.2.0/mips64-elf
    #     /usr/mips64-elf/include/c++/11.2.0/backward
    #     /usr/lib/gcc/mips64-elf/11.2.0/include
    #     /usr/lib/gcc/mips64-elf/11.2.0/include-fixed
    #     /usr/mips64-elf/include
    #     /home/callan/Code/n64/cmake-demo-rom/external/libreultra/include/2.0I
    # )

    target_compile_definitions( ${ELF_NAME} PUBLIC
        ROM_VER_MAJOR=${BUILD_VER_MAJOR}
        ROM_VER_MINOR=${BUILD_VER_MINOR}
        ROM_VER_REV=${BUILD_VER_REV}
        FLASHCART_TARGET_TYPE=${HARDWARE_TYPE}
        FLASHCART_TARGET_REVISION=${HARDWARE_REVISION}
    )

    #probably a better way to do this, but if you need to build for something that uses different compiler flags...
    #SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_LIST_DIR}/src/gscore.c PROPERTIES COMPILE_FLAGS -O0) 

    target_include_directories( ${ELF_NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/include)

    target_link_libraries( ${ELF_NAME}
        PRIVATE libdragon
        PRIVATE fatfs
    )

    add_custom_command(TARGET ${ELF_NAME}
        POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${ELF_NAME} ${BIN_NAME}
    )

    add_custom_command(TARGET ${ELF_NAME}
        POST_BUILD # Make sure your ROM size is big enough... perhaps we could use "size" on the bin to check?
        COMMAND ${N64_TOOL} ARGS -l 1M -t "${PROJECT_NAME}" -h ${ROM_HEADER} -o ${ROM_NAME} ${BIN_NAME}
    )
    add_custom_command(TARGET ${ELF_NAME}
        POST_BUILD
        COMMAND ${CHECKSUM_TOOL} ARGS ${ROM_NAME} && ls -lh ${PROJECT_NAME}*
    )
elseif(TOOLCHAIN_N64SDKMOD)

elseif(TOOLCHAIN_N64CHAIN)
    add_custom_command(TARGET ${ELF_NAME}
        POST_BUILD
        COMMAND ${CHECKSUM_TOOL} ARGS ${ROM_HEADER}
    )
else() #add some other compilation flags
#show error?
endif(TOOLCHAIN_LIBDRAGON)
unset(TOOLCHAIN_LIBDRAGON CACHE) # <---- this is the important to make sure the next build succeeds!!
