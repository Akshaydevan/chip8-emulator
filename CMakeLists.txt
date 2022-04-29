cmake_minimum_required(VERSION 3.5)

project(chip8-emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_executable(chip8-emulator
    src/main.cpp
    src/emulator.hpp
    src/emulator.cpp
    src/renderer.hpp
    src/renderer.cpp
    src/keyboardhandler.hpp
    src/keyboardhandler.cpp
)

target_link_libraries(chip8-emulator PRIVATE sfml-system sfml-graphics sfml-window)
add_library(emulator STATIC src/emulator.hpp src/emulator.cpp src/keyboardhandler.hpp src/keyboardhandler.cpp)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
