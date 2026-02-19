#include "chip8_timer.h"

using namespace chip8;

void Timer::set(uint8_t value)
{
    _value = value;
}

void Timer::decrement()
{
    if (_value > 0)
    {
        --_value;
    }
}