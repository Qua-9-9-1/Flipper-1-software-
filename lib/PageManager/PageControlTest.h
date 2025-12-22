#pragma once
#include "Page.h"
#include "PinDefinitions.h"
#include "Assets.h"
#include "PageManager.h"

class PageControlTest : public IPage {
    public:
        PageControlTest() {};
        void onEnter() override;
        void onEvent(AppEvent *event) override;
        void checkKonami(ButtonID input);
        void draw(U8G2 *u8g2) override;
    private:
        bool isUpPressed = false;
        bool isDownPressed = false;
        bool isLeftPressed = false;
        bool isRightPressed = false;
        bool isOkPressed = false;
        bool isBackPressed = false;
        const ButtonID konamiSequence[10] = {
            BTN_UP, BTN_UP, BTN_DOWN, BTN_DOWN, 
            BTN_LEFT, BTN_RIGHT, BTN_LEFT, BTN_RIGHT, 
            BTN_BACK, BTN_OK 
        };
        int konamiIndex = 0;

};