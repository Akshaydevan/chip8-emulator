#include <chrono>
#include <iostream>

#include "emulator.hpp"
#include "renderer.hpp"

int main(int argc, char* argv[])
{
    Emulator chip8;
    Renderer renderer;

    std::string rom;

    if (argc > 1) {
        rom = argv[1];
    } else {
        std::cerr << "add rom name as argument\n";
        return -1;
    }

    if (!chip8.loadROM(rom)) {
        std::cerr << "cannot open rom\n";
        return -1;
    }

    renderer.init();

    int cpuCounter = 0;
    auto cpuClock = std::chrono::high_resolution_clock::now();

    while (!chip8.isEnd()) {
        // Run only 50 cpu cycles per 100 milliseconds (500 per seconds)
        // to simulate real speed of chip8
        if (cpuCounter < 50) {
            chip8.runNextCycle();

            if (!renderer.poll())
                break;

            if (chip8.shouldDraw())
                renderer.render(chip8.getDisplayBuffer());
        }

        auto now = std::chrono::high_resolution_clock::now();
        int timeElapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - cpuClock).count();

        if (timeElapsed >= 100) {
            cpuCounter = 0;
            cpuClock = std::chrono::high_resolution_clock::now();
        }

        cpuCounter++;
    }

    return 0;
}
