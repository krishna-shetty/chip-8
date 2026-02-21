#ifndef CHIP8_DISPLAY_H
#define CHIP8_DISPLAY_H

#include <SDL3/SDL.h>
#include <array>

namespace chip8
{
    class Display
    {
        public:
            static constexpr int WIDTH = 64;
            static constexpr int HEIGHT = 32;

            static constexpr int FRAMERATE = 60;
            
            void clearScreen();
            void setPixel(int x, int y, bool value);
            bool getPixel(int x, int y);
            
            Display(uint32_t pixelOffColor = 0xF0F6F0FF, uint32_t pixelOnColor = 0x101010FF, int scale = 10);
            ~Display() noexcept;

            Display(const Display&) = delete;
            Display& operator=(const Display&) = delete;
            Display(Display&&) = delete;
            Display& operator=(Display&&) = delete;

            void clearBuffers();

            void draw();
        private:
            std::array<std::array<bool, WIDTH>, HEIGHT> _pixels{};
            std::array<uint32_t, WIDTH * HEIGHT> _pixelBuffer{};

            SDL_Window* _window = nullptr;
            SDL_Renderer* _renderer = nullptr;
            SDL_Texture* _texture = nullptr;

            int _scale = 10;

            uint32_t _pixelOffColor = 0xF0F6F0FF;
            uint32_t _pixelOnColor = 0x222323FF;

            void createWindow();
            void destroyWindow();

            void createTexture();
            void destroyTexture();
    };
} // namespace chip8
#endif // CHIP8_DISPLAY_H