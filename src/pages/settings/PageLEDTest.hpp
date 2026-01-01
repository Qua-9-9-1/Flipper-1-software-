#pragma once
#include "AppEvents.h"
#include "Page.hpp"
#include "PageManager.hpp"
#include "helpers/LedHelper.h"

class PageLEDTest : public IPage {
   public:
    PageLEDTest();
    void onEnter() override;
    void onEvent(AppEvent* event) override;
    void draw(U8G2* u8g2) override;

   private:
    int                  _currentRow;
    LedMode              _selectedMode;
    uint8_t              _color[4];
    static const LedMode _availableModes[];
    static const char*   _modeNames[];
    int                  _modeIndex;
};