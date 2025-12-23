#pragma once
#include <Adafruit_NeoPixel.h>

class LedDriver {    
    public:
        LedDriver(uint8_t pin);
        void init();
        void setBrightness(uint8_t brightness);
        void tick(int currentMode, int batteryLevel);
        uint32_t getColor(uint8_t r, uint8_t g, uint8_t b);
    private:
        void LEDOff();
        void LEDBoot(unsigned long now);
        void LEDBattery(unsigned long now, int batteryLevel);
        void LEDScan(unsigned long now);
        void LEDAction(unsigned long now);
        void LEDManual(uint32_t color);

        Adafruit_NeoPixel* _pixels;
        uint8_t _pin;
        int _brightness; // 0-255
        unsigned long _lastUpdate;
        int _step;
        bool _fadeDirection;
};