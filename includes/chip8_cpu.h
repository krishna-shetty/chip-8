#ifndef CHIP8_CPU_H
#define CHIP8_CPU_H

#include "chip8_memory.h"
#include "chip8_display.h"
#include "chip8_stack.h"
#include "chip8_timer.h"
#include "chip8_keypad.h"

#include <array>


class CPU
{
    public:
        static constexpr uint8_t REGISTER_COUNT = 16;

        CPU(chip8::Memory& memory,
            chip8::Display& display,
            chip8::Keypad& keypad,
            chip8::Timer& soundTimer,
            chip8::Timer& delayTimer)
            : _memory(memory),
              _display(display),
              _keypad(keypad),
              _soundTimer(soundTimer),
              _delayTimer(delayTimer)
        {}

        void cycle();

    private:
        chip8::Memory& _memory;
        chip8::Display& _display;
        chip8::Keypad& _keypad;
        chip8::Timer& _soundTimer;
        chip8::Timer& _delayTimer;

        uint16_t _pc;

        std::array<uint8_t, REGISTER_COUNT> _registers;

        uint16_t fetch();
        void decodeExecute(uint16_t);
};

#endif // CHIP8_CPU_H

