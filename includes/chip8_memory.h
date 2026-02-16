#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#include <array>
#include "fontset.h"

namespace chip8
{
    class Memory
    {
        public:
            Memory();
            uint8_t read(std::uint16_t address) const;
            void write(std::uint16_t address, uint8_t value);
        private:
            std::array<uint8_t, 0x1000> _data = {0}; 
    };
} // namespace chip8

#endif // MEMORY_H