#include "PageError.hpp"

PageError::PageError() : errorMessage("Unknown Error"), selected(0) {}

void PageError::setError(const char* msg) { errorMessage = msg; }

void PageError::onEnter() {
    setLedMode(LED_MODE_ON);
    setLedColor(255, 75, 0);
}

void PageError::onEvent(AppEvent* event) {
    if (event->type == EVENT_BUTTON_CLICK) {
        switch (event->value) {
            case BTN_UP:
            case BTN_DOWN:
                selected = !selected;
                break;
            case BTN_OK:
                if (selected == 0)
                    ESP.restart();
                else
                    esp_deep_sleep_start();
                break;
        }
    }
}

void PageError::draw(U8G2* u8g2) {
    u8g2->clearBuffer();

    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, 128, 16);
    u8g2->setDrawColor(0);
    u8g2->drawStr(40, 12, "SYSTEM ERROR");
    u8g2->setDrawColor(1);
    u8g2->drawStr(5, 30, errorMessage);
    u8g2->drawStr(10, 55, selected == 0 ? "> Reboot" : "  Reboot");
    u8g2->drawStr(70, 55, selected == 1 ? "> PowerOff" : "  PowerOff");
}