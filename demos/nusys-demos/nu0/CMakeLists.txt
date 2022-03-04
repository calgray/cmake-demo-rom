
set(TARGET_NAME NU0-DEMO)

# build ROM
set( ELF_NAME             ${TARGET_NAME}.elf )
set( BIN_NAME             ${TARGET_NAME}.bin )
set( ROM_NAME             ${TARGET_NAME}.v64 )

set(BUILD_DIR             ${CMAKE_CURRENT_BINARY_DIR})
#set(BUILD_DIR             ${CMAKE_CURRENT_BINARY_DIR}/${TARGET_NAME})

# build boot object
add_custom_command(OUTPUT ${BOOT_OBJ}
    COMMAND ${CMAKE_OBJCOPY} -I binary -B mips -O elf32-bigmips ${BOOT_SRC} ${BOOT_OBJ}
    COMMENT "generating boot object"
    MAIN_DEPENDENCY ${BOOT}
)
add_custom_target(boot DEPENDS ${BOOT_OBJ})

# build object files
add_library(${TARGET_NAME} OBJECT
    ${BOOT_SRC}
    asm/entry.s
    asm/rom_header.s
)
target_include_directories(${TARGET_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
)

add_executable(${ELF_NAME}
    $<TARGET_OBJECTS:${TARGET_NAME}>
    src/main/gfxinit.c
    src/main/graphic.c
    src/main/main.c
    src/main/stage00.c
)
target_include_directories(${ELF_NAME}
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/include
)
# PRE_BUILD | PRE_LINK | POST_BUILD
add_custom_command(TARGET ${ELF_NAME}
  PRE_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${BUILD_DIR}/asm
  COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_OBJECTS:${TARGET_NAME}> ${BUILD_DIR}/asm
  COMMAND_EXPAND_LISTS
)


# build linker script with gcc preprocessor
set(LD_SCRIPT nu0.ld)
# TODO: use #${CMAKE_CXX_FLAGS}
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR})
set(MOD_FLAGS
-D_FINALROM=1 -DNDEBUG=1 -DF3DEX_GBI_2=1
-DBUILD_DIR=${BUILD_DIR}
-DN64SDK_DIR=${N64SDK_DIR}
-MMD -MP
)
set(MOD_INCLUDES
    -I${libultra_DIR}/include/n64
    -I${libultra_DIR}/include/n64/PR
    -I${libultra_DIR}/include/n64/nusys
    -Iinclude
    -I${CMAKE_CURRENT_BINARY_DIR}
    -I${CMAKE_CURRENT_BINARY_DIR}/include
    -Isrc
    -I.)
# TODO: newer cpp versions output typedefs which are not supported in linker scripts
set(CMAKE_C_PREPROCESSOR cpp-8)
add_custom_command(OUTPUT ${LD_SCRIPT}
    ##COMMAND ${CMAKE_CXX_COMPILER} -march=vr4300 -mtune=vr4300 -MMD -MP -MT -MF -o nu0.ld ${CMAKE_CURRENT_LIST_DIR}/nu0.ld
    COMMAND ${CMAKE_C_PREPROCESSOR} -P -Wno-trigraphs ${MOD_INCLUDES} ${MOD_FLAGS} -MT ${CMAKE_CURRENT_BINARY_DIR}/nu0.ld -MF ${CMAKE_CURRENT_BINARY_DIR}/nu0.ld.d -o ${CMAKE_CURRENT_BINARY_DIR}/nu0.ld ${CMAKE_CURRENT_LIST_DIR}/nu0.ld
    COMMENT "Generating linker script nu0.ld"
    MAIN_DEPENDENCY ${CMAKE_CURRENT_LIST_DIR}/nu0.ld
)
add_custom_target(ldscript DEPENDS ${LD_SCRIPT})
add_dependencies(${ELF_NAME} ldscript)
target_link_options(${ELF_NAME} PUBLIC "-T${LD_SCRIPT}")
#set_target_properties(${ELF_NAME} PROPERTIES LINK_DEPENDS ldscript)

add_dependencies(${ELF_NAME} boot)
#set_target_properties(${ELF_NAME} PROPERTIES LINK_DEPENDS boot)

add_dependencies(${TARGET_NAME} boot)

target_link_libraries(${ELF_NAME}
    PRIVATE n64sdkmod::ultra
    PRIVATE n64sdkmod::nusys
    PRIVATE n64sdkmod::nualsgi
    PRIVATE n64sdkmod::nualstl
)
