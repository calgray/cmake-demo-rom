
set(TARGET_NAME NU0-DEMO)

# build ROM
set( ELF_NAME             ${TARGET_NAME}.elf )
set( BIN_NAME             ${TARGET_NAME}.bin )
set( ROM_NAME             ${TARGET_NAME}.v64 )

set(BUILD_DIR             "CMakeFiles/NU0-DEMO.elf.dir")

message("BUILD_DIR ${BUILD_DIR}")
message("N64SDK_DIR ${N64SDK_DIR}")

# TODO: newer cpp versions output typedefs which are not supported in linker scripts
set(CMAKE_C_PREPROCESSOR cpp-8)

add_executable(${ELF_NAME}
    asm/entry.s
    asm/rom_header.s
    src/main/gfxinit.c
    src/main/graphic.c
    src/main/main.c
    src/main/stage00.c
)
target_include_directories(${ELF_NAME} PRIVATE .)
target_include_directories(${ELF_NAME}
    PUBLIC
    include
    ${N64SDK_DIR}/include/n64
    ${N64SDK_DIR}/include/n64/PR
    ${N64SDK_DIR}/include/n64/nusys
)
target_compile_definitions(${ELF_NAME} PUBLIC _FINALROM=1)
target_compile_definitions(${ELF_NAME} PUBLIC NDEBUG=1)
target_compile_definitions(${ELF_NAME} PUBLIC F3DEX_GBI_2=1)
target_compile_options(${ELF_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${C_RELEASE_OPTIONS}>")

# RELEASE
set(CMAKE_C_FLAGS "-G 0 -O2 -g -march=vr4300 -mtune=vr4300 -mabi=32 -ffreestanding -mfix4300")
set(CMAKE_ASM_FLAGS "-G 0 -O2 -g -march=vr4300 -mtune=vr4300 -mabi=32 -ffreestanding -mfix4300 -x assembler-with-cpp -MMD")
#-MF build/asm/entry.d

# build boot object
add_custom_command(TARGET ${ELF_NAME}
    PRE_BUILD
    COMMAND ${CMAKE_OBJCOPY} -I binary -B mips -O elf32-bigmips ${BOOT_SRC} ${BUILD_DIR}/${BOOT_OBJ}
    COMMENT "generating boot object"
    MAIN_DEPENDENCY ${BOOT_SRC}
    BYPRODUCTS ${BUILD_DIR}/${BOOT_OBJ}
)


# build linker script with gcc preprocessor
# TODO: use #${CMAKE_CXX_FLAGS}
set(LD_SCRIPT nu0.ld)
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
    -I.
)

add_custom_command(TARGET ${ELF_NAME}
    PRE_LINK
    COMMAND ${CMAKE_C_PREPROCESSOR} -P -Wno-trigraphs ${MOD_INCLUDES} ${MOD_FLAGS}
        -MT ${BUILD_DIR}/nu0.ld
        -MF ${BUILD_DIR}/nu0.ld.d
        -o ${BUILD_DIR}/nu0.ld
        ${CMAKE_CURRENT_LIST_DIR}/nu0.ld
    COMMENT "Generating linker script nu0.ld"
    MAIN_DEPENDENCY ${CMAKE_CURRENT_LIST_DIR}/nu0.ld
    BYPRODUCTS ${BUILD_DIR}/nu0.ld ${BUILD_DIR}/nu0.ld.d
)
target_link_options(${ELF_NAME} PUBLIC "-T${LD_SCRIPT}")

# target_link_options(${ELF_NAME}
#     PUBLIC
#     -Map nu0.map
#     --no-check-sections
#     /home/callan/Code/n64/cmake-demo-rom/external/n64sdkmod/lib/n64/nusys/nusys_rom.o
# )
# target_link_directories(${ELF_NAME}
#     PUBLIC
#     .
#     ${BUILD_DIR}
#     ${N64SDK_DIR}
#     ${N64SDK_DIR}/lib/n64/nusys
#     /usr/lib/gcc/mips64-elf/11.2.0/32
#     /usr/lib/gcc/mips64-elf/11.2.0
# )
# target_link_libraries(${ELF_NAME}
#     ultra_rom
#     gcc
# )
# CMAKE style
# target_link_libraries(${ELF_NAME}
#     PRIVATE n64sdkmod::ultra
#     PRIVATE n64sdkmod::nusys
#     PRIVATE n64sdkmod::nualsgi
#     PRIVATE n64sdkmod::nualstl
#     ultra_rom
# )

#set_target_properties(${ELF_NAME} PROPERTIES LINK_DEPENDS ldscript)
#set_target_properties(${ELF_NAME} PROPERTIES LINK_DEPENDS boot)


set(CUSTOM_LINK_FLAGS
"-L build -T ${BUILD_DIR}/nu0.ld -Map ${BUILD_DIR}/nu0.map --no-check-sections -o ${BUILD_DIR}/nu0.elf"
" /home/callan/Code/n64/cmake-demo-rom/external/n64sdkmod/lib/n64/nusys/nusys_rom.o"
" ${BUILD_DIR}/src/main/gfxinit.o"
" ${BUILD_DIR}/src/main/graphic.o"
" ${BUILD_DIR}/src/main/main.o"
" ${BUILD_DIR}/src/main/stage00.o"
" ${BUILD_DIR}/asm/entry.o"
" ${BUILD_DIR}/asm/rom_header.o"
" ${BUILD_DIR}/boot.6102.o"
" -L/home/callan/Code/n64/cmake-demo-rom/external/n64sdkmod/lib/n64/nusys -lnusys"
" -L/home/callan/Code/n64/cmake-demo-rom/external/n64sdkmod/lib/n64 -lultra_rom -lultra"
" -L/usr/lib/gcc/mips64-elf/11.2.0/32 -lgcc"
" -o ${CMAKE_CURRENT_BINARY_DIR}/${ELF_NAME}"
)
string(CONCAT CUSTOM_LINK_FLAGS ${CUSTOM_LINK_FLAGS})
# NOTE: link directories must come before link libraries
set(CMAKE_C_LINK_EXECUTABLE "${CMAKE_LINKER} ${CUSTOM_LINK_FLAGS}")

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

