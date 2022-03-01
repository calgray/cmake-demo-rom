
set(TARGET_NAME NU0-DEMO)

# build ROM
set( ELF_NAME             ${TARGET_NAME}.elf )
set( BIN_NAME             ${TARGET_NAME}.bin )
set( ROM_NAME             ${TARGET_NAME}.v64 )


add_executable(${ELF_NAME}
    ${CMAKE_CURRENT_LIST_DIR}/asm/entry.s
    ${CMAKE_CURRENT_LIST_DIR}/asm/rom_header.s
    ${CMAKE_CURRENT_LIST_DIR}/src/main/gfxinit.c
    ${CMAKE_CURRENT_LIST_DIR}/src/main/graphic.c
    ${CMAKE_CURRENT_LIST_DIR}/src/main/main.c
    ${CMAKE_CURRENT_LIST_DIR}/src/main/stage00.c
    #${CMAKE_CURRENT_LIST_DIR}/nu0.ld
)

target_include_directories(${ELF_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# build linker script with gcc preprocessor
set(LD_SCRIPT ${CMAKE_CURRENT_BINARY_DIR}/nu0.ld)

# TODO: newer cpp versions output typedefs which are not supported in linker scripts
# TODO: use #${CMAKE_CXX_FLAGS}
set(CMAKE_C_PREPROCESSOR cpp-8)
add_custom_command(OUTPUT ${LD_SCRIPT}
    ##COMMAND ${CMAKE_CXX_COMPILER} -march=vr4300 -mtune=vr4300 -MMD -MP -MT -MF -o nu0.ld ${CMAKE_CURRENT_LIST_DIR}/nu0.ld
    COMMAND ${CMAKE_C_PREPROCESSOR} -P -Wno-trigraphs -I${libultra_DIR}/include/n64 -I${libultra_DIR}/include/n64/PR -I${libultra_DIR}/include/n64/nusys -Iinclude -I${CMAKE_CURRENT_BINARY_DIR} -I${CMAKE_CURRENT_BINARY_DIR}/include -Isrc -I. -D_FINALROM=1 -DNDEBUG=1 -DF3DEX_GBI_2=1 -DBUILD_DIR=${CMAKE_CURRENT_BINARY_DIR} -MMD -MP -MT ${CMAKE_CURRENT_BINARY_DIR}/nu0.ld -MF ${CMAKE_CURRENT_BINARY_DIR}/nu0.ld.d -o ${CMAKE_CURRENT_BINARY_DIR}/nu0.ld ${CMAKE_CURRENT_LIST_DIR}/nu0.ld
    COMMENT "Generating linker script nu0.ld"
)
add_custom_target(ldscript DEPENDS ${LD_SCRIPT})
add_dependencies(${ELF_NAME} ldscript)
#set_target_properties(${ELF_NAME} PROPERTIES LINK_DEPENDS ${LD_SCRIPT})

set(LINKER_FILE_NAME ${LD_SCRIPT})
target_link_options(${ELF_NAME} PUBLIC "-T${LINKER_FILE_NAME}")


# build boot object
add_custom_command(OUTPUT ${BOOT_OBJ}
    COMMAND ${CMAKE_OBJCOPY} -I binary -B mips -O elf32-bigmips ${BOOT} ${BOOT_OBJ}
    MAIN_DEPENDENCY ${BOOT}
)

#target_compile_options(${ELF_NAME} PUBLIC -m32)

target_link_libraries(${ELF_NAME}
    PRIVATE n64sdkmod::ultra
    PRIVATE n64sdkmod::nusys
    PRIVATE n64sdkmod::nualsgi
    PRIVATE n64sdkmod::nualstl
)
