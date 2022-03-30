#include <iostream>

#include "emulator.hpp"

int main() {
    Emulator chip8;

    if(!chip8.loadROM("breakout.ch8")) {
        std::cerr << "cannot open file";
        return -1;
    }

    chip8.runROM();

    return 0;
}
