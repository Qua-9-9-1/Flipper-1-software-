#pragma once
#include <Arduino.h>
#include "AppEvents.h"

class BuzzerDriver {
    public:
        BuzzerDriver(uint8_t pin);
        void init();
        void tone(uint16_t frequency, uint32_t duration);
        void noTone();
        void playSound(SoundType sound);
        uint32_t getSoundDuration(SoundType sound);
    private:
        uint8_t _pin;
        uint8_t _channel;
};