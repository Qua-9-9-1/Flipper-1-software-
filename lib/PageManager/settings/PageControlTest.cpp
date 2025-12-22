#include "PageControlTest.h"

void PageControlTest::onEnter() {
    konamiIndex = 0;
}

void PageControlTest::onEvent(AppEvent *event) {
    if (event->type == EVENT_BUTTON_DOWN) {
        switch(event->value) {
            case BTN_UP: isUpPressed = true; break;
            case BTN_OK: isOkPressed = true; break;
            case BTN_DOWN: isDownPressed = true; break;
            case BTN_LEFT: isLeftPressed = true; break;
            case BTN_RIGHT: isRightPressed = true; break;
            case BTN_BACK: isBackPressed = true; break;
        }
    } else if (event->type == EVENT_BUTTON_UP) {
        switch(event->value) {
            case BTN_UP: isUpPressed = false; break;
            case BTN_OK: isOkPressed = false; break;
            case BTN_DOWN: isDownPressed = false; break;
            case BTN_LEFT: isLeftPressed = false; break;
            case BTN_RIGHT: isRightPressed = false; break;
            case BTN_BACK: isBackPressed = false; break;
        }
    }

    if (event->type == EVENT_BUTTON_CLICK) {
        checkKonami((ButtonID)event->value);
    }
}

void PageControlTest::checkKonami(ButtonID input) {
    if (input == konamiSequence[konamiIndex]) {
        konamiIndex++;        
        if (konamiIndex == 10) {
            PageManager::getInstance()->popPage();
        }
    } else {
        konamiIndex = 0;
    }
}

void PageControlTest::draw(U8G2 *u8g2) {
    u8g2->clearBuffer();
                
    u8g2->drawXBMP(26, 1, ARROW_WIDTH, ARROW_HEIGHT, isUpPressed ? arrow_up_clicked_bits : arrow_up_bits);
    u8g2->drawXBMP(26, 37, ARROW_WIDTH, ARROW_HEIGHT, isDownPressed ? arrow_down_clicked_bits : arrow_down_bits);
    u8g2->drawXBMP(12, 15, ARROW_HEIGHT, ARROW_WIDTH, isLeftPressed ? arrow_left_clicked_bits : arrow_left_bits);
    u8g2->drawXBMP(48, 15, ARROW_HEIGHT, ARROW_WIDTH, isRightPressed ? arrow_right_clicked_bits : arrow_right_bits);
    u8g2->drawXBMP(73, 3, BUTTON_WIDTH, BUTTON_HEIGHT, isOkPressed ? ok_button_clicked_bits : ok_button_bits);
    u8g2->drawXBMP(93, 28, BUTTON_WIDTH, BUTTON_HEIGHT, isBackPressed ? back_button_clicked_bits : back_button_bits);

    u8g2->setCursor(20, 62);
    u8g2->print("Konami: ");
    u8g2->print(konamiIndex);
    u8g2->print("/10");
    
    u8g2->sendBuffer();
}