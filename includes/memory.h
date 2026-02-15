#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <array>
#include "fontset.h"

class Memory
{
    private:
        std::array<uint8_t, 0x1000> _data = {0}; 
};

#endif // MEMORY_H