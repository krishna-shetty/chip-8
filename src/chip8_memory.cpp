#include "chip8_memory.h"
#include "fstream"
#include "string"

using namespace chip8;

Memory::Memory()
{
    uint16_t fontset_start_address = 0x050;

    for (size_t i = 0; i < FONTSET.size(); ++i)
    {
        _data[fontset_start_address + i] = FONTSET[i];
    }
}

void Memory::write(std::uint16_t address, uint8_t value)
{
    if (address < _data.size())
    {
        _data[address] = value;
    }
    else
    {
        throw std::out_of_range("Memory address out of range");
    }
}

std::uint8_t Memory::read(std::uint16_t address) const
{
    if (address < _data.size())
    {
        return _data[address];
    }
    else
    {
        throw std::out_of_range("Memory address out of range");
    }
}

/*
 * Title: Building a CHIP-* Emulator [C++]
 * Author: Austin Morlan
 * Date: Feb 20, 2026
 * Source: https://austinmorlan.com/posts/chip8_emulator/
 */
void Memory::loadROM(uint16_t startAddress = 0x200, char const* filename)
{
    std::ifstream rom(filename, std::ios::binary | std::ios::ate);

    if (!rom.is_open())
    {
        throw std::runtime_error(std::string("Error opening file: ") + filename);
    }

    std::streampos size = rom.tellg();
    char* buffer;

    rom.seekg(0, std::ios::beg);
    rom.read(buffer, size);
    rom.close();

    for (long i = 0; i < size; ++i)
    {
        _data[startAddress + i] = buffer[i];
    }

    delete[] buffer;
}