#pragma once

#include "Page.hpp"
#include "PageManager.hpp"
#include "helpers/AudioHelper.h"

class TextInputPage : public IPage {
   public:
    TextInputPage(const char* title = "Edit", char* buffer = nullptr, int maxLen = 0);

    void setTarget(const char* title, char* buffer, int maxLen);
    void onEnter() override;
    void onEvent(AppEvent* event) override;
    void draw(U8G2* u8g2) override;

   private:
    char cycleChar(char current, int direction);

    const char* _title;
    char*       _buffer;
    int         _maxLen;
    int         _cursorPos;
    char        _backupBuffer[32];

    bool _showPopup;
    bool _popupYesSelected;
};