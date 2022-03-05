
set(TARGET_NAME NU0-DEMO)

# build ROM
set( ELF_NAME             ${TARGET_NAME}.elf )
set( BIN_NAME             ${TARGET_NAME}.bin )
set( ROM_NAME             ${TARGET_NAME}.v64 )

# Tested with Unix Makefiles build dir
set(BUILD_DIR             "CMakeFiles/NU0-DEMO.elf.dir")

message("BUILD_DIR ${BUILD_DIR}")
message("N64SDK_DIR ${N64SDK_DIR}")

# TODO: newer cpp versions output typedefs which are not supported in linker scripts
set(CMAKE_C_PREPROCESSOR cpp-8)

# original config is RELWITHDEBINFO
add_executable(${ELF_NAME}
    asm/entry.s
    asm/rom_header.s
    src/main/gfxinit.c
    src/main/graphic.c
    src/main/main.c
    src/main/stage00.c
)
target_include_directories(${ELF_NAME} PRIVATE .)
target_include_directories(${ELF_NAME} PUBLIC
    include
    ${N64SDK_DIR}/include/n64
    ${N64SDK_DIR}/include/n64/PR
    ${N64SDK_DIR}/include/n64/nusys
)
target_compile_definitions(${ELF_NAME} PUBLIC _FINALROM=1)
target_compile_definitions(${ELF_NAME} PUBLIC NDEBUG=1)
target_compile_definitions(${ELF_NAME} PUBLIC F3DEX_GBI_2=1)
# target_compile_options(${ELF_NAME} PUBLIC
#     -march=vr4300
#     -mtune=vr4300
#     -mabi=32
#     -mfix4300
#     -ffreestanding
# )

# build boot object
add_custom_command(TARGET ${ELF_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_OBJCOPY} -I binary -B mips -O elf32-bigmips ${BOOT_SRC} ${BUILD_DIR}/${BOOT_OBJ}
    COMMENT "generating boot object"
    MAIN_DEPENDENCY ${BOOT_SRC}
    BYPRODUCTS ${BUILD_DIR}/${BOOT_OBJ}
)

# build linker script with gcc preprocessor
set(LD_SCRIPT nu0.ld)
set(MOD_FLAGS
    -D_FINALROM=1 -DNDEBUG=1 -DF3DEX_GBI_2=1
    -DBUILD_DIR=${BUILD_DIR}
    -DN64SDK_DIR=${N64SDK_DIR}
    -MMD -MP
)
set(MOD_INCLUDES
    -I${N64SDK_DIR}/include/n64
    -I${N64SDK_DIR}/include/n64/PR
    -I${N64SDK_DIR}/include/n64/nusys
    -Iinclude
    -I${CMAKE_CURRENT_BINARY_DIR}
    -I${CMAKE_CURRENT_BINARY_DIR}/include
    -Isrc
    -I.
)
add_custom_command(TARGET ${ELF_NAME}
    PRE_LINK
    COMMAND ${CMAKE_C_PREPROCESSOR} -P -Wno-trigraphs
        ${MOD_INCLUDES}
        ${MOD_FLAGS}
        -MT ${BUILD_DIR}/nu0.ld
        -MF ${BUILD_DIR}/nu0.ld.d
        -o ${BUILD_DIR}/nu0.ld
        ${CMAKE_CURRENT_LIST_DIR}/nu0.ld
    COMMENT "Generating linker script nu0.ld"
    MAIN_DEPENDENCY ${CMAKE_CURRENT_LIST_DIR}/nu0.ld
    DEPENDS ${BUILD_DIR}/${BOOT_OBJ}
    BYPRODUCTS ${BUILD_DIR}/nu0.ld ${BUILD_DIR}/nu0.ld.d
)

set(MODERN ON)
if(MODERN)
    target_link_options(${ELF_NAME}
        PUBLIC
        -march=vr4300 -mtune=vr4300 -mabi=32 -ffreestanding -mfix4300
        ${BUILD_DIR}/${BOOT_OBJ}
        -Wl,-T,${BUILD_DIR}/${LD_SCRIPT},-Map,${BUILD_DIR}/nu0.map,--no-check-sections
    )
    target_link_directories(${ELF_NAME}
        PUBLIC
        .
        ${BUILD_DIR}
        ${N64SDK_DIR}
        ${N64SDK_DIR}/lib/n64/nusys
        ${N64SDK_DIR}/lib/n64
        /usr/lib/gcc/mips64-elf/11.2.0/32
        /usr/lib/gcc/mips64-elf/11.2.0
    )
    target_link_libraries(${ELF_NAME}
        n64sdkmod::nusys
        ultra_rom
        gcc
    )

    #<CMAKE_C_COMPILER>     := ${CMAKE_C_COMPILER}
    #<FLAGS>                := ${CMAKE_C_FLAGS}
    #<CMAKE_C_LINK_FLAGS>   :=
    #<LINK_FLAGS>           :=
    #<OBJECTS>              :=
    #<TARGET>               :=
    #<LINK_LIBRARIES>       := 
    
    set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
    # default:
    # set(CMAKE_C_LINK_EXECUTABLE "<CMAKE_C_COMPILER> <FLAGS> <CMAKE_C_LINK_FLAGS> <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")
endif()


add_custom_command(TARGET ${ELF_NAME}
    POST_BUILD
    # pad (for bootcode?)
    COMMAND ${CMAKE_OBJCOPY} --pad-to=0x100000 --gap-fill=0xFF $<TARGET_FILE:${ELF_NAME}> ${ROM_NAME} -O binary
    COMMAND makemask ${ROM_NAME}
    BYPRODUCTS ${ROM_NAME}
    COMMENT "Generating ${ROM_NAME}"
)

# Common Failures
# undefined reference to `NU_SPEC_BOOT_STACK`
# - missing "-x assembler-with-cpp" on assembling asm objects

