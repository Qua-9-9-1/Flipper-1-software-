#include "DisplayDriver.h"

DisplayDriver::DisplayDriver(uint8_t sda_pin, uint8_t scl_pin): _sda(sda_pin), _scl(scl_pin) {
    _u8g2 = new U8G2_SSD1306_128X64_NONAME_F_HW_I2C(U8G2_R0, U8X8_PIN_NONE, U8X8_PIN_NONE, U8X8_PIN_NONE);
}

void DisplayDriver::init() {
    Wire.begin(_sda, _scl);
    _u8g2->begin();
    _u8g2->setFont(u8g2_font_ncenB08_tr);
}

void DisplayDriver::showBootScreen() {
    _u8g2->clearBuffer();
    _u8g2->drawStr(10, 30, "FLIPPER -1");
    _u8g2->drawStr(20, 50, "Booting...");
    _u8g2->sendBuffer();
}

void DisplayDriver::updateStatus(int dataType, int dataValue) {
    _u8g2->clearBuffer();
    _u8g2->drawStr(0, 10, "Running...");
    
    _u8g2->setCursor(0, 30);
    _u8g2->print("Type: "); _u8g2->print(dataType);
    
    _u8g2->setCursor(0, 50);
    _u8g2->print("Val: "); _u8g2->print(dataValue);
    
    _u8g2->sendBuffer();
}