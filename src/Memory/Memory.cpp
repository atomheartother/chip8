#include <bits/stdint-uintn.h>
#include <iostream>
#include <fstream>
#include "Memory.hh"

// Total size of RAM for the emulator
const unsigned short VIRT_RAM_SIZE = 0xFFFF;
// Interpreter took up the first 0x200 bytes of memory
// We don't have to do this, but this also means we can free up 0x200 bytes from our alloc.
const unsigned short PROGRAM_OFFSET = 0x200;
// Physical size of ram
const unsigned short RAM_SIZE = VIRT_RAM_SIZE - PROGRAM_OFFSET;

Memory::Memory() {
    _ram = new uint8_t[RAM_SIZE];
}

Memory::~Memory() {
    delete[] _ram;
}

int Memory::Load(const char* filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (!in) {
        std::cout << "File not found: " << filename << std::endl;
        return 2;
    }

    in.seekg(0, in.end);
    const int length = in.tellg();
    in.seekg(0, in.beg);

    if (length > RAM_SIZE) {
        std::cout << "Program " << filename << " is too large for RAM: " << length << "B" << std::endl;
        return 3;
    }

    in.read(reinterpret_cast<char*>(_ram), length);
    if (!in) {
        std::cout << "Couldn't load program, only read " << in.gcount() << "B" << std::endl;
        return 4;
    }
    return 0;
}