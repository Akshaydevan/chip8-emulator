#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <stack>
#include <chrono>
#include "keyboardhandler.hpp"

uint16_t byteAtIndex(std::uint16_t, int, int = 0);

struct CallInfo{
    std::vector<uint8_t>::iterator returnAddress;
    std::vector<uint8_t> registers;
};

class Emulator
{
public:
    Emulator();

    bool loadROM(std::string);
    void loadROM(std::vector<std::uint16_t> &);
    void runNextCycle();

    bool isEnd();

    std::vector<std::uint8_t>& getRegisters();
    std::vector<std::uint8_t>& getMemory();
    std::vector<std::uint8_t>::iterator getProgramCounter();
    std::array<bool, 2048>& getDisplayBuffer();

    void setKeyboardHandler();

private:
    std::vector<std::uint8_t> m_memory;
    std::stack<CallInfo> m_callStack;
    std::vector<std::uint8_t> m_registers;
    std::uint16_t m_registerI;
    std::array<bool, 2048> m_displayBuffer;
    std::vector<std::uint8_t>::iterator m_progCounter;
    int m_timerValue;
    std::chrono::high_resolution_clock::time_point m_timer;
    KeyboardHandler &m_keyboardHandler;
    bool m_endOfROM;
};

