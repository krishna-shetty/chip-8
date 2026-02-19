#include "chip8_beeper.h"
#include <stdexcept>
#include <algorithm>

using namespace chip8;

bool Beeper::initalize()
{ 
    _configuredSpec.format = SDL_AUDIO_F32;
    _configuredSpec.channels = 1;
    _configuredSpec.freq = SAMPLE_RATE;

    _stream = SDL_OpenAudioDeviceStream(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, &_configuredSpec, nullptr, nullptr);

    if (_stream == nullptr)
    {
        SDL_Log("SDL_OpenAudioDeviceStream failed: %s", SDL_GetError());
        return false;
    }

    SDL_ResumeAudioStreamDevice(_stream);

    float samplesPerCycle = (float) SAMPLE_RATE / _frequency;
    _oscillator.configure(samplesPerCycle, _volume);

    _isPlaying.store(false, std::memory_order_release);

    return true;
}

void Beeper::shutdown()
{
    if(!_stream) return;

    SDL_DestroyAudioStream(_stream);

    _stream = nullptr;
    _isPlaying.store(false, std::memory_order_release);
}

void Beeper::play()
{
    _isPlaying.store(true, std::memory_order_release);
}

void Beeper::stop()
{
    _isPlaying.store(false, std::memory_order_release);

    if (_stream)
    {
        SDL_ClearAudioStream(_stream);
    }
}

bool Beeper::isPlaying() const
{
    return _isPlaying.load(std::memory_order_acquire);
}

void Beeper::update()
{
    if (!_stream) return;

    // Aim to keep about 50ms of audio buffered to prevent under-runs
    const int queuedBytes = SDL_GetAudioStreamQueued(_stream);
    const int bytesPerFrame = (int)sizeof(float) * _configuredSpec.channels;
    const int targetFrames = (int)(_configuredSpec.freq * 0.05f);
    const int targetBytes  = targetFrames * bytesPerFrame;

    if (queuedBytes >= targetBytes)
        return;

    float buffer[BUFFER_SIZE];

    if(isPlaying())
    {
        for(int i = 0; i < BUFFER_SIZE; ++i)
        {
            buffer[i] = _oscillator.nextSample();
        } 
    }
    else
    {
            std::fill(std::begin(buffer), std::end(buffer), 0.0f);
    }

    SDL_PutAudioStreamData(_stream, buffer, sizeof(buffer));
}

Beeper::Beeper(float frequency, float volume)
    : _frequency(frequency), _volume(volume)
{
    if (!initalize())
    {
        throw std::runtime_error("Failed to initialize Beeper");
    }
}

Beeper::~Beeper() noexcept
{
    shutdown();
}