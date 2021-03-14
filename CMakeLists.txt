cmake_minimum_required(VERSION 3.0)
project(chip8 VERSION "0.1")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

# Release by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

add_executable(chip8)

set_target_properties(chip8
PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)



target_sources(chip8
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Memory/Memory.cpp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/CPU/CPU.cpp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Keys/Keys.cpp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Screen/Screen.cpp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Screen/SFML/SFML.cpp
)

add_subdirectory(./lib/SFML)

target_link_libraries(chip8
    PRIVATE sfml-graphics
)