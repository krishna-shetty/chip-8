#ifndef CHIP8_BEEPER_H
#define CHIP8_BEEPER_H

#include <SDL3/SDL.h>
#include <atomic>

namespace chip8
{
    class Oscillator
    {
    private:
        float currentStep;
        float stepSize;
        float volume;

    public:
        Oscillator()
            : currentStep(0.0f),
              stepSize(0.0f),
              volume(1.0f)
        {
        }

        Oscillator(float rate, float volume = 1.0f)
            : currentStep(0.0f),
              stepSize((2.0f * SDL_PI_F) / rate),
              volume(volume)
        {
        }

        float nextSample()
        {
            currentStep += stepSize;

            return volume * SDL_sinf(currentStep);
        }

        void configure(float samplesPerCycle, float volume = 1.0f)
        {
            this->volume = volume;
            stepSize = (2 * M_PI) / samplesPerCycle;
        }
    };

    class Beeper
    {
    public:
        static constexpr int SAMPLE_RATE = 44100;
        static constexpr int BUFFER_SIZE = 4096;

        bool initalize();
        void play();
        void stop();
        void update();
        void shutdown();

        Beeper(float frequency = 440.0f, float volume = 0.5f);
        ~Beeper() noexcept;

        Beeper(const Beeper &) = delete;
        Beeper &operator=(const Beeper &) = delete;
        Beeper(Beeper &&) = delete;
        Beeper &operator=(Beeper &&) = delete;

    private:
        std::atomic<bool> _isPlaying{false};
        Oscillator _oscillator;
        SDL_AudioSpec _configuredSpec{};
        SDL_AudioStream *_stream = nullptr;

        float _frequency = 440.0f; // Default frequency (A4)
        float _volume = 0.5f;      // Default volume

        bool isPlaying() const;
    };
} // namespace chip8
#endif // CHIP8_BEEPER_H
