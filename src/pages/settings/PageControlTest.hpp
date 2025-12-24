#pragma once
#include "PinDefinitions.h"
#include "Assets.h"
#include "Page.hpp"
#include "PageManager.hpp"

class PageControlTest : public IPage {
    public:
        PageControlTest();
        void onEnter() override;
        void onEvent(AppEvent *event) override;
        void checkKonami(ButtonID input);
        void draw(U8G2 *u8g2) override;
    private:
        bool _isUpPressed = false;
        bool _isDownPressed = false;
        bool _isLeftPressed = false;
        bool _isRightPressed = false;
        bool _isOkPressed = false;
        bool _isBackPressed = false;
        const ButtonID _konamiSequence[10] = {
            BTN_UP, BTN_UP, BTN_DOWN, BTN_DOWN, 
            BTN_LEFT, BTN_RIGHT, BTN_LEFT, BTN_RIGHT, 
            BTN_BACK, BTN_OK 
        };
        int _konamiIndex;

};