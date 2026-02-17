#ifndef CHIP_8_STACK_H
#define CHIP_8_STACK_H

#include <array>
#include <cstdint>

class Stack
{
    public:
        void push(uint16_t value);
        uint16_t pop();
    private:
        std::array<uint16_t, 16> _stack{0};
        uint8_t _sp = 0;
};

#endif // CHIP_8_STACK_H
