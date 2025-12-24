#include "PageControlTest.hpp"

PageControlTest::PageControlTest() : _konamiIndex(0) {}

void PageControlTest::onEnter() { _konamiIndex = 0; }

void PageControlTest::onEvent(AppEvent* event) {
    if (event->type == EVENT_BUTTON_DOWN) {
        switch (event->value) {
            case BTN_UP:
                _isUpPressed = true;
                break;
            case BTN_OK:
                _isOkPressed = true;
                break;
            case BTN_DOWN:
                _isDownPressed = true;
                break;
            case BTN_LEFT:
                _isLeftPressed = true;
                break;
            case BTN_RIGHT:
                _isRightPressed = true;
                break;
            case BTN_BACK:
                _isBackPressed = true;
                break;
        }
    } else if (event->type == EVENT_BUTTON_UP) {
        switch (event->value) {
            case BTN_UP:
                _isUpPressed = false;
                break;
            case BTN_OK:
                _isOkPressed = false;
                break;
            case BTN_DOWN:
                _isDownPressed = false;
                break;
            case BTN_LEFT:
                _isLeftPressed = false;
                break;
            case BTN_RIGHT:
                _isRightPressed = false;
                break;
            case BTN_BACK:
                _isBackPressed = false;
                break;
        }
    }

    if (event->type == EVENT_BUTTON_CLICK) {
        checkKonami((ButtonID)event->value);
    }
}

void PageControlTest::checkKonami(ButtonID input) {
    if (input == _konamiSequence[_konamiIndex]) {
        _konamiIndex++;
        if (_konamiIndex == 10) {
            PageManager::getInstance()->popPage();
        }
    } else {
        _konamiIndex = 0;
    }
}

void PageControlTest::draw(U8G2* u8g2) {
    u8g2->clearBuffer();

    u8g2->drawXBMP(26, 1, ARROW_WIDTH, ARROW_HEIGHT,
                   _isUpPressed ? arrow_up_clicked_bits : arrow_up_bits);
    u8g2->drawXBMP(26, 37, ARROW_WIDTH, ARROW_HEIGHT,
                   _isDownPressed ? arrow_down_clicked_bits : arrow_down_bits);
    u8g2->drawXBMP(12, 15, ARROW_HEIGHT, ARROW_WIDTH,
                   _isLeftPressed ? arrow_left_clicked_bits : arrow_left_bits);
    u8g2->drawXBMP(48, 15, ARROW_HEIGHT, ARROW_WIDTH,
                   _isRightPressed ? arrow_right_clicked_bits : arrow_right_bits);
    u8g2->drawXBMP(73, 3, BUTTON_WIDTH, BUTTON_HEIGHT,
                   _isOkPressed ? ok_button_clicked_bits : ok_button_bits);
    u8g2->drawXBMP(93, 28, BUTTON_WIDTH, BUTTON_HEIGHT,
                   _isBackPressed ? back_button_clicked_bits : back_button_bits);

    u8g2->setCursor(20, 62);
    u8g2->print("Konami: ");
    u8g2->print(_konamiIndex);
    u8g2->print("/10");

    u8g2->sendBuffer();
}