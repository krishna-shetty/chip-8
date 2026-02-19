#ifndef CHIP8_TIMER_H
#define CHIP8_TIMER_H

#include <cstdint>

namespace chip8
{
    class Timer
    {
        private:
            uint8_t _value;

        public:
            Timer() : _value(0) {}

            void set(uint8_t value);

            uint8_t get() const { return _value; }
            
            void decrement();
    };
} // namespace chip8
#endif // CHIP8_TIMER_H