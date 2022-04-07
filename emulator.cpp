#include "emulator.hpp"
#include <fstream>
#include <sstream>
#include <ctime>

Emulator::Emulator()
{
    m_memory.resize(4096);
    m_registers.resize(16);
    m_displayBuffer.fill(0);

    std::srand(std::time(nullptr));
}

bool Emulator::loadROM(std::string filename) {
    std::ifstream file(filename, std::fstream::binary);

    if (!file.is_open()) {
        return false;
    }

    std::uint8_t higherbyte, lowerbyte;

    for(int index = 512; file >> higherbyte; index++) {
        file >> lowerbyte;

        m_memory[index] = higherbyte;
        m_memory[index + 1] = lowerbyte;

        index += 2;
    }

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

void Emulator::runROM() {
    auto progCounter = m_memory.begin() + 512;

    while(progCounter < m_memory.end() - 1) {
        std::uint16_t opcode = ((*progCounter) << 8) | *(progCounter + 1);

        switch(byteAtIndex(opcode, 1)) {
        case 0:
            if (byteAtIndex(opcode, 3) == 0x0E && byteAtIndex(opcode, 4) == 0) {
                std::fill(m_displayBuffer.begin(), m_displayBuffer.end(), 0);
            }
            break;

        case 1:{
            int jumpaddress = byteAtIndex(opcode, 2, 4);
            progCounter = m_memory.begin() + (jumpaddress - 1);
            break;
        }

        case 3:
            if (m_registers[byteAtIndex(opcode, 2)] == byteAtIndex(opcode, 3, 4)) {
               progCounter++;
            }
            break;

        case 4:
            if (m_registers[byteAtIndex(opcode, 2)] != byteAtIndex(opcode, 3, 4)) {
               progCounter++;
            }
            break;

        case 5:
            if (m_registers[byteAtIndex(opcode, 2)] == m_registers[byteAtIndex(opcode, 3)]) {
               progCounter++;
            }
            break;

        case 6:
            m_registers[byteAtIndex(opcode, 2)] = byteAtIndex(opcode, 3, 4);
            break;

        case 7:
            m_registers[byteAtIndex(opcode, 2)] += byteAtIndex(opcode, 3, 4);
            break;

        case 8:{
            if (m_registers[byteAtIndex(opcode, 4)] == 0){
                m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 3)];
            }
            else if (m_registers[byteAtIndex(opcode, 4)] == 1){
                m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 2)] | m_registers[byteAtIndex(opcode, 3)];
            }
            else if (m_registers[byteAtIndex(opcode, 4)] == 2){
                m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 2)] & m_registers[byteAtIndex(opcode, 3)];
            }
            else if (m_registers[byteAtIndex(opcode, 4)] == 3){
                m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 2)] ^ m_registers[byteAtIndex(opcode, 3)];
            }
            else if (m_registers[byteAtIndex(opcode, 4)] == 4){
                m_registers[byteAtIndex(opcode, 2)] += m_registers[byteAtIndex(opcode, 3)];
            }
            else if (m_registers[byteAtIndex(opcode, 4)] == 5){
                m_registers[byteAtIndex(opcode, 2)] -= m_registers[byteAtIndex(opcode, 3)];
            }
            else if (m_registers[byteAtIndex(opcode, 4)] == 6){
                std::uint16_t leastbit = m_registers[byteAtIndex(opcode, 2)] << 15;
                m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 2)] >> 1;

                m_registers[15] = leastbit;
            }
            else if (m_registers[byteAtIndex(opcode, 4)] == 7){
                m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 3)] - m_registers[byteAtIndex(opcode, 2)];
            }
            else if (m_registers[byteAtIndex(opcode, 4)] == 14){
                std::uint16_t mostsigbit = m_registers[byteAtIndex(opcode, 2)] >> 15;
                m_registers[byteAtIndex(opcode, 2)] = m_registers[byteAtIndex(opcode, 2)] << 1;

                m_registers[15] = mostsigbit;
            }
            break;
        }

        case 9:
            if(m_registers[byteAtIndex(opcode, 2)] != m_registers[byteAtIndex(opcode, 3)]) {
                progCounter++;
            }
            break;

        case 10:
            m_registerI = byteAtIndex(opcode, 2, 4);
            break;

        case 12:
            m_registers[byteAtIndex(opcode, 2)] = (std::rand() % 255) & byteAtIndex(opcode, 3, 4);
            break;

        case 15:{
            if (byteAtIndex(opcode, 4) == 0x0E) {
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
            break;
        }

        progCounter++;
    }
}

std::vector<std::uint16_t>& Emulator::getRegisters() {
    return m_registers;
}

std::vector<std::uint8_t>& Emulator::getMemory() {
    return m_memory;
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

