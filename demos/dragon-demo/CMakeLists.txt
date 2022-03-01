
set(TARGET_NAME DRAGON-DEMO)

# build ROM
set( ELF_NAME             ${TARGET_NAME}.elf )
set( BIN_NAME             ${TARGET_NAME}.bin )
set( ROM_NAME             ${TARGET_NAME}.v64 )

add_executable(${ELF_NAME})

target_sources(${ELF_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/src/main.cpp
    ${CMAKE_CURRENT_LIST_DIR}/src/libdragonwrapper.cpp
)

target_include_directories(${ELF_NAME}
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
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

target_compile_definitions(${ELF_NAME} PUBLIC
    ROM_VER_MAJOR=${BUILD_VER_MAJOR}
    ROM_VER_MINOR=${BUILD_VER_MINOR}
    ROM_VER_REV=${BUILD_VER_REV}
    FLASHCART_TARGET_TYPE=${HARDWARE_TYPE}
    FLASHCART_TARGET_REVISION=${HARDWARE_REVISION}
)

#probably a better way to do this, but if you need to build for something that uses different compiler flags...
#SET_SOURCE_FILES_PROPERTIES(${CMAKE_CURRENT_LIST_DIR}/src/gscore.c PROPERTIES COMPILE_FLAGS -O0) 


target_link_libraries(${ELF_NAME}
    PRIVATE libdragon
    PRIVATE fatfs
)


# extra libdragon rom packaging steps
add_custom_command(TARGET ${ELF_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} ARGS -O binary ${ELF_NAME} ${BIN_NAME}
)

add_custom_command(TARGET ${ELF_NAME}
    POST_BUILD # Make sure your ROM size is big enough... perhaps we could use "size" on the bin to check?
    COMMAND ${N64_TOOL} ARGS -l 1M -t "${TARGET_NAME}" -h ${ROM_HEADER} -o ${ROM_NAME} ${BIN_NAME}
)
add_custom_command(TARGET ${ELF_NAME}
    POST_BUILD
    COMMAND ${CHECKSUM_TOOL} ARGS ${ROM_NAME} && ls -lh ${TARGET_NAME}*
)