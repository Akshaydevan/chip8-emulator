#include <iostream>

#include "emulator.hpp"
#include "renderer.hpp"


int main(int argc, char *argv[]) {
    Emulator chip8;
    Renderer renderer;

    std::string rom;

    if (argc > 1) {
        rom = argv[1];
    }
    else {
        std::cerr << "add rom name as argument\n";
        return -1;
    }

    if(!chip8.loadROM(rom)) {
        std::cerr << "cannot open rom\n" << std::endl;
        return -1;
    }

    renderer.init();
    int frame = 0;


    while(!chip8.isEnd()){
        chip8.runNextCycle();

        if (!renderer.poll())
            break;

        renderer.render(chip8.getDisplayBuffer());


        frame++;
    }

    return 0;
}
