#include "LedDriver.hpp"

LedDriver::LedDriver(uint8_t pin)
    : _pin(pin),
      _brightness(50),
      _lastUpdate(0),
      _color({0, 0, 0}),
      _fadeSpeed(2),
      _step(0),
      _endTimeout(0),
      _fadeDirection{true} {
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

void LedDriver::setColor(uint8_t r, uint8_t g, uint8_t b) {
    _color[0] = r;
    _color[1] = g;
    _color[2] = b;
}

void LedDriver::setFadeSpeed(uint8_t speed) { _fadeSpeed = speed; }

void LedDriver::startTimeout(int durationMs) {
    _endTimeout = millis() + durationMs;
    _pixels->setPixelColor(0, _pixels->Color(_color[0], _color[1], _color[2]));
    _pixels->show();
}
void LedDriver::tick(LedMode mode, int batteryLevel) {
    unsigned long now = millis();

    switch (mode) {
        case LED_MODE_ON:
            _pixels->setPixelColor(0, _pixels->Color(_color[0], _color[1], _color[2]));
            break;
        case LED_MODE_OFF:
            _pixels->clear();
            break;
        case LED_MODE_FADE:
            LEDFade(now);
            break;
        case LED_MODE_BLINK:
            LEDBlink(now);
            break;
        case LED_MODE_BOOT:
            LEDBoot(now);
            break;
        case LED_MODE_BATTERY:
            LEDBattery(now, batteryLevel);
            break;
        case LED_MODE_TIMEOUT:
            LEDTimeout(now);
            break;
        default:
            break;
    }
    _pixels->show();
}

uint32_t LedDriver::getColor() { return _pixels->Color(_color[0], _color[1], _color[2]); }

void LedDriver::LEDBoot(unsigned long now) {
    if (now - _lastUpdate > 10) {
        _lastUpdate = now;
        _step += _fadeDirection ? 2 : -2;
        if (_step >= 255) _fadeDirection = false;
        if (_step <= 0) _fadeDirection = true;
        _pixels->setPixelColor(0, _pixels->Color(_step / 2, _step, 0));
    }
}

void LedDriver::LEDBattery(unsigned long now, int batteryLevel) {
    if (batteryLevel > 75) {
        setColor(0, 255, 255);
    } else if (batteryLevel > 50) {
        setColor(0, 255, 0);
    } else if (batteryLevel > 25) {
        setColor(255, 165, 0);
    } else if (batteryLevel > 5) {
        setColor(255, 40, 0);
    } else {
        setColor(255, 40, 0);
        LEDFade(now);
    }
    if (batteryLevel > 5)
        _pixels->setPixelColor(0, _pixels->Color(_color[0], _color[1], _color[2]));
}

void LedDriver::LEDBlink(unsigned long now) {
    if (now - _lastUpdate > 500) {
        _lastUpdate = now;
        _step       = !_step;
        if (_step) {
            _pixels->setPixelColor(0, _pixels->Color(_color[0], _color[1], _color[2]));
        } else {
            _pixels->clear();
        }
    }
}

void LedDriver::LEDFade(unsigned long now) {
    if (now - _lastUpdate > 10) {
        _lastUpdate = now;
        _step += _fadeDirection ? _fadeSpeed : -_fadeSpeed;
        if (_step >= 255) {
            _fadeDirection = false;
            _step          = 255;
        } else if (_step <= 0) {
            _fadeDirection = true;
            _step          = 0;
        }
        uint8_t r = (_color[0] * _step) / 255;
        uint8_t g = (_color[1] * _step) / 255;
        uint8_t b = (_color[2] * _step) / 255;
        _pixels->setPixelColor(0, _pixels->Color(r, g, b));
    }
}

void LedDriver::LEDTimeout(unsigned long now) {
    if (now >= _endTimeout) {
        _pixels->clear();
    } else {
        _pixels->setPixelColor(0, _pixels->Color(_color[0], _color[1], _color[2]));
    }
}