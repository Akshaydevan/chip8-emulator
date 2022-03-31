#include <gtest/gtest.h>

#include "../emulator.hpp"

TEST(opcodetest, 6XNN) {
    Emulator chip8;

    chip8.loadROMString("6000");
    chip8.loadROMString("6101");
    chip8.loadROMString("6A0F");
    chip8.loadROMString("6FAA");

    chip8.runROM();

    EXPECT_EQ(chip8.getRegisters()[0], 0);
    EXPECT_EQ(chip8.getRegisters()[1], 1);
    EXPECT_EQ(chip8.getRegisters()[10], 15);
    EXPECT_EQ(chip8.getRegisters()[15], 170);

}
