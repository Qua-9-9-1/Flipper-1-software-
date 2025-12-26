#include "DisplayDriver.hpp"

DisplayDriver::DisplayDriver(uint8_t sda_pin, uint8_t scl_pin) : _sda(sda_pin), _scl(scl_pin) {
    _u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, U8X8_PIN_NONE,
                                                    U8X8_PIN_NONE);
}

void DisplayDriver::init() {
    Wire.begin(_sda, _scl);
    _u8g2->begin();
    _u8g2->setFont(u8g2_font_ncenB08_tr);
    Wire.setClock(400000);  // set I2C frequency to 400kHz
}

U8G2_SSD1306_128X64_NONAME_F_HW_I2C* DisplayDriver::getU8g2() { return _u8g2; }