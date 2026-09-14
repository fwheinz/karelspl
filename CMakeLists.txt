cmake_minimum_required(VERSION 3.20)

project(spl_karel C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)

add_executable(spl
    spl.c
    lib/libspl.c
)

target_include_directories(spl PRIVATE include)

if(UNIX AND NOT APPLE)
    set(MATH_LIB m)
else()
    set(MATH_LIB "")
endif()

target_link_libraries(spl PRIVATE
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
		${MATH_LIB}
)

add_executable(karel
    karel.c
    lib/libspl.c
    lib/libkarel.c
)

target_include_directories(karel PRIVATE include)

target_link_libraries(karel PRIVATE
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
		${MATH_LIB}
)
