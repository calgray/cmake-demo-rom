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

set(ROM_HEADER ${TOOLCHAIN_PREFIX}/lib/header)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMake)
message(${CMAKE_PROJECT_DIR}/CMake)

#if the toolchain is different, we need to handle it here, or push it to a module, or even the toolchain?!
if(TOOLCHAIN_LIBDRAGON)
    # libdragon toolchain does not compule ultra and nusys

    # find libdragon
    find_package(libdragon REQUIRED)

    # find libreultra - doesn't build with libdragon due to conflicting sysytem libs
    # add_subdirectory(external/libreultra)

    # find fatfs
    add_subdirectory(external/fatfs)

    # build dragon projects
    add_subdirectory(demos/dragon-demo)

elseif(TOOLCHAIN_N64SDKMOD)
    # n64sdkmod provides ultra and nusys with few modification

    find_package(n64sdkmod REQUIRED)

    # build ultra and nusys projects
    add_subdirectory(demos/nusys-demos/nu0)

elseif(TOOLCHAIN_N64CHAIN)
    add_custom_command(TARGET ${ELF_NAME}
        POST_BUILD
        COMMAND ${CHECKSUM_TOOL} ARGS ${ROM_HEADER}
    )
else() #add some other compilation flags
#show error?
endif(TOOLCHAIN_LIBDRAGON)
unset(TOOLCHAIN_LIBDRAGON CACHE) # <---- this is the important to make sure the next build succeeds!!
