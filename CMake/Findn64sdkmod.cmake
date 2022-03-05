

set(N64SDK_DIR ${CMAKE_SOURCE_DIR}/external/n64sdkmod)

set(BOOT_SRC ${N64SDK_DIR}/lib/n64/PR/bootcode/boot.6102)
set(BOOT_OBJ boot.6102.o)

add_library(n64sdkmod::ultra STATIC IMPORTED)
set_target_properties(n64sdkmod::ultra
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/libultra.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64
)
add_library(n64sdkmod::nusys STATIC IMPORTED)
set_target_properties(n64sdkmod::nusys
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/nusys/libnusys.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64/nusys
)

add_library(n64sdkmod::nualstl STATIC IMPORTED)
set_target_properties(n64sdkmod::nualstl
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/nusys/libnualstl.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64/nusys
)

add_library(n64sdkmod::nualsgi STATIC IMPORTED)
set_target_properties(n64sdkmod::nualsgi
    PROPERTIES
    IMPORTED_LOCATION ${N64SDK_DIR}/lib/n64/nusys/libnualsgi.a
    INTERFACE_INCLUDE_DIRECTORIES ${N64SDK_DIR}/include/n64/nusys
)
