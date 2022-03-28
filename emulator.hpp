#pragma once

#include <iostream>
#include <vector>

uint16_t byteIndex(std::uint16_t, int);

class Emulator
{
public:
    Emulator();

    bool loadROM(std::string);
    void runROM();

private:
    std::vector<std::uint16_t> m_memory;
};

