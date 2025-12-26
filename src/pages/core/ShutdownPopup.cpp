#include "ShutdownPopup.hpp"

ShutdownPopup::ShutdownPopup() : _isVisible(false), _cursor(1) {}

void ShutdownPopup::show() {
    _isVisible = true;
    _cursor    = 1;
}

void ShutdownPopup::hide() { _isVisible = false; }

bool ShutdownPopup::active() { return _isVisible; }

void ShutdownPopup::draw(U8G2* u8g2) {
    if (!_isVisible) return;
    u8g2->setDrawColor(0);
    u8g2->drawBox(6, 10, 116, 44);
    u8g2->setDrawColor(1);
    u8g2->drawFrame(6, 10, 116, 44);
    u8g2->drawStr(14, 28, "Shutdown ?");
    u8g2->drawXBMP(110 - SYSTEM_ICON_DIM, 14, SYSTEM_ICON_DIM, SYSTEM_ICON_DIM, icon_poweroff_bits);
    u8g2->drawStr(10, 48, _cursor == 0 ? "> OK" : "  OK");
    u8g2->drawStr(70, 48, _cursor == 1 ? "> Cancel" : "  Cancel");
}

bool ShutdownPopup::onEvent(AppEvent* event) {
    if (!_isVisible) return false;
    if (event->type == EVENT_BUTTON_CLICK) {
        switch (event->value) {
            case BTN_LEFT:
            case BTN_RIGHT:
                _cursor = !_cursor;
                return true;
            case BTN_BACK:
                hide();
                return true;
            case BTN_OK:
                if (_cursor == 0) {
                    esp_deep_sleep_start();
                } else {
                    hide();
                }
                return true;
        }
    }
    return true;
}