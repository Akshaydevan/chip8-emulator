cmake_minimum_required(VERSION 3.5)

project(chip8-emulator LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()

add_executable(chip8-emulator
    main.cpp
    emulator.hpp
    emulator.cpp
)

