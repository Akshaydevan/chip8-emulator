#include "emulator.hpp"
#include <fstream>
#include <sstream>
#include <ctime>

Emulator::Emulator()
    :m_keyboardHandler(KeyboardHandler::getInstance())
{
    m_memory.resize(4096);
    m_registers.resize(16);
    m_displayBuffer.fill(0);
    m_progCounter = m_memory.begin() + 512;
    m_endOfROM = false;

    std::srand(std::time(nullptr));
}

bool Emulator::loadROM(std::string filename) {
    std::ifstream file(filename, std::ios_base::binary);

    if (!file.is_open()) {
        return false;
    }

    std::vector<uint8_t> buffer{ std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};

    std::copy(buffer.begin(), buffer.end(), m_memory.begin() + 512);

    return true;
}

void Emulator::loadROM(std::vector<std::uint16_t> &rom) {
    int index = 512;

    for (auto opcode : rom) {
        uint8_t higherbyte = opcode >> 8;
        uint8_t lowerbyte = opcode & 0x0FF;

        m_memory[index] = higherbyte;
        m_memory[index + 1] = lowerbyte;

        index += 2;
    }
}

void Emulator::runNextCycle() {
    if (m_progCounter >= m_memory.end() - 1){
        m_endOfROM = true;
        return;
    }

    std::uint16_t opcode = ((*m_progCounter) << 8) | *(m_progCounter + 1);


    switch(byteAtIndex(opcode, 1)) {
    case 0:
        if (byteAtIndex(opcode, 3) == 0x0E && byteAtIndex(opcode, 4) == 0) {
            std::fill(m_displayBuffer.begin(), m_displayBuffer.end(), 0);
        }
        else if (byteAtIndex(opcode, 3) == 0x0E && byteAtIndex(opcode, 4) == 0x0E) {
            CallInfo info = m_callStack.top();
            m_progCounter = info.returnAddress;

            m_registers = info.registers;
            m_callStack.pop();

            return;
        }
        break;

    case 1:{
        int jumpaddress = byteAtIndex(opcode, 2, 4);
        m_progCounter = m_memory.begin() + (jumpaddress);

        return;
    }

    case 2:{
        CallInfo info{m_progCounter+2, m_registers};

        m_callStack.push(info);

        int calladdress = byteAtIndex(opcode, 2, 4);
        m_progCounter = m_memory.begin() + (calladdress);

        return;
    }

    case 3:
        if (m_registers[byteAtIndex(opcode, 2)] == byteAtIndex(opcode, 3, 4)) {
            m_progCounter += 4;
            return;
        }
        break;
    case 4:
        if (m_registers[byteAtIndex(opcode, 2)] != byteAtIndex(opcode, 3, 4)) {
            m_progCounter += 4;
            return;
        }
        break;

    case 5:
        if (m_registers[byteAtIndex(opcode, 2)] == m_registers[byteAtIndex(opcode, 3)]) {
            m_progCounter += 4;
            return;
        }
        break;

    case 6:
        m_registers[byteAtIndex(opcode, 2)] = byteAtIndex(opcode, 3, 4);
        break;

    case 7:
        m_registers[byteAtIndex(opcode, 2)] += byteAtIndex(opcode, 3, 4);
        break;

    case 8:{
        if (byteAtIndex(opcode, 4) == 0){
            m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 3)];
        }
        else if (byteAtIndex(opcode, 4) == 1){
            m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 2)] | m_registers[byteAtIndex(opcode, 3)];
        }
        else if (byteAtIndex(opcode, 4) == 2){
            m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 2)] & m_registers[byteAtIndex(opcode, 3)];
        }
        else if (byteAtIndex(opcode, 4) == 3){
            m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 2)] ^ m_registers[byteAtIndex(opcode, 3)];
        }
        else if (byteAtIndex(opcode, 4) == 4){            
            uint8_t vx = m_registers[byteAtIndex(opcode, 2)];
            uint8_t vy = m_registers[byteAtIndex(opcode, 3)];
            uint8_t result = vx + vy;

            //checking for overflow
            if (result < (int)vx + (int)vy)
                m_registers[15] = 1;
            else
                m_registers[15] = 0;

            m_registers[byteAtIndex(opcode, 2)] = result;
        }
        else if (byteAtIndex(opcode, 4) == 5){
            uint8_t vx = m_registers[byteAtIndex(opcode, 2)];
            uint8_t vy = m_registers[byteAtIndex(opcode, 3)];
            uint8_t result = vx - vy;

            //checking for overflow
            if (result > (int)vx - (int)vy)
                m_registers[15] = 0;
            else
                m_registers[15] = 1;

            m_registers[byteAtIndex(opcode, 2)] = result;
        }
        else if (byteAtIndex(opcode, 4) == 6){
            std::uint16_t leastbit = m_registers[byteAtIndex(opcode, 2)] & 1;
            m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 2)] >> 1;

            m_registers[15] = leastbit;
        }
        else if (byteAtIndex(opcode, 4) == 7){
            uint8_t vx = m_registers[byteAtIndex(opcode, 2)];
            uint8_t vy = m_registers[byteAtIndex(opcode, 3)];
            uint8_t result = vy - vx;

            //checking for overflow
            if (result < (int)vx - (int)vy)
                m_registers[15] = 0;
            else
                m_registers[15] = 1;

            m_registers[byteAtIndex(opcode, 2)] = result;
        }
        else if (byteAtIndex(opcode, 4) == 14){
            std::uint16_t mostsigbit = m_registers[byteAtIndex(opcode, 2)] >> 15;
            m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 2)] << 1;

            m_registers[15] = mostsigbit;
        }
        break;
    }

    case 9:
        if(m_registers[byteAtIndex(opcode, 2)] != m_registers[byteAtIndex(opcode, 3)]) {
            m_progCounter += 4;
            return;
        }
        break;

    case 10:
        m_registerI = byteAtIndex(opcode, 2, 4);
        break;

    case 11:{
        int jumpaddress = byteAtIndex(opcode, 2, 4);
        m_progCounter = m_memory.begin() + (jumpaddress + m_registers[0]);

        return;
    }

    case 12:
        m_registers[byteAtIndex(opcode, 2)] = (std::rand() % 255) & byteAtIndex(opcode, 3, 4);
        break;

    case 13:{
        int xpos = m_registers[byteAtIndex(opcode, 2)];
        int ypos = m_registers[byteAtIndex(opcode, 3)];
        int height = byteAtIndex(opcode, 4);

        auto spriteaddr = m_registerI;

        m_registers[15] = 0;

        for (int i = 0; i < height; i++) {
            int x = xpos;
            for (int j = 1; j <= 8; j++, x++) {
                uint8_t byte = m_memory[spriteaddr];
                uint8_t mask = 1 << (8 - j);

                byte = byte & mask;

                int bit = byte >> (8 - j);
                int index = x + (ypos * 64);

                //if anything is written beyond display buffer just ignore
                if (x >= 64 || ypos >= 32)
                    continue;

                if (m_displayBuffer[index] == 1 && bit == 1)
                    m_registers[15] = 1;

                m_displayBuffer[index] = m_displayBuffer[index] ^ bit;
            }

            ypos++;
            spriteaddr++;
        }
    }
    break;

    case 14:
        if (byteAtIndex(opcode, 3) == 9) {
            if (m_keyboardHandler.getKeyPress() == m_registers[byteAtIndex(opcode, 2)]) {
                m_progCounter += 4;
                return;
            }
        }
        else if (byteAtIndex(opcode, 3) == 0x0A) {
            if (m_keyboardHandler.getKeyPress() != m_registers[byteAtIndex(opcode, 2)]) {
                m_progCounter += 4;
                return;
            }
        }
        break;

    case 15:{
        if (byteAtIndex(opcode, 4) == 7) {
            auto end = std::chrono::high_resolution_clock::now();

            int delay = (end - m_timer).count();
            int timer = m_timerValue - (delay * 60);

            if (timer < 0)
                timer = 0;

            m_registers[byteAtIndex(opcode, 2)] = timer;
        }
        else if (byteAtIndex(opcode, 4) == 0x0A) {
            int key = m_keyboardHandler.getKeyPress();

            if (key == -1)
                return;
            else
                m_registers[byteAtIndex(opcode, 2)] = key;
        }
        else if (byteAtIndex(opcode, 3, 4) == 21) {
            m_timer = std::chrono::high_resolution_clock::now();
            m_timerValue = m_registers[byteAtIndex(opcode, 2)];
        }
        else if (byteAtIndex(opcode, 4) == 0x0E) {
            m_registerI += m_registers[byteAtIndex(opcode, 2)];
        }
        else if (byteAtIndex(opcode, 3, 4) == 85) {
            for(int i = 0; i <16; i++) {
                m_memory[m_registerI + i] = m_registers[i];
            }
        }
        else if (byteAtIndex(opcode, 3, 4) == 101) {
            for(int i = 0; i <16; i++) {
                m_registers[i] = m_memory[m_registerI + i];
            }
        }
    }
    break;

    default:
        std::cerr << "unimplemented opcode\n";
        break;
    }

    m_progCounter += 2;

    return;
}

std::vector<std::uint8_t>& Emulator::getRegisters() {
    return m_registers;
}

std::vector<std::uint8_t>& Emulator::getMemory() {
    return m_memory;
}

std::vector<std::uint8_t>::iterator Emulator::getProgramCounter() {
    return m_progCounter;
}

std::array<bool, 2048>& Emulator::getDisplayBuffer() {
    return m_displayBuffer;
}

bool Emulator::isEnd() {
    return m_endOfROM;
}

void Emulator::setKeyboardHandler() {

}

uint16_t byteAtIndex(std::uint16_t b, int i, int j) {
    std::uint16_t mask = 0;

    for (int num = 1; num <= 4; num++) {
        mask = mask << 4;

        if (j == 0) {
            if (num == i) {
                mask = mask | 0x0f;
            }
        }
        else if (num >= i && num <= j) {
            mask = mask | 0x0f;
        }
    }

    if (j == 0) {
        return (b & mask) >> (16 - (i * 4));
    }
    else {
        return b & mask;
    }
}

