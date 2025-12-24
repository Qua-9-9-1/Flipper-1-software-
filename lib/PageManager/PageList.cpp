#include "PageList.hpp"

PageList::PageList(const char* title) : _title(title), _selectedIndex(0), _scrollOffset(0) {}

void PageList::addItem(const char* name, IPage* target, uint8_t iconIndex) {
    _items.push_back({name, target, iconIndex});
}

void PageList::onEvent(AppEvent *event) {
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
                if (_items[_selectedIndex].targetPage) {
                    PageManager::getInstance()->pushPage(_items[_selectedIndex].targetPage);
                }
                break;
            case BTN_BACK:
                PageManager::getInstance()->popPage();
                break;
        }
        normalizeCursor();
    }
}

void PageList::draw(U8G2 *u8g2) {
    u8g2->clearBuffer();
    
    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, 128, 12);
    u8g2->setDrawColor(0);
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->drawStr(2, 10, _title);
    
    u8g2->setDrawColor(1);
    
    for (int i = 0; i < _items.size(); i++) {
        // TODO : handle scrolling
        if (i == _selectedIndex)
            u8g2->drawStr(0, YSTART + (i * 12), ">");
        u8g2->drawStr(10, YSTART + (i * 12), _items[i].title);
        drawIcon(u8g2, _items[_selectedIndex].iconIndex);
    }

    u8g2->sendBuffer();
}

void PageList::drawIcon(U8G2 *u8g2, uint8_t iconIndex) {
    if (iconIndex >= sizeof(ListIcons) / sizeof(ListIcons[0]) || iconIndex < 0)
        return;

    icon_list_t icon = ListIcons[iconIndex];
    u8g2->drawXBMP(128 - icon.width - 2, 14, icon.width, icon.height, icon.bitmap);
}

void PageList::normalizeCursor() {
    if (_selectedIndex < 0) {
        _selectedIndex = _items.size() - 1;
    } else if (_selectedIndex >= _items.size()) {
        _selectedIndex = 0;
    }
}