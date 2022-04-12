#include <iostream>

#include "emulator.hpp"
#include "renderer.hpp"

int main() {
    Emulator chip8;
    Renderer renderer;

    if(!chip8.loadROM("breakout.ch8")) {
        std::cerr << "cannot open file";
        return -1;
    }

    renderer.init();

    while(!chip8.isEnd()){
        chip8.runNextCycle();
        renderer.poll();
        renderer.render(chip8.getDisplayBuffer());
    }

    return 0;
}
