#include "chip8_cpu.h"

using namespace chip8;

uint16_t CPU::fetch()
{
    uint16_t high = _memory.read(_pc);
    uint16_t low  = _memory.read(_pc + 1);

    _pc += 2;

    return (high << 8) | low;
}

void decode(uint16_t op)
{
    uint8_t  x   = (op >> 8) & 0x0F;
    uint8_t  y   = (op >> 4) & 0x0F;
    uint8_t  n   =  op       & 0x0F;
    uint8_t  kk  =  op       & 0xFF;
    uint16_t nnn =  op       & 0x0FFF;
}