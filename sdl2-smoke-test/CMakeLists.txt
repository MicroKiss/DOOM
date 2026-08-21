cmake_minimum_required(VERSION 3.20)
project(sdl2_smoke_test C)

find_package(SDL2 CONFIG REQUIRED)

add_executable(sdl2_smoke_test main.c)

target_link_libraries(
    sdl2_smoke_test
    PRIVATE
        SDL2::SDL2main
        SDL2::SDL2
)