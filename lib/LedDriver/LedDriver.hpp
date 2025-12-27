#pragma once
#include <Adafruit_NeoPixel.h>

#include "AppEvents.h"

class LedDriver {
   public:
    LedDriver(uint8_t pin);
    void     init();
    void     setBrightness(uint8_t brightness);
    void     setColor(uint8_t r, uint8_t g, uint8_t b);
    void     startTimeout(int ms);
    void     tick(LedMode mode, int batteryLevel);
    uint32_t getColor();

   private:
    void LEDBoot(unsigned long now);
    void LEDBattery(unsigned long now, int batteryLevel);
    void LEDBlink(unsigned long now);
    void LEDFade(unsigned long now);
    void LEDTimeout(unsigned long now);

    Adafruit_NeoPixel* _pixels;
    uint8_t            _pin;
    int                _brightness;  // 0-255
    uint8_t            _color[3];
    unsigned long      _lastUpdate;
    int                _endTimeout;
    int                _step;
    bool               _fadeDirection;
};