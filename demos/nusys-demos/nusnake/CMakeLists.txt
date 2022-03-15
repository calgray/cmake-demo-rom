
set(TARGET_NAME nusnake-demo)
set(LD_SCRIPT nusnake.ld)
# replace /usr with N64SDK_DIR in .ld

# build ROM
set( ELF_NAME             ${TARGET_NAME}.elf )
set( BIN_NAME             ${TARGET_NAME}.bin )
set( ROM_NAME             ${TARGET_NAME}.v64 )

# Tested with Unix Makefiles build dir
set(BUILD_DIR             "CMakeFiles/${ELF_NAME}.dir")

# TODO: newer cpp versions output typedefs which are not supported in linker scripts
set(CMAKE_C_PREPROCESSOR cpp-8)

# original config is RELWITHDEBINFO
file(GLOB_RECURSE ASM_SRCS RELATIVE ${CMAKE_CURRENT_LIST_DIR} asm/*.s)
file(GLOB_RECURSE SOUND_SRCS RELATIVE ${CMAKE_CURRENT_LIST_DIR} sound/*.s)
file(GLOB_RECURSE C_SRCS RELATIVE ${CMAKE_CURRENT_LIST_DIR} src/*.c)
file(GLOB_RECURSE ASSETS_SRCS RELATIVE ${CMAKE_CURRENT_LIST_DIR} assets/*.c)
add_executable(${ELF_NAME}
    ${ASM_SRCS}
    ${SOUND_SRCS}
    ${C_SRCS}
    ${ASSET_SRCS}
)
target_link_libraries(${ELF_NAME} n64)
target_include_directories(${ELF_NAME} PRIVATE .)
target_include_directories(${ELF_NAME} PUBLIC
    include
    sound
    assets
    ${N64SDK_DIR}/include/n64
    ${N64SDK_DIR}/include/n64/PR
    ${N64SDK_DIR}/include/n64/nusys
)
target_compile_definitions(${ELF_NAME} PUBLIC 
    _FINALROM=1
    F3DEX_GBI_2=1
)

###################
# build boot object
###################
add_custom_command(TARGET ${ELF_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_OBJCOPY} -I binary -B mips -O elf32-bigmips ${BOOT_SRC} ${BUILD_DIR}/${BOOT_OBJ}
    COMMENT "generating boot object"
    MAIN_DEPENDENCY ${BOOT_SRC}
    BYPRODUCTS ${BUILD_DIR}/${BOOT_OBJ}
)

#####################
# build linker script with gcc preprocessor
#####################
set(MOD_FLAGS
    -DNDEBUG=1
    -D_FINALROM=1
    -DF3DEX_GBI_2=1
    -DN_AUDIO=1
    -DBUILD_DIR=${BUILD_DIR}
    -DN64SDK_DIR=${N64SDK_DIR}
    -MMD -MP
)
set(MOD_INCLUDES
    -I${N64SDK_DIR}/include/n64
    -I${N64SDK_DIR}/include/n64/PR
    -I${N64SDK_DIR}/include/n64/nusys
    -I${CMAKE_CURRENT_BINARY_DIR}
    -I${CMAKE_CURRENT_BINARY_DIR}/include
    -Iinclude
    -Isrc
    -Isound
    -Iassets
    -I.
)
add_custom_command(TARGET ${ELF_NAME}
    PRE_LINK
    COMMAND ${CMAKE_C_PREPROCESSOR} -P -Wno-trigraphs
        ${MOD_INCLUDES}
        ${MOD_FLAGS}
        -MT ${BUILD_DIR}/${LD_SCRIPT}
        -MF ${BUILD_DIR}/${LD_SCRIPT}.d
        -o ${BUILD_DIR}/${LD_SCRIPT}
        ${CMAKE_CURRENT_LIST_DIR}/${LD_SCRIPT}
    COMMENT "Generating linker script ${LD_SCRIPT}"
    MAIN_DEPENDENCY ${CMAKE_CURRENT_LIST_DIR}/${LD_SCRIPT}
    DEPENDS ${BUILD_DIR}/${BOOT_OBJ}
    BYPRODUCTS ${BUILD_DIR}/${LD_SCRIPT} ${BUILD_DIR}/${LD_SCRIPT}.d
)

###########
# LINKING
###########
target_link_options(${ELF_NAME}
    PUBLIC
    ${BUILD_DIR}/${BOOT_OBJ}
    -Wl,-T,${BUILD_DIR}/${LD_SCRIPT},-Map,${BUILD_DIR}/nu0.map,--no-check-sections
)
target_link_directories(${ELF_NAME}
    PUBLIC
    .
    ${BUILD_DIR}
    ${N64SDK_DIR}
)
# order specific
target_link_libraries(${ELF_NAME}
    # n64sdkmod::nualstl_n
    # n64sdkmod::n_mus
    # n64sdkmod::n_audio_sc
    n64sdkmod::nualsgi_n
    n64sdkmod::nusys
    n64sdkmod::n_audio
    n64sdkmod::ultra_rom
    gcc
)

######################
# Pad (for bootcode?)
######################
add_custom_command(TARGET ${ELF_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} --pad-to=0x100000 --gap-fill=0xFF $<TARGET_FILE:${ELF_NAME}> ${ROM_NAME} -O binary
    COMMAND makemask ${ROM_NAME}
    BYPRODUCTS ${ROM_NAME}
    COMMENT "Generating ${ROM_NAME}"
)

#################
# Common Failures
#################
# undefined reference to `NU_SPEC_BOOT_STACK`
# - missing "-x assembler-with-cpp" on assembling asm objects

