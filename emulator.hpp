#pragma once

#include <iostream>
#include <vector>

uint16_t byteAtIndex(std::uint16_t, int, int = 0);

class Emulator
{
public:
    Emulator();

    bool loadROM(std::string);
    void loadROMString(std::string);
    void runROM();

private:
    std::vector<std::uint16_t> m_memory;
    std::vector<std::uint16_t> m_registers;
    std::uint16_t m_registerI;
};

