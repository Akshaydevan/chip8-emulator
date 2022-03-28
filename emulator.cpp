#include "emulator.hpp"
#include <fstream>

Emulator::Emulator()
{
    m_memory.reserve(4096);
}

bool Emulator::loadROM(std::string filename) {
    std::ifstream file(filename, std::fstream::binary);

    if (!file.is_open()) {
        std::cerr << "cannot open file";
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
    for(auto i = m_memory.begin(); i != m_memory.end(); i++) {
        std::uint16_t opcode = *i;

        switch(byteIndex(opcode, 1)) {

        }
    }
}


uint16_t byteIndex(std::uint16_t b, int i) {
    int pop_size = 16 - (i *4);

    return b >> pop_size;
}

