cmake_minimum_required(VERSION 3.5)

project(chip8-emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



add_executable(chip8-emulator
    main.cpp
    emulator.hpp
    emulator.cpp
)

add_library(emulator STATIC emulator.hpp emulator.cpp)

if(BUILD_TESTING)
    enable_testing()
    add_subdirectory(tests)
endif()
