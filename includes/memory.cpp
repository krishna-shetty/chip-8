#include "memory.h"

Memory::Memory()
{
    // Load the fontset into memory starting at address 0x050
    for (size_t i = 0; i < FONTSET.size(); ++i)
    {
        _data[0x050 + i] = FONTSET[i];
    }
}