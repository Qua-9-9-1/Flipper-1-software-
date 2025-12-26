#include "PageBoot.hpp"

PageBoot::PageBoot() : _progress(0), _lastUpdate(0) {}

void PageBoot::onEnter() { _progress = 0; }

void PageBoot::draw(U8G2* u8g2) {
    // Loading simulation
    if (millis() - _lastUpdate > 50) {
        _progress++;
        _lastUpdate = millis();
        if (_progress > 100) {
            // transition to main menu
        }
    }

    u8g2->clearBuffer();
    u8g2->drawStr(30, 30, "FLIPPER -1");
    u8g2->drawFrame(14, 40, 100, 10);
    u8g2->drawBox(16, 42, _progress, 6);
}