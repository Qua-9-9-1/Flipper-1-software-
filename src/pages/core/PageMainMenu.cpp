#include "PageMainMenu.hpp"

PageMainMenu::PageMainMenu() {
    _pos[0] = 0;
    _pos[1] = 0;
    _linkedPages.clear();
}

PageMainMenu::~PageMainMenu() { _linkedPages.clear(); }

void PageMainMenu::onEnter() { setLedMode(LED_MODE_BATTERY, 100); }

void PageMainMenu::onEvent(AppEvent* event) {
    int    index  = 0;
    IPage* target = nullptr;

    if (event->type == EVENT_BUTTON_CLICK) {
        playSound(SOUND_TICK);
        switch (event->value) {
            case BTN_UP:
                _pos[1]--;
                break;
            case BTN_DOWN:
                _pos[1]++;
                break;
            case BTN_LEFT:
                _pos[0]--;
                break;
            case BTN_RIGHT:
                _pos[0]++;
                break;
            case BTN_OK: {
                index = (_pos[1] * COLS) + _pos[0];
                if (index >= 0 && index < (int)_linkedPages.size()) {
                    target = _linkedPages[index].second;
                    if (target != nullptr) PageManager::getInstance()->pushPage(target);
                }
                break;
            }
            case BTN_BACK: {
                // TODO: Shutdown popup
                break;
            }
        }
        normalizePos();
    }
}

void PageMainMenu::draw(U8G2* u8g2) {
    int x = 0;
    int y = 0;
    u8g2->clearBuffer();

    for (size_t i = 0; i < _linkedPages.size(); i++) {
        x = (i % COLS) * MENU_ICON_DIM;
        y = (i / COLS) * MENU_ICON_DIM;
        u8g2->drawXBMP(x, y, MENU_ICON_DIM, MENU_ICON_DIM, _icons[i]);

        u8g2->drawFrame(_pos[0] * MENU_ICON_DIM, _pos[1] * MENU_ICON_DIM, MENU_ICON_DIM,
                        MENU_ICON_DIM);
    }

    u8g2->sendBuffer();
}

void PageMainMenu::addIcon(int icon, IPage* linkedPage) {
    if (_linkedPages.size() == 8) {
        Serial.println("Max icons reached");
        return;
    }
    _linkedPages.push_back(std::make_pair(icon, linkedPage));
}

void PageMainMenu::normalizePos() {
    int maxIndex = _linkedPages.size() - 1;
    int maxRow   = maxIndex / COLS;
    int maxCol   = maxIndex % COLS;

    if (_pos[0] < 0) _pos[0] = COLS - 1;
    if (_pos[0] >= COLS) _pos[0] = 0;
    if (_pos[1] < 0) _pos[1] = maxRow;
    if (_pos[1] > maxRow) _pos[1] = 0;
    if (_pos[1] == maxRow && _pos[0] > maxCol) _pos[0] = maxCol;
}
