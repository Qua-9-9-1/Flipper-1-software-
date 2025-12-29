#include "PageList.hpp"

const int LINE_HEIGHT      = 12;
const int HEADER_HEIGHT    = 16;
const int ITEMS_PER_SCREEN = 4;

PageList::PageList(const char* title) : _title(title), _selectedIndex(0), _scrollOffset(0) {}

void PageList::onEnter() { setLEDMode(LED_MODE_BATTERY, 100); }

void PageList::addItem(const char* name, IPage* target, int actionID) {
    _items.push_back({name, target, 0, actionID});
}

void PageList::addItemWithIcon(const char* name, IPage* target, uint8_t iconIndex, int actionID) {
    _items.push_back({name, target, iconIndex, actionID});
}

void PageList::onEvent(AppEvent* event) {
    if (event->type == EVENT_BUTTON_CLICK) {
        playSound(SOUND_TICK);
        switch (event->value) {
            case BTN_UP:
                _selectedIndex--;
                break;
            case BTN_DOWN:
                _selectedIndex++;
                break;
            case BTN_OK:
                handleSelection(_items[_selectedIndex]);
                break;
            case BTN_BACK:
                PageManager::getInstance()->popPage();
                break;
        }
        if (_selectedIndex < _scrollOffset) {
            _scrollOffset = _selectedIndex;
        } else if (_selectedIndex >= _scrollOffset + ITEMS_PER_SCREEN) {
            _scrollOffset = _selectedIndex - ITEMS_PER_SCREEN + 1;
        }
        normalizeCursor();
    }
}

void PageList::draw(U8G2* u8g2) {
    int endIndex    = _scrollOffset + ITEMS_PER_SCREEN + 1;
    int visualIndex = 0;
    int yPos        = 0;

    u8g2->clearBuffer();
    drawIcon(u8g2, _items[_selectedIndex].iconIndex);
    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, 128, HEADER_HEIGHT);
    u8g2->setDrawColor(0);
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->drawStr(4, (HEADER_HEIGHT + 8) / 2, _title);
    u8g2->setDrawColor(1);
    if (endIndex > _items.size()) endIndex = _items.size();
    for (int i = _scrollOffset; i < endIndex; i++) {
        visualIndex = i - _scrollOffset;
        yPos        = HEADER_HEIGHT + (visualIndex * LINE_HEIGHT);
        if (i == _selectedIndex) {
            u8g2->drawStr(0, yPos + 10, ">");
        }
        u8g2->drawStr(10, yPos + 10, _items[i].title);
    }
    drawScrollbar(u8g2);
    u8g2->sendBuffer();
}

void PageList::drawScrollbar(U8G2* u8g2) {
    int totalHeight = 0;
    int barHeight   = 0;
    int barY        = 0;

    if (_items.size() > ITEMS_PER_SCREEN) {
        totalHeight = 64 - HEADER_HEIGHT;
        barHeight   = (totalHeight * ITEMS_PER_SCREEN) / _items.size();
        if (barHeight < 4) barHeight = 4;
        barY = HEADER_HEIGHT +
               ((totalHeight - barHeight) * _scrollOffset) / (_items.size() - ITEMS_PER_SCREEN);
        u8g2->drawBox(124, barY, 3, barHeight);
        u8g2->drawFrame(123, HEADER_HEIGHT, 5, totalHeight);
    }
}

void PageList::drawIcon(U8G2* u8g2, uint8_t iconIndex) {
    if (iconIndex > sizeof(ListIcons) / sizeof(ListIcons[0]) || iconIndex <= 0) return;

    icon_list_t icon = ListIcons[iconIndex - 1];
    u8g2->drawXBMP(128 - icon.width - 2, 14, icon.width, icon.height, icon.bitmap);
}

void PageList::normalizeCursor() {
    if (_selectedIndex < 0) {
        _selectedIndex = _items.size() - 1;
        _scrollOffset  = _items.size() - ITEMS_PER_SCREEN;
        if (_scrollOffset < 0) _scrollOffset = 0;
    } else if (_selectedIndex >= _items.size()) {
        _selectedIndex = 0;
        _scrollOffset  = 0;
    }
}

void PageList::handleSelection(ListItem item) {
    if (item.targetPage) {
        PageManager::getInstance()->pushPage(item.targetPage);
    } else {
        // action handling
        Serial.print("Action on item: ");
        Serial.println(item.title);
        // BadUsbManager::getInstance()->runScript(item.title);
    }
}