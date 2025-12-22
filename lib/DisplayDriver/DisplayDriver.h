#pragma once
#include <U8g2lib.h>
#include <Wire.h>

enum ErrorCode {
    NO_PAGE,
    CRASH,
    OTHER
};

class DisplayDriver {
public:
    DisplayDriver(uint8_t sda_pin, uint8_t scl_pin);
    void init();
    void showError(ErrorCode code);
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C* getU8g2();
    
private:
    uint8_t _sda;
    uint8_t _scl;
    U8G2_SSD1306_128X64_NONAME_F_HW_I2C* _u8g2; 
};