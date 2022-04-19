#include <gtest/gtest.h>

#include "../emulator.hpp"

TEST(opcodetest, 00E0) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.push_back(0x00E0);
    chip8.loadROM(rom);

    chip8.runNextCycle();

    auto displayBuffer = chip8.getDisplayBuffer();

    for (auto &i : displayBuffer)
        EXPECT_EQ(i, 0);
}

TEST(opcodetest, 00EE) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x02208; //call
    rom[4] = 0x000EE; //return
    chip8.loadROM(rom);

    chip8.runNextCycle();
    EXPECT_EQ(chip8.getProgramCounter() - chip8.getMemory().begin(), 520);

    chip8.runNextCycle();

    EXPECT_EQ(chip8.getProgramCounter() - chip8.getMemory().begin(), 514);
}

TEST(opcodetest, 1NNN) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x01212; //jmp to 530
    chip8.loadROM(rom);

    chip8.runNextCycle();

    EXPECT_EQ(chip8.getProgramCounter() - chip8.getMemory().begin(), 530);
}


TEST(opcodetest, 2NNN) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x02208; //call
    chip8.loadROM(rom);

    chip8.runNextCycle();
    EXPECT_EQ(chip8.getProgramCounter() - chip8.getMemory().begin(), 520);
}

TEST(opcodetest, 3XNN) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x0600F; //store 15 in VO
    rom[1] = 0x0300F;
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getProgramCounter() - chip8.getMemory().begin(), 518);
}

TEST(opcodetest, 4XNN) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x0600F; //store 15 in VO
    rom[1] = 0x04000;
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getProgramCounter() - chip8.getMemory().begin(), 518);
}

TEST(opcodetest, 5XYO) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x0600F; //store 15 in VO
    rom[1] = 0x0610F; //store 15 in V1
    rom[2] = 0x05010;
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getProgramCounter() - chip8.getMemory().begin(), 520);
}

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

TEST(opcodetest, 7XNN) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x06F09; //store 9 in VF
    rom[1] = 0x07F09; //add 9 to VF
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[15], 18);
}

TEST(opcodetest, 8XY0) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x0600F; //store 15 in VO
    rom[1] = 0x08100; //store V1 = V0
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[1], 15);
}

TEST(opcodetest, 8XY1) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x06001; //store 1 in VO
    rom[1] = 0x06101; //store 1 in V1
    rom[2] = 0x08101; //store V1 = V1 | V0
    rom[3] = 0x06100; //store 0 in V1
    rom[4] = 0x08101; //store V1 = V1 | V0
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[1], 1);

    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[1], 1);
}

TEST(opcodetest, 8XY2) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x06001; //store 1 in VO
    rom[1] = 0x06101; //store 1 in V1
    rom[2] = 0x08102; //store V1 = V1 & V0
    rom[3] = 0x06100; //store 0 in V1
    rom[4] = 0x08102; //store V1 = V1 & V0
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[1], 1);

    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[1], 0);
}

TEST(opcodetest, 8XY3) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x06001; //store 1 in VO
    rom[1] = 0x06101; //store 1 in V1
    rom[2] = 0x08103; //store V1 = V1 (xor) V0
    rom[3] = 0x06100; //store 0 in V1
    rom[4] = 0x08103; //store V1 = V1 (xor) V0
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[1], 0);

    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[1], 1);
}

TEST(opcodetest, 8XY4) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x06001; //store 1 in VO
    rom[1] = 0x06101; //store 1 in V1
    rom[2] = 0x08104; //store V1 += V0
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[1], 2);
}

TEST(opcodetest, 8XY5) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x0600F; //store 15 in VO
    rom[1] = 0x06101; //store 1 in V1
    rom[2] = 0x08015; //store V0 -= V1
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[0], 14);
}

TEST(opcodetest, 8XY6) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x06005; //store 5 in VO
    rom[1] = 0x08006; //store V0 = V0 >> 1 and VF = least significant bit
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[0], 2);
    EXPECT_EQ(chip8.getRegisters()[15], 1);
}

TEST(opcodetest, 8XY7) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x06001; //store 1 in VO
    rom[1] = 0x0610A; //store 10 in V1
    rom[2] = 0x08017; //store V0 = V1 - v0
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[0], 9);
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

    while(!chip8.isEnd()){
        chip8.runNextCycle();
    }

    EXPECT_EQ(chip8.getMemory()[1024], 10);
}

TEST(opcodetest, collision) {
    Emulator chip8;
    std::vector<uint16_t> rom;

    rom.resize(20);
    rom[0] = 0x0A208; //set I to 520
    rom[1] = 0x0D001; //draw 1 pixel at 0,0
    rom[2] = 0x0D001; //draw 1 pixel at 0,0
    rom[4] = 0x08000; //single pixel sprite at 520
    chip8.loadROM(rom);

    chip8.runNextCycle();
    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[15], 0);

    chip8.runNextCycle();

    EXPECT_EQ(chip8.getRegisters()[15], 1);
}
