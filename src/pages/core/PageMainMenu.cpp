#include "PageMainMenu.hpp"

const unsigned char* _icons[6]    = {icon_ghz_bits, icon_nfc_rfid_bits, icon_ir_bits,
                                     icon_usb_bits, icon_settings_bits, icon_shutdown_bits};
const char*          _appNames[6] = {"Radio", "NFC/RFID", "IR", "USB", "Settings", "Power Off"};

const int HEADER_HEIGHT = 12;
const int GRID_COLS     = 4;
const int GRID_ROWS     = 2;

PageMainMenu::PageMainMenu() {
    _pos[0] = 0;
    _pos[1] = 0;
    _linkedPages.clear();
}

PageMainMenu::~PageMainMenu() { _linkedPages.clear(); }

void PageMainMenu::onEnter() { setLEDMode(LED_MODE_BATTERY, 100); }

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
                index = (_pos[1] * GRID_COLS) + _pos[0];
                if (index >= 0 && index < (int)_linkedPages.size()) {
                    target = _linkedPages[index].second;
                    if (target != nullptr) PageManager::getInstance()->pushPage(target);
                }
                break;
            }
        }
        normalizePos();
    }
}

void PageMainMenu::draw(U8G2* u8g2) {
    int         slotWidth    = 128 / GRID_COLS;
    int         slotHeight   = (64 - HEADER_HEIGHT) / GRID_ROWS;
    const char* title        = "Menu";
    int         currentIndex = (_pos[1] * GRID_COLS) + _pos[0];
    int         col          = 0;
    int         row          = 0;
    int         x            = 0;
    int         y            = 0;

    if (currentIndex >= 0 && currentIndex < 6) {
        title = _appNames[currentIndex];
    }
    u8g2->clearBuffer();
    drawPageHeader(u8g2, title);
    for (size_t i = 0; i < _linkedPages.size(); i++) {
        int col = i % GRID_COLS;
        int row = i / GRID_COLS;
        int x   = (col * slotWidth) + ((slotWidth - MENU_ICON_DIM) / 2);
        int y   = HEADER_HEIGHT + (row * slotHeight) + ((slotHeight - MENU_ICON_DIM) / 2);
        u8g2->drawXBMP(x, y, MENU_ICON_DIM, MENU_ICON_DIM, _icons[i]);
        if (col == _pos[0] && row == _pos[1]) {
            u8g2->drawRFrame(x, y, MENU_ICON_DIM, MENU_ICON_DIM, 2);
        }
    }
}

void PageMainMenu::addIcon(Icon icon, IPage* linkedPage) {
    if (_linkedPages.size() == 8) {
        Serial.println("Max icons reached");
        return;
    }
    _linkedPages.push_back(std::make_pair((int)icon, linkedPage));
}

void PageMainMenu::normalizePos() {
    int maxIndex = _linkedPages.size() - 1;
    int maxRow   = maxIndex / GRID_COLS;
    int maxCol   = maxIndex % GRID_COLS;

    if (_pos[0] < 0) _pos[0] = GRID_COLS - 1;
    if (_pos[0] >= GRID_COLS) _pos[0] = 0;
    if (_pos[1] < 0) _pos[1] = maxRow;
    if (_pos[1] > maxRow) _pos[1] = 0;
    if (_pos[1] == maxRow && _pos[0] > maxCol) _pos[0] = maxCol;
}

void PageMainMenu::drawPageHeader(U8G2* u8g2, const char* title) {
    int cursorX = 126;

    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, 128, HEADER_HEIGHT);
    u8g2->setDrawColor(0);
    u8g2->setFont(u8g2_font_5x7_tf);
    u8g2->drawStr(2, 9, title);
    drawBattery(u8g2, cursorX);
    drawUSB(u8g2, cursorX);
    drawSD(u8g2, cursorX);
    drawSound(u8g2, cursorX);
    u8g2->setDrawColor(1);
}

void PageMainMenu::drawBattery(U8G2* u8g2, int& cursorX) {
    int  batLevel   = BatteryHelper::getLevel();
    bool isCharging = BatteryHelper::isCharging();
    int  width      = 0;
    cursorX -= 12;

    u8g2->drawFrame(cursorX, 3, 10, 6);
    u8g2->drawBox(cursorX + 10, 4, 2, 4);

    if (isCharging) {
        u8g2->drawLine(cursorX + 3, 8, cursorX + 5, 6);
        u8g2->drawLine(cursorX + 5, 6, cursorX + 3, 6);
        u8g2->drawLine(cursorX + 3, 6, cursorX + 6, 3);
    } else {
        Serial.print("Battery Level: ");
        Serial.println(batLevel);
        width = map(batLevel, 0, 100, 0, 6);
        Serial.print("Battery Width: ");
        Serial.println(width);
        if (width > 0) u8g2->drawBox(cursorX + 2, 5, width, 2);
    }
}

void PageMainMenu::drawUSB(U8G2* u8g2, int& cursorX) {
    // TODO : real USB connection state from system
    bool connected = true;

    if (connected) {
        cursorX -= 10;
        u8g2->drawLine(cursorX + 3, 3, cursorX + 3, 8);
        u8g2->drawLine(cursorX + 1, 3, cursorX + 5, 3);
        u8g2->drawBox(cursorX + 2, 2, 3, 2);
    }
}

void PageMainMenu::drawSD(U8G2* u8g2, int& cursorX) {
    // TODO : real SD card state from storage manager
    bool sdPresent = true;

    if (sdPresent) {
        cursorX -= 10;
        u8g2->drawFrame(cursorX, 3, 7, 7);
        u8g2->drawBox(cursorX + 1, 3, 1, 2);
        u8g2->drawBox(cursorX + 3, 3, 1, 2);
        u8g2->drawBox(cursorX + 5, 3, 1, 2);
        u8g2->setDrawColor(1);
        u8g2->drawLine(cursorX + 6, 3, cursorX + 6, 4);
        u8g2->setDrawColor(0);
    }
}

void PageMainMenu::drawSound(U8G2* u8g2, int& cursorX) {
    // TODO : real sound state from AudioHelper
    bool isMuted = false;

    cursorX -= 12;
    u8g2->drawBox(cursorX, 4, 2, 4);
    u8g2->drawLine(cursorX + 2, 4, cursorX + 5, 2);
    u8g2->drawLine(cursorX + 5, 2, cursorX + 5, 9);
    u8g2->drawLine(cursorX + 5, 9, cursorX + 2, 7);
    if (isMuted) {
        u8g2->drawLine(cursorX + 7, 4, cursorX + 9, 8);
        u8g2->drawLine(cursorX + 9, 4, cursorX + 7, 8);
    } else {
        u8g2->drawPixel(cursorX + 7, 6);
        u8g2->drawLine(cursorX + 9, 4, cursorX + 9, 8);
    }
}