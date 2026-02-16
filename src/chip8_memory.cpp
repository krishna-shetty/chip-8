#include "chip8_memory.h"

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