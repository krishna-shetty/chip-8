#include <iostream>
#include "chip8_memory.h"

using namespace chip8;

int main() {
    Memory memory;
    uint16_t address = 0x050; 
    uint8_t value = memory.read(address);
    std::cout << "Value at address 0x" << std::hex << address << ": 0x" << std::hex << static_cast<int>(value) << std::endl;
    return 0;
}