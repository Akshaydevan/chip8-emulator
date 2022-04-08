#include <gtest/gtest.h>

#include "../emulator.hpp"

TEST(opcodetest, 6XNN) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.push_back(0x06000);
    rom.push_back(0x06101);
    rom.push_back(0x06A0F);
    rom.push_back(0x06FAA);

    chip8.loadROM(rom);

    while(!chip8.isEnd()){
        chip8.runNextCycle();
    }

    EXPECT_EQ(chip8.getRegisters()[0], 0);
    EXPECT_EQ(chip8.getRegisters()[1], 1);
    EXPECT_EQ(chip8.getRegisters()[10], 15);
    EXPECT_EQ(chip8.getRegisters()[15], 170);

}

TEST(opcodetest, FX55) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.push_back(0x0600A);
    rom.push_back(0x0610A);
    rom.push_back(0x06A0A);
    rom.push_back(0x0A400);
    rom.push_back(0x0F055);

    chip8.loadROM(rom);
    chip8.runROM();

    EXPECT_EQ(chip8.getMemory()[1024], 10);
}
