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

if(CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "-Wall -Wextra")
  set(CMAKE_CXX_FLAGS_DEBUG "-g")
  # set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif(CMAKE_COMPILER_IS_GNUCXX)

add_executable(chip8)

target_sources(chip8
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Memory/Memory.cpp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/CPU/CPU.cpp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Keys/Keys.cpp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Screen/Screen.cpp
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/Screen/SDL/SDL.cpp
)

if (EMSCRIPTEN)
  set(USE_FLAGS "-s WASM=0 -s USE_SDL=2 -s EXPORTED_FUNCTIONS=['_emLoadRom'] --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/build/Brick.ch8 -s EXTRA_EXPORTED_RUNTIME_METHODS='['allocate', 'intArrayFromString']'")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${USE_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${USE_FLAGS}")
  set(CMAKE_EXECUTABLE_SUFFIX .js)
  add_compile_definitions(chip8 EMSCRIPTEN)
  set_target_properties(chip8
  PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/public
  )
else()
  # Not compiling to WASM, set up desktop stuff
  include(cmake/CPM.cmake)

  set(CPM_SOURCE_CACHE ${CMAKE_CURRENT_SOURCE_DIR}/lib/)

  CPMAddPackage(
    NAME SDL2         # The unique name of the dependency (should be the exported target's name)
    GIT_TAG release-2.0.14      # The minimum version of the dependency (optional, defaults to 0)
    GIT_REPOSITORY git@github.com:libsdl-org/SDL.git
    OPTIONS
      "SDL_SHARED On"
      "SDL_STATIC Off"
  )

  target_link_libraries(chip8
      PRIVATE SDL2
  )

  set(OUTPUT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)
  if (WIN32)
    set_target_properties(chip8 SDL2
    PROPERTIES
      ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_DIR}
      LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR}
      RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    )
  else()
    set_target_properties(chip8
    PROPERTIES
      RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR}
    )
  endif(WIN32)
endif()