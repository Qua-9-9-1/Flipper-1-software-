#include "PageBoot.h"

PageBoot::PageBoot() : progress(0), lastUpdate(0) {}

void PageBoot::onEnter() {
    progress = 0;
}

void PageBoot::draw(U8G2 *u8g2) {
    // Loading simulation
    if (millis() - lastUpdate > 50) { 
        progress++;
        lastUpdate = millis();
        if (progress > 100) {
            // transition to main menu
        }
    }
    
    u8g2->clearBuffer();
    u8g2->drawStr(30, 30, "FLIPPER -1");
    u8g2->drawFrame(14, 40, 100, 10);
    u8g2->drawBox(16, 42, progress, 6);
    u8g2->sendBuffer();
}