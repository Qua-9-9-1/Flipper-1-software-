#include "BuzzerDriver.h"

BuzzerDriver::BuzzerDriver(uint8_t pin) : _pin(pin), _channel(0) {}

void BuzzerDriver::init() {
    pinMode(_pin, OUTPUT);
}

void BuzzerDriver::tone(uint16_t frequency, uint32_t duration) {
    ::tone(_pin, frequency);
}

void BuzzerDriver::noTone() {
    ::noTone(_pin);
}

void BuzzerDriver::playSound(SoundType sound) {
    switch (sound) {
        case SOUND_TICK:
            tone(2000, 20);
            break;
        case SOUND_OK:
            tone(1500, 50); 
            break;
        case SOUND_BACK:
            tone(800, 50);
            break;
        case SOUND_SUCCESS:
            tone(1200, 100); delay(100);
            tone(1600, 100); delay(100);
            tone(2000, 100);
            break;
        case SOUND_ERROR:
            tone(200, 300);
            break;
        default:
            break;
    }
}

uint32_t BuzzerDriver::getSoundDuration(SoundType sound) {
    switch (sound) {
        case SOUND_TICK:
            return 20;
        case SOUND_OK:
            return 50;
        case SOUND_BACK:
            return 50;
        case SOUND_SUCCESS:
            return 100;
        case SOUND_ERROR:
            return 300;
        default:
            return 0;
    }
}
