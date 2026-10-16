#include "PageList.hpp"

const int LINE_HEIGHT      = 12;
const int HEADER_HEIGHT    = 16;
const int ITEMS_PER_SCREEN = 4;

PageList::PageList(const char* title) : _title(title), _selectedIndex(0), _scrollOffset(0) {}

void PageList::onEnter() { setLEDMode(LED_MODE_BATTERY, 100); }

void PageList::addItem(const char* name, std::vector<ItemOption> options) {
    _items.push_back({name, options, 0, 0});
}

void PageList::addItemWithIcon(const char* name, std::vector<ItemOption> options, uint8_t iconIndex) {
    _items.push_back({name, options, iconIndex, 0});
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
                    Serial.println("[PageList] No action assigned to this item.");
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
    int endIndex    = _scrollOffset + ITEMS_PER_SCREEN;
    int visualIndex = 0;
    int yPos        = 0;

    u8g2->clearBuffer();
    
    if (_items.size() > 0) {
        drawIcon(u8g2, _items[_selectedIndex].iconIndex);
    }

    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, 128, HEADER_HEIGHT);
    u8g2->setDrawColor(0);
    u8g2->setFont(u8g2_font_6x10_tf);

    if (_items.size() > 0 && _items[_selectedIndex].options.size() > 1) {
        int optIdx = _items[_selectedIndex].currentOptionIndex;
        const char* optName = _items[_selectedIndex].options[optIdx].name;
        
        int textWidth = u8g2->getStrWidth(optName);
        int startX = (128 - textWidth) / 2;
        
        u8g2->drawStr(startX - 12, (HEADER_HEIGHT + 8) / 2, "<");
        u8g2->drawStr(startX, (HEADER_HEIGHT + 8) / 2, optName);
        u8g2->drawStr(startX + textWidth + 6, (HEADER_HEIGHT + 8) / 2, ">");
    } 
    else {
        u8g2->drawStr(4, (HEADER_HEIGHT + 8) / 2, _title);
    }

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
        Serial.println("[PageList] No action assigned to this option.");
    }
}