#pragma once
#include "Page.h"
#include "PinDefinitions.h"
#include "Assets.h"
#include "PageManager.h"

class PageLEDConfig : public IPage {
    public:
        PageLEDConfig() {};
        void onEnter() override;
        void onEvent(AppEvent *event) override;
        void draw(U8G2 *u8g2) override;
    private:
        int _selectedOption = 0;
        int _brightness = 50;
        int _ledMode = 0;
};