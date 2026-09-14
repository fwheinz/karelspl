cmake_minimum_required(VERSION 3.16)

project(spl_karel C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2_IMAGE REQUIRED SDL2_image)
pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)

# Common compiler options
add_compile_options(-O3 -ggdb -Wall)

# SPL library
add_library(spl_lib STATIC
    lib/libspl.c
)

target_include_directories(spl_lib PRIVATE
    include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# SPL executable
add_executable(spl
    spl.c
)

target_include_directories(spl PRIVATE include)

target_link_libraries(spl PRIVATE
    spl_lib
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    m
)

# Karel library
add_library(karel_lib STATIC
    lib/libkarel.c
)

target_include_directories(karel_lib PRIVATE
    include
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_IMAGE_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
)

# Karel executable
add_executable(karel
    karel.c
)

target_include_directories(karel PRIVATE include)

target_link_libraries(karel PRIVATE
    spl_lib
    karel_lib
    ${SDL2_LIBRARIES}
    ${SDL2_IMAGE_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    m
)
