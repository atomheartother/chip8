cmake_minimum_required(VERSION 3.0)
project(chip8 VERSION "0.1")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_executable(chip8)
target_sources(chip8
    PRIVATE ./src/main.cpp
    PRIVATE ./src/Display/Display.cpp
    PRIVATE ./src/Memory/Memory.cpp
    PRIVATE ./src/CPU/CPU.cpp
)

add_subdirectory(./lib/SFML)

target_link_libraries(chip8
    PRIVATE sfml-graphics
)