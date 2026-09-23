
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(LIB_DIR "${CMAKE_SOURCE_DIR}/lib")
set(INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include" "${CMAKE_SOURCE_DIR}/include/SDL2")

# Compiler flags
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(-O3 -ggdb -Wall)
elseif(MSVC)
    add_compile_options(/O2 /W4)
endif()

# ---------------------------------------------------------------------------
# SDL2
# ---------------------------------------------------------------------------

if(WIN32)

    add_library(SDL2 SHARED IMPORTED)

    set_target_properties(SDL2 PROPERTIES
            IMPORTED_IMPLIB
            "${LIB_DIR}/libSDL2.dll.a"
            IMPORTED_LOCATION
            "${LIB_DIR}/SDL2.dll"
            INTERFACE_INCLUDE_DIRECTORIES
            "${INCLUDE_DIR}"
    )

    add_library(SDL2_image SHARED IMPORTED)

    set_target_properties(SDL2_image PROPERTIES
            IMPORTED_IMPLIB
            "${LIB_DIR}/libSDL2_image.dll.a"
            IMPORTED_LOCATION
            "${LIB_DIR}/SDL2_image.dll"
            INTERFACE_INCLUDE_DIRECTORIES
            "${INCLUDE_DIR}"
    )

    add_library(SDL2_ttf SHARED IMPORTED)

    set_target_properties(SDL2_ttf PROPERTIES
            IMPORTED_IMPLIB
            "${LIB_DIR}/libSDL2_ttf.dll.a"
            IMPORTED_LOCATION
            "${LIB_DIR}/SDL2_ttf.dll"
            INTERFACE_INCLUDE_DIRECTORIES
            "${INCLUDE_DIR}"
    )

else()

    # Linux
    find_package(PkgConfig REQUIRED)

    pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
    pkg_check_modules(SDL2_IMAGE REQUIRED IMPORTED_TARGET SDL2_image)
    pkg_check_modules(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)

endif()

# ---------------------------------------------------------------------------
# Target helpers
# ---------------------------------------------------------------------------

function(add_spl_target TARGET_NAME)
    add_executable(${TARGET_NAME}
            ${ARGN}
            lib/libspl.c
    )

    target_include_directories(${TARGET_NAME} PRIVATE
            "${INCLUDE_DIR}"
    )

    if(WIN32)
        target_link_libraries(${TARGET_NAME} PRIVATE
                SDL2
                SDL2_image
                SDL2_ttf
        )
    else()
        target_link_libraries(${TARGET_NAME} PRIVATE
                PkgConfig::SDL2
                PkgConfig::SDL2_IMAGE
                PkgConfig::SDL2_TTF
                m
        )
    endif()
endfunction()


function(add_karel_target TARGET_NAME)
    add_executable(${TARGET_NAME}
            ${ARGN}
            lib/libspl.c
            lib/libkarel.c
    )

    target_include_directories(${TARGET_NAME} PRIVATE
            "${INCLUDE_DIR}"
    )

    if(WIN32)
        target_link_libraries(${TARGET_NAME} PRIVATE
                SDL2
                SDL2_image
                SDL2_ttf
        )
    else()
        target_link_libraries(${TARGET_NAME} PRIVATE
                PkgConfig::SDL2
                PkgConfig::SDL2_IMAGE
                PkgConfig::SDL2_TTF
                m
        )
    endif()
endfunction()
