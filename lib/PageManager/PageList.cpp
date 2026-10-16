#include "PageList.hpp"

const int LINE_HEIGHT      = 12;
const int HEADER_HEIGHT    = 16;
const int ITEMS_PER_SCREEN = 4;

PageList::PageList(const char* title) : _title(title), _selectedIndex(0), _scrollOffset(0) {}

void PageList::onEnter() { setLEDMode(LED_MODE_BATTERY, 100); }

void PageList::addItem(const char* name, std::vector<ItemOption> options) {
    ListItem item;
    strncpy(item.title, name, sizeof(item.title) - 1);
    item.title[sizeof(item.title) - 1] = '\0';
    item.options = options;
    item.iconIndex = 0;
    item.currentOptionIndex = 0;
    _items.push_back(item);
}

void PageList::addItemWithIcon(const char* name, std::vector<ItemOption> options, uint8_t iconIndex) {
    ListItem item;
    strncpy(item.title, name, sizeof(item.title) - 1);
    item.title[sizeof(item.title) - 1] = '\0';
    item.options = options;
    item.iconIndex = iconIndex;
    item.currentOptionIndex = 0;
    _items.push_back(item);
}

void PageList::updateItemTitle(int index, const char* newTitle) {
    if (index >= 0 && index < (int)_items.size() && newTitle) {
        strncpy(_items[index].title, newTitle, sizeof(_items[index].title) - 1);
        _items[index].title[sizeof(_items[index].title) - 1] = '\0';
    }
}

void PageList::clearItems() {
    _items.clear();
    _selectedIndex = 0;
    _scrollOffset = 0;
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
            case BTN_LEFT:
                if (!_items.empty() && _items[_selectedIndex].options.size() > 1) {
                    _items[_selectedIndex].currentOptionIndex--;
                    if (_items[_selectedIndex].currentOptionIndex < 0) {
                        _items[_selectedIndex].currentOptionIndex = _items[_selectedIndex].options.size() - 1;
                    }
                }
                break;
            case BTN_RIGHT:
                if (!_items.empty() && _items[_selectedIndex].options.size() > 1) {
                    _items[_selectedIndex].currentOptionIndex++;
                    if (_items[_selectedIndex].currentOptionIndex >= (int)_items[_selectedIndex].options.size()) {
                        _items[_selectedIndex].currentOptionIndex = 0;
                    }
                }
                break;
            case BTN_OK:
                if (!_items.empty() && !_items[_selectedIndex].options.empty()) {
                    int optIdx = _items[_selectedIndex].currentOptionIndex;
                    if (optIdx >= 0 && optIdx < (int)_items[_selectedIndex].options.size()) {
                        handleSelection(_items[_selectedIndex].options[optIdx]);
                    }
                } else {
                    Serial.println("[PageList] No action assigned to this item");
                }
                break;
            case BTN_BACK:
                PageManager::getInstance()->popPage();
                break;
        }
        normalizeCursor();
        if (_selectedIndex < _scrollOffset) {
            _scrollOffset = _selectedIndex;
        } else if (_selectedIndex >= _scrollOffset + ITEMS_PER_SCREEN) {
            _scrollOffset = _selectedIndex - ITEMS_PER_SCREEN + 1;
        }
    }
}

void PageList::draw(U8G2* u8g2) {
    int endIndex    = _scrollOffset + ITEMS_PER_SCREEN + 1;
    int visualIndex = 0;
    int yPos        = 0;

    u8g2->clearBuffer();
    if (!_items.empty() && _selectedIndex >= 0 && _selectedIndex < (int)_items.size()) {
        drawIcon(u8g2, _items[_selectedIndex].iconIndex);
    }
    const char* headerText = _title;
    char optBuffer[40];
    if (!_items.empty() && _selectedIndex >= 0 && _selectedIndex < (int)_items.size()) {
        const auto& curItem = _items[_selectedIndex];
        if (curItem.options.size() > 1) {
            int optIdx = curItem.currentOptionIndex;
            if (optIdx >= 0 && optIdx < (int)curItem.options.size()) {
                snprintf(optBuffer, sizeof(optBuffer), "< %s >", curItem.options[optIdx].name);
                headerText = optBuffer;
            }
        }
    }

    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, 128, HEADER_HEIGHT);
    u8g2->setDrawColor(0);
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->drawStr(4, (HEADER_HEIGHT + 8) / 2, headerText);

    u8g2->setDrawColor(1);
    if (endIndex > (int)_items.size()) endIndex = _items.size();
    for (int i = _scrollOffset; i < endIndex; i++) {
        visualIndex = i - _scrollOffset;
        yPos        = HEADER_HEIGHT + (visualIndex * LINE_HEIGHT);
        if (i == _selectedIndex) {
            u8g2->drawStr(0, yPos + 10, ">");
        }
        u8g2->drawStr(10, yPos + 10, _items[i].title);
    }
    drawScrollbar(u8g2);
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
    if (iconIndex == 0 || iconIndex > sizeof(ListIcons) / sizeof(ListIcons[0])) return;

    icon_list_t icon = ListIcons[iconIndex - 1];
    u8g2->drawXBMP(128 - icon.width - 2, 14, icon.width, icon.height, icon.bitmap);
}

void PageList::normalizeCursor() {
    if (_items.empty()) {
        _selectedIndex = 0;
        _scrollOffset  = 0;
        return;
    }
    if (_selectedIndex < 0) {
        _selectedIndex = _items.size() - 1;
        _scrollOffset  = _items.size() - ITEMS_PER_SCREEN;
        if (_scrollOffset < 0) _scrollOffset = 0;
    } else if (_selectedIndex >= (int)_items.size()) {
        _selectedIndex = 0;
        _scrollOffset  = 0;
    }
}

void PageList::handleSelection(const ItemOption& item) {
    if (item.action) {
        item.action();
    } else {
        Serial.println("[PageList] No action assigned to this option");
    }
}