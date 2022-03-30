#include "emulator.hpp"
#include <fstream>

Emulator::Emulator()
{
    m_memory.reserve(4096);
    m_registers.resize(16);
}

bool Emulator::loadROM(std::string filename) {
    std::ifstream file(filename, std::fstream::binary);

    if (!file.is_open()) {
        return false;
    }

    std::uint8_t higherbyte, lowerbyte;

    while(file >> higherbyte) {
        file >> lowerbyte;

        std::uint16_t opcode = (higherbyte << 8) | lowerbyte;
        m_memory.push_back(opcode);
    }

    return true;
}

void Emulator::runROM() {
    auto progCounter = m_memory.begin();

    while(true) {
        std::uint16_t opcode = *progCounter;

        switch(byteAtIndex(opcode, 1)) {
        case 0:
            break;

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
        case 10:
            m_registerI = byteAtIndex(opcode, 2, 4);
            break;

        default:
            break;
        }

        progCounter++;
    }
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

