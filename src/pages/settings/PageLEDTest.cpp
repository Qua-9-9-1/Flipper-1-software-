#include "PageLEDTest.hpp"

const LedMode PageLEDTest::_availableModes[] = {LED_MODE_ON, LED_MODE_BLINK, LED_MODE_FADE,
                                                LED_MODE_TIMEOUT, LED_MODE_OFF};

const char* PageLEDTest::_modeNames[] = {"ON (Fixe)", "BLINK (Cligno)", "FADE (Respi)",
                                         "TIMEOUT (1s)", "OFF"};

const int MODE_COUNT = 5;
const int YSTART     = 20;
const int HLINE      = 10;
const int BAR_WIDTH  = 80;
const int BAR_X      = 15;
const int YMODE      = 60;

PageLEDTest::PageLEDTest() : _currentRow(0), _color{127, 127, 127, 100}, _modeIndex(0) {
    _selectedMode = _availableModes[0];
}

void PageLEDTest::onEnter() {
    setLEDColor(_color[0], _color[1], _color[2]);
    setLEDBrightness(_color[3]);
    setLEDMode(_selectedMode);
}

void PageLEDTest::onEvent(AppEvent* event) {
    int key   = 0;
    int delta = 0;

    if (event->type == EVENT_BUTTON_CLICK) {
        key = event->value;
        if (key == BTN_UP) {
            _currentRow--;
            if (_currentRow < 0) _currentRow = 4;
        } else if (key == BTN_DOWN) {
            _currentRow++;
            if (_currentRow > 4) _currentRow = 0;
        } else if (key == BTN_LEFT || key == BTN_RIGHT) {
            delta = (key == BTN_RIGHT) ? 10 : -10;
            switch (_currentRow) {
                case 0:
                    _color[0] = constrain(_color[0] + delta, 0, 255);
                    setLEDColor(_color[0], _color[1], _color[2]);
                    break;
                case 1:
                    _color[1] = constrain(_color[1] + delta, 0, 255);
                    setLEDColor(_color[0], _color[1], _color[2]);
                    break;
                case 2:
                    _color[2] = constrain(_color[2] + delta, 0, 255);
                    setLEDColor(_color[0], _color[1], _color[2]);
                    break;
                case 3:
                    _color[3] = constrain(_color[3] + delta, 0, 255);
                    setLEDBrightness(_color[3]);
                    break;
                case 4:
                    if (key == BTN_RIGHT)
                        _modeIndex++;
                    else
                        _modeIndex--;
                    if (_modeIndex >= MODE_COUNT) _modeIndex = 0;
                    if (_modeIndex < 0) _modeIndex = MODE_COUNT - 1;
                    _selectedMode = _availableModes[_modeIndex];
                    break;
            }
        } else if (key == BTN_OK) {
            if (_selectedMode == LED_MODE_TIMEOUT) {
                setLEDMode(LED_MODE_TIMEOUT, 1000);
            } else {
                setLEDMode(_selectedMode);
            }
        } else if (key == BTN_BACK) {
            PageManager::getInstance()->popPage();
        }
    }
}

void PageLEDTest::draw(U8G2* u8g2) {
    int  values[] = {_color[0], _color[1], _color[2], _color[3]};
    int  y        = 0;
    int  fill     = 0;
    char buf[5]   = {0};

    u8g2->clearBuffer();
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, 128, 10);
    u8g2->setDrawColor(0);
    u8g2->drawStr(2, 8, "LED Lab");
    u8g2->setDrawColor(1);
    u8g2->drawStr(0, YSTART, "R");
    u8g2->drawStr(0, YSTART + HLINE, "G");
    u8g2->drawStr(0, YSTART + HLINE * 2, "B");
    u8g2->drawStr(0, YSTART + HLINE * 3, "A");
    for (int i = 0; i < 4; i++) {
        y = YSTART + (i * HLINE) - 8;
        u8g2->drawFrame(BAR_X, y, BAR_WIDTH, 6);
        fill = map(values[i], 0, 255, 0, BAR_WIDTH - 2);
        if (fill > 0) u8g2->drawBox(BAR_X + 1, y + 1, fill, 4);
        sprintf(buf, "%d", values[i]);
        u8g2->drawStr(BAR_X + BAR_WIDTH + 5, YSTART + (i * HLINE), buf);
        if (_currentRow == i) {
            u8g2->drawStr(120, YSTART + (i * HLINE), "<");
        }
    }
    u8g2->drawLine(0, 50, 128, 50);
    u8g2->drawStr(0, YMODE, "Mode:");
    u8g2->drawStr(35, YMODE, _modeNames[_modeIndex]);
    if (_currentRow == 4) {
        u8g2->drawFrame(33, YMODE - 9, 90, 11);
    }
}