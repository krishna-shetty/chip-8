#include <chip8_display.h>

using namespace chip8;

Display::Display(uint32_t pixelOffColor, uint32_t pixelOnColor, int scale)
    : _scale(scale), 
    _pixelOffColor(pixelOffColor), 
    _pixelOnColor(pixelOnColor)
{
    createWindow();
    createTexture();
}

Display::~Display()
{
    destroyTexture();
    destroyWindow();
}

void Display::createWindow()
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        throw std::runtime_error(SDL_GetError());
    }

    if (!SDL_CreateWindowAndRenderer(
            "Chip-8",
            WIDTH * _scale,
            HEIGHT * _scale,
            0,
            &_window,
            &_renderer))
    {
        throw std::runtime_error(SDL_GetError());
    }
}

void Display::createTexture()
{
    if(_texture)
    {
        SDL_DestroyTexture(_texture);
    }

    _texture = SDL_CreateTexture(
        _renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT);

    if (_texture == nullptr)
    {
        destroyWindow();
        throw std::runtime_error(SDL_GetError());
    } 
}

void Display::destroyWindow()
{
    if (_renderer)
    {
        SDL_DestroyRenderer(_renderer);
        _renderer = nullptr;
    }
    if (_window)
    {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

void Display::destroyTexture()
{
    if (_texture)
    {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
}

void Display::setPixel(int x, int y, bool value)
{
    if (x >= 0 && x < WIDTH && y >= 0 && y < HEIGHT)
    {
        _pixels[y][x] = value;
    }
}

void Display::clearBuffers()
{
    for (auto &row: _pixels)
    {
        row.fill(false);
    }

    _pixelBuffer.fill(_pixelOffColor);
}

void Display::draw()
{
    for(int i = 0; i < HEIGHT; ++i)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            _pixelBuffer[i * WIDTH + j] = _pixels[i][j] ? _pixelOnColor : _pixelOffColor;
        }
    }

    void *lockedPixels = nullptr;
    int pitchBytes = 0;

    if(!SDL_LockTexture(_texture, nullptr, &lockedPixels, &pitchBytes))
    {
        throw std::runtime_error(SDL_GetError());
    }

    uint8_t* dst = static_cast<uint8_t*>(lockedPixels);
    uint8_t* src = reinterpret_cast<uint8_t*>(_pixelBuffer.data());

    const int rowBytes = WIDTH * static_cast<int>(sizeof(uint32_t));

    for (int i = 0; i < HEIGHT; ++i)
    {
        std::memcpy(dst + i * pitchBytes, src + i * rowBytes, rowBytes);
    }
    

    SDL_UnlockTexture(_texture);

    SDL_RenderClear(_renderer);

    SDL_FRect dstRect = 
    {
        0.0f, 
        0.0f, 
        static_cast<float>(WIDTH * _scale), 
        static_cast<float>(HEIGHT * _scale)
    };

    SDL_RenderTexture(_renderer, _texture, NULL, &dstRect);
    SDL_RenderPresent(_renderer);
}