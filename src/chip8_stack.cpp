#include "chip8_stack.h"

using namespace chip8;

void Stack::push(uint16_t value)
{
    if (_sp < _stack.size())
    {
        _stack[_sp++] = value;
    }
    else
    {
        throw std::overflow_error("Stack overflow");
    }
}

uint16_t Stack::pop()
{
    if (_sp > 0)
    {
        return _stack[--_sp];
    }
    else
    {
        throw std::underflow_error("Stack underflow");
    }
}