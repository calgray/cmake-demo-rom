

set(N64SDK_DIR ${CMAKE_SOURCE_DIR}/external/n64sdkmod)

set(BOOT_SRC ${N64SDK_DIR}/lib/n64/PR/bootcode/boot.6102)
set(BOOT_OBJ boot.6102.o)
# ultra
add_library(n64sdkmod::ultra STATIC IMPORTED)
set_target_properties(n64sdkmod::ultra
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/libultra.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64
)
# ultra_rom
add_library(n64sdkmod::ultra_rom STATIC IMPORTED)
set_target_properties(n64sdkmod::ultra_rom
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/libultra_rom.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64
)
# nualstl_n
add_library(n64sdkmod::nualstl_n STATIC IMPORTED)
set_target_properties(n64sdkmod::nualstl_n
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/nusys/libnualstl_n.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64/nusys
)
# n_audio_sc
add_library(n64sdkmod::n_audio_sc STATIC IMPORTED)
set_target_properties(n64sdkmod::n_audio_sc
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/libn_audio_sc.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64
)
# n_audio
add_library(n64sdkmod::n_audio STATIC IMPORTED)
set_target_properties(n64sdkmod::n_audio
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/libn_audio.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64
)
# n_mus
add_library(n64sdkmod::n_mus STATIC IMPORTED)
set_target_properties(n64sdkmod::n_mus
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/libn_mus.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64
)
# nualstl
add_library(n64sdkmod::nualstl STATIC IMPORTED)
set_target_properties(n64sdkmod::nualstl
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/nusys/libnualstl.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64/nusys
)
# nualsgi_n
add_library(n64sdkmod::nualsgi_n STATIC IMPORTED)
set_target_properties(n64sdkmod::nualsgi_n
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/nusys/libnualsgi_n.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64/nusys
)
# nusys
add_library(n64sdkmod::nusys STATIC IMPORTED)
set_target_properties(n64sdkmod::nusys
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/nusys/libnusys.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64/nusys
)
