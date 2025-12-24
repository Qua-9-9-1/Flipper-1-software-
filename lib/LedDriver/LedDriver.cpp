#include "LedDriver.hpp"

LedDriver::LedDriver(uint8_t pin)
    : _pin(pin), _brightness(50), _lastUpdate(0), _step(0), _fadeDirection(true) {
    // LED mode GRB + 800KHz
    _pixels = new Adafruit_NeoPixel(1, pin, NEO_GRB + NEO_KHZ800);
}

void LedDriver::init() {
    _pixels->begin();
    _pixels->setBrightness(_brightness);
    _pixels->show();
}

void LedDriver::setBrightness(uint8_t brightness) {
    _brightness = brightness;
    _pixels->setBrightness(_brightness);
    _pixels->show();
}

void LedDriver::tick(int currentMode, int batteryLevel) {
    unsigned long now = millis();

    switch (currentMode) {
        case 0:
            LEDOff();
            break;
        case 1:
            LEDBoot(now);
            break;
        case 2:
            LEDBattery(now, batteryLevel);
            break;
        case 3:
            LEDScan(now);
            break;
        case 4:
            LEDAction(now);
            break;
        case 5:
            LEDManual(getColor(255, 255, 255));
            break;
        default:
            LEDOff();
            break;
    }
}

uint32_t LedDriver::getColor(uint8_t r, uint8_t g, uint8_t b) { return _pixels->Color(r, g, b); }

void LedDriver::LEDOff() {
    _pixels->clear();
    _pixels->show();
}

void LedDriver::LEDBoot(unsigned long now) {
    if (now - _lastUpdate > 10) {
        _lastUpdate = now;
        _step += _fadeDirection ? 2 : -2;
        if (_step >= 255) _fadeDirection = false;
        if (_step <= 0) _fadeDirection = true;
        _pixels->setPixelColor(0, _pixels->Color(_step / 2, _step, 0));
        _pixels->show();
    }
}

void LedDriver::LEDBattery(unsigned long now, int batteryLevel) {
    if (batteryLevel > 75) {
        _pixels->setPixelColor(0, _pixels->Color(0, 0, 255));
    } else if (batteryLevel > 50) {
        _pixels->setPixelColor(0, _pixels->Color(0, 255, 0));
    } else if (batteryLevel > 25) {
        _pixels->setPixelColor(0, _pixels->Color(255, 165, 0));
    } else if (batteryLevel > 5) {
        _pixels->setPixelColor(0, _pixels->Color(255, 0, 0));
    } else {
        if (now - _lastUpdate > 20) {
            _lastUpdate = now;
            _step += _fadeDirection ? 10 : -10;
            if (_step >= 255) _fadeDirection = false;
            if (_step <= 0) _fadeDirection = true;
            _pixels->setPixelColor(0, _pixels->Color(_step, 0, 0));
        }
    }
    _pixels->show();
}

void LedDriver::LEDScan(unsigned long now) {
    int val = 0;

    if (now - _lastUpdate > 50) {
        _lastUpdate = now;
        val         = random(50, 255);
        _pixels->setPixelColor(0, _pixels->Color(0, val, val));
        _pixels->show();
    }
}

void LedDriver::LEDAction(unsigned long now) {
    if (now - _lastUpdate > 100) {
        _lastUpdate = now;
        _step       = !_step;
        if (_step)
            _pixels->setPixelColor(0, _pixels->Color(255, 0, 0));
        else
            _pixels->clear();
        _pixels->show();
    }
}

void LedDriver::LEDManual(uint32_t color) {
    _pixels->setPixelColor(0, color);
    _pixels->show();
}
