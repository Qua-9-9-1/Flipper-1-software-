#pragma once
#include <functional>

#include "Page.hpp"
#include "PageManager.hpp"
#include "helpers/AudioHelper.h"

class TextInputPage : public IPage {
   public:
    using SaveCallback = std::function<void(const char*)>;

    TextInputPage(const char* title = "Edit", char* buffer = nullptr, int maxLen = 0, SaveCallback onSave = nullptr);

    void setTarget(const char* title, char* buffer, int maxLen, SaveCallback onSave = nullptr);
    void onEnter() override;
    void onEvent(AppEvent* event) override;
    void draw(U8G2* u8g2) override;

   private:
    char cycleChar(char current, int direction);

    const char*  _title;
    char*        _buffer;
    int          _maxLen;
    int          _cursorPos;
    char         _backupBuffer[32];
    SaveCallback _onSave;

    bool _showPopup;
    bool _popupYesSelected;
};