#ifndef CHIP8_MEMORY_H
#define CHIP8_MEMORY_H

#include <stdint.h>
#include <array>
#include "fontset.h"

namespace chip8
{
    class Memory
    {
        public:
            const static uint16_t FONTSET_START_ADDRESS = 0x050;
            Memory();
            uint8_t read(std::uint16_t address) const;
            void write(std::uint16_t address, uint8_t value);
            void loadROM(char const *filename, uint16_t address = 0x200);
        private:
            std::array<uint8_t, 0x1000> _data = {0}; 
    };
} // namespace chip8

#endif // CHIP8_MEMORY_H