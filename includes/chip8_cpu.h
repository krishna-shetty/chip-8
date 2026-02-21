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

        CPU(float clockSpeed,
            chip8::Memory& memory,
            chip8::Display& display,
            chip8::Stack& stack,
            chip8::Keypad& keypad,
            chip8::Timer& soundTimer,
            chip8::Timer& delayTimer)
            : _memory(memory),
              _display(display),
              _stack(stack),
              _keypad(keypad),
              _soundTimer(soundTimer),
              _delayTimer(delayTimer),
              _clockSpeed(clockSpeed)
        {}

        void cycle();     
        float getClockSpeed() const;   

    private:
        chip8::Memory& _memory;
        chip8::Display& _display;
        chip8::Stack& _stack;
        chip8::Keypad& _keypad;
        chip8::Timer& _soundTimer;
        chip8::Timer& _delayTimer;

        uint16_t _pc = 0x200;
        uint16_t _I = 0;

        std::array<uint8_t, REGISTER_COUNT> _registers;

        float _clockSpeed = 1000000;
        uint16_t fetch();
        void decodeExecute(uint16_t);

        uint16_t getI() const;
        void setI(uint16_t value);
        void advancePC();
        void skipNext();
        void jump(uint16_t address);
        void setRegister(uint8_t reg, uint8_t value);
        uint8_t getRegister(uint8_t reg) const;
        void addToRegister(uint8_t reg, uint8_t value);
        void drawSprite(uint8_t x, uint8_t y, uint8_t height);
};

#endif // CHIP8_CPU_H