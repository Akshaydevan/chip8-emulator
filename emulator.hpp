#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include "keyboardhandler.hpp"

uint16_t byteAtIndex(std::uint16_t, int, int = 0);

class Emulator
{
public:
    Emulator();

    bool loadROM(std::string);
    void loadROM(std::vector<std::uint16_t> &);
    void runNextCycle();

    bool isEnd();

    std::vector<std::uint16_t>& getRegisters();
    std::vector<std::uint8_t>& getMemory();
    std::vector<std::uint8_t>::iterator getProgramCounter();
    std::array<bool, 2048>& getDisplayBuffer();

    void setKeyboardHandler();

private:
    std::vector<std::uint8_t> m_memory;
    std::stack<std::vector<std::uint8_t>::iterator> m_callStack;
    std::vector<std::uint16_t> m_registers;
    std::uint16_t m_registerI;
    std::array<bool, 2048> m_displayBuffer;
    std::vector<std::uint8_t>::iterator m_progCounter;
    KeyboardHandler &m_keyboardHandler;
    bool m_endOfROM;
};

