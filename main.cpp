#include <iostream>

#include "emulator.hpp"

int main() {
    Emulator chip8;

    if(!chip8.loadROM("breakout.ch8")) {
        std::cerr << "cannot open file";
        return -1;
    }

    while(!chip8.isEnd()){
        chip8.runNextCycle();
    }

    return 0;
}
