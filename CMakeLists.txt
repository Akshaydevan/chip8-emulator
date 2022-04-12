cmake_minimum_required(VERSION 3.5)

project(chip8-emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML COMPONENTS system window graphics CONFIG REQUIRED)

add_executable(chip8-emulator
    main.cpp
    emulator.hpp
    emulator.cpp
    renderer.hpp
    renderer.cpp
)

target_link_libraries(chip8-emulator PRIVATE sfml-system sfml-graphics sfml-window)
add_library(emulator STATIC emulator.hpp emulator.cpp)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
