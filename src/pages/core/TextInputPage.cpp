#include "TextInputPage.hpp"

#include <string.h>

TextInputPage::TextInputPage(const char* title, char* buffer, int maxLen, SaveCallback onSave)
    : _title(title),
      _buffer(buffer),
      _maxLen(maxLen),
      _cursorPos(0),
      _onSave(onSave),
      _showPopup(false),
      _popupYesSelected(true) {
    _backupBuffer[0] = '\0';
}

void TextInputPage::setTarget(const char* title, char* buffer, int maxLen, SaveCallback onSave) {
    _title            = title;
    _buffer           = buffer;
    _maxLen           = maxLen;
    _cursorPos        = 0;
    _onSave           = onSave;
    _showPopup        = false;
    _popupYesSelected = true;
    _backupBuffer[0]  = '\0';
}

void TextInputPage::onEnter() {
    if (_buffer && _maxLen > 1) {
        int copyLen = _maxLen - 1;
        if (copyLen >= (int)sizeof(_backupBuffer)) {
            copyLen = sizeof(_backupBuffer) - 1;
        }
        strncpy(_backupBuffer, _buffer, copyLen);
        _backupBuffer[copyLen] = '\0';

        int len = strlen(_buffer);
        for (int i = len; i < _maxLen - 1; i++) {
            _buffer[i] = ' ';
        }
        _buffer[_maxLen - 1] = '\0';
    }
    _cursorPos = 0;
    _showPopup = false;
}

char TextInputPage::cycleChar(char current, int direction) {
    const char charset[] = " ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";
    int        len       = sizeof(charset) - 1;
    int        idx       = 0;

    for (int i = 0; i < len; i++) {
        if (charset[i] == current) {
            idx = i;
            break;
        }
    }

    idx += direction;

    if (idx < 0) idx = len - 1;
    if (idx >= len) idx = 0;

    return charset[idx];
}

void TextInputPage::onEvent(AppEvent* event) {
    if (event->type == EVENT_BUTTON_CLICK) {
        playSound(SOUND_TICK);

        if (!_showPopup) {
            switch (event->value) {
                case BTN_LEFT:
                    _cursorPos--;
                    if (_cursorPos < 0) _cursorPos = 0;
                    break;
                case BTN_RIGHT:
                    _cursorPos++;
                    if (_cursorPos >= _maxLen - 1) _cursorPos = _maxLen - 2;
                    break;
                case BTN_UP:
                    _buffer[_cursorPos] = cycleChar(_buffer[_cursorPos], 1);
                    break;
                case BTN_DOWN:
                    _buffer[_cursorPos] = cycleChar(_buffer[_cursorPos], -1);
                    break;
                case BTN_OK:
                    _showPopup        = true;
                    _popupYesSelected = true;
                    break;
                case BTN_BACK:
                    if (_buffer) {
                        strncpy(_buffer, _backupBuffer, _maxLen - 1);
                        _buffer[_maxLen - 1] = '\0';
                    }
                    PageManager::getInstance()->popPage();
                    break;
            }
        } else {
            switch (event->value) {
                case BTN_LEFT:
                case BTN_RIGHT:
                    _popupYesSelected = !_popupYesSelected;
                    break;
                case BTN_UP:
                case BTN_DOWN:
                    break;
                case BTN_OK:
                    if (_popupYesSelected) {
                        if (_buffer && _maxLen > 1) {
                            int end = _maxLen - 2;
                            while (end >= 0 && _buffer[end] == ' ') {
                                end--;
                            }
                            _buffer[end + 1] = '\0';
                        }
                        if (_onSave && _buffer) {
                            _onSave(_buffer);
                        }
                        PageManager::getInstance()->popPage();
                    } else {
                        if (_buffer) {
                            strncpy(_buffer, _backupBuffer, _maxLen - 1);
                            _buffer[_maxLen - 1] = '\0';
                        }
                        PageManager::getInstance()->popPage();
                    }
                    break;
                case BTN_BACK:
                    _showPopup = false;
                    break;
            }
        }
    }
}

void TextInputPage::draw(U8G2* u8g2) {
    u8g2->clearBuffer();

    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, 128, 16);
    u8g2->setDrawColor(0);
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->drawStr(4, 12, _title);

    u8g2->setDrawColor(1);
    int textY  = 40;
    int startX = 10;

    for (int i = 0; i < _maxLen - 1; i++) {
        char c[2]  = {_buffer[i], '\0'};
        int  charX = startX + (i * 7);

        u8g2->drawStr(charX, textY, c);

        if (i == _cursorPos && !_showPopup) {
            u8g2->drawLine(charX, textY + 2, charX + 5, textY + 2);
        }
    }

    if (_showPopup) {
        u8g2->setDrawColor(0);
        u8g2->drawBox(14, 16, 100, 42);
        u8g2->setDrawColor(1);
        u8g2->drawFrame(14, 16, 100, 42);

        u8g2->drawStr(48, 30, "Save?");

        if (_popupYesSelected) u8g2->drawBox(20, 40, 32, 14);
        u8g2->setDrawColor(_popupYesSelected ? 0 : 1);
        u8g2->drawStr(27, 51, "Yes");

        u8g2->setDrawColor(1);
        if (!_popupYesSelected) u8g2->drawBox(76, 40, 32, 14);
        u8g2->setDrawColor(!_popupYesSelected ? 0 : 1);
        u8g2->drawStr(83, 51, "No");
    }
}