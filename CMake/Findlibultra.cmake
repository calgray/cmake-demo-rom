
add_library(libultra::libultra STATIC IMPORTED)

set_target_properties(libultra::libultra
    PROPERTIES
    IMPORTED_LOCATION ${TOOLCHAIN_PREFIX}/lib/libultra.a
    INTERFACE_INCLUDE_DIRECTORIES ${TOOLCHAIN_PREFIX}/include
)
