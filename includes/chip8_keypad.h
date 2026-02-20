#ifndef CHIP8_KEYPAD_H
#define CHIP8_KEYPAD_H

#include <SDL3/SDL.h>
#include <array>
#include <cstdint>

namespace chip8
{
    class Keypad
    {
    public:
        static constexpr int KEY_COUNT = 16;
        static constexpr int NO_KEY = -1;

        void handleEvent(const SDL_Event& e)
        {
            if (e.type != SDL_EVENT_KEY_DOWN && e.type != SDL_EVENT_KEY_UP)
                return;

            const int k = mapKeyToChip8(e.key.key); 
            if (k == NO_KEY)
                return;

            _keys[(size_t)k] = (e.type == SDL_EVENT_KEY_DOWN) ? 1 : 0;

            if (e.type == SDL_EVENT_KEY_DOWN)
            {
                _lastPressed = k;
                _hasLastPressed = true;
            }
        }

        bool isDown(uint8_t chip8Key) const
        {
            return chip8Key < KEY_COUNT && _keys[chip8Key] != 0;
        }

        bool popLastPressed(uint8_t& outKey)
        {
            if (!_hasLastPressed) return false;
            outKey = (uint8_t)_lastPressed;
            _hasLastPressed = false;
            return true;
        }

        void clear()
        {
            _keys.fill(0);
            _hasLastPressed = false;
            _lastPressed = NO_KEY;
        }

    private:
        static int mapKeyToChip8(SDL_Keycode key)
        {
            switch (key)
            {
                case SDLK_1: return 0x0;
                case SDLK_2: return 0x1;
                case SDLK_3: return 0x2;
                case SDLK_4: return 0x3;

                case SDLK_Q: return 0x4;
                case SDLK_W: return 0x5;
                case SDLK_E: return 0x6;
                case SDLK_R: return 0x7;

                case SDLK_A: return 0x8;
                case SDLK_S: return 0x9;
                case SDLK_D: return 0xA;
                case SDLK_F: return 0xB;

                case SDLK_Z: return 0xC;
                case SDLK_X: return 0xD;
                case SDLK_C: return 0xE;
                case SDLK_V: return 0xF;

                default:     return NO_KEY;
            }
        }

    private:
        std::array<bool, KEY_COUNT> _keys{}; 
        int  _lastPressed = NO_KEY;
        bool _hasLastPressed = false;
    };
}

#endif // CHIP8_KEYPAD_H
