#include "PageMainMenu.h"

PageMainMenu::PageMainMenu() {
    pos[0] = 0;
    pos[1] = 0;
    linkedPages.clear();
}

PageMainMenu::~PageMainMenu() {
    linkedPages.clear();
}

void PageMainMenu::onEvent(AppEvent *event) {
    int index = 0;
    IPage* target = nullptr;

    if (event->type == EVENT_BUTTON_CLICK) {
        switch (event->value) {
            case BTN_UP:    pos[1]--; break;
            case BTN_DOWN:  pos[1]++; break;
            case BTN_LEFT:  pos[0]--; break;
            case BTN_RIGHT: pos[0]++; break;
            case BTN_OK: {
                index = (pos[1] * COLS) + pos[0];
                if (index >= 0 && index < (int)linkedPages.size()) {
                    target = linkedPages[index].second;
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

void PageMainMenu::draw(U8G2 *u8g2) {
    int x = 0;
    int y = 0;
    u8g2->clearBuffer();

    for (size_t i = 0; i < linkedPages.size(); i++) {
        x = (i % COLS) * MENU_ICON_DIM;
        y = (i / COLS) * MENU_ICON_DIM;
        u8g2->drawXBMP(x, y, MENU_ICON_DIM, MENU_ICON_DIM, icons[i]);

        u8g2->drawFrame(pos[0] * MENU_ICON_DIM, pos[1] * MENU_ICON_DIM, MENU_ICON_DIM, MENU_ICON_DIM);
    }

    u8g2->sendBuffer();
}

void PageMainMenu::addIcon(int icon, IPage* linkedPage) {
    if (linkedPages.size() == 8) {
        Serial.println("Max icons reached");
        return;
    }
    linkedPages.push_back(std::make_pair(icon, linkedPage));
}

void PageMainMenu::normalizePos() {
    int maxIndex = linkedPages.size() - 1;
    int maxRow = maxIndex / COLS;
    int maxCol = maxIndex % COLS;

    if (pos[0] < 0) pos[0] = COLS - 1;
    if (pos[0] >= COLS) pos[0] = 0;
    if (pos[1] < 0) pos[1] = maxRow;
    if (pos[1] > maxRow) pos[1] = 0;
    if (pos[1] == maxRow && pos[0] > maxCol) pos[0] = maxCol;
}
