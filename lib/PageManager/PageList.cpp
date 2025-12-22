#include "PageList.h"

PageList::PageList(const char* t) : title(t), selectedIndex(0), scrollOffset(0) {}

void PageList::addItem(const char* name, IPage* target, uint8_t iconIndex) {
    items.push_back({name, target, iconIndex});
}

void PageList::onEvent(AppEvent *event) {
    if (event->type == EVENT_BUTTON_CLICK) {
        switch(event->value) {
            case BTN_UP: 
                selectedIndex = abs(selectedIndex - 1); 
                break;
            case BTN_DOWN: 
                selectedIndex = abs(selectedIndex + 1); 
                break;
            case BTN_OK:
                if(items[selectedIndex].targetPage) {
                    PageManager::getInstance()->pushPage(items[selectedIndex].targetPage);
                }
                break;
            case BTN_BACK:
                PageManager::getInstance()->popPage();
                break;
        }
        selectedIndex %= items.size();
    }
}

void PageList::draw(U8G2 *u8g2) {
    int yStart = 24;

    u8g2->clearBuffer();
    
    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, 128, 12);
    u8g2->setDrawColor(0);
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->drawStr(2, 10, title);
    
    u8g2->setDrawColor(1);
    
    for (int i = 0; i < items.size(); i++) {
        // TODO : handle scrolling
        if(i == selectedIndex) u8g2->drawStr(0, yStart + (i * 12), ">");
        u8g2->drawStr(10, yStart + (i * 12), items[i].title);
        drawIcon(u8g2, items[selectedIndex].iconIndex);
    }

    u8g2->sendBuffer();
}

void PageList::drawIcon(U8G2 *u8g2, uint8_t iconIndex) {
    if (iconIndex >= sizeof(ListIcons) / sizeof(ListIcons[0]) || iconIndex < 0)
        return;

    icon_list_t icon = ListIcons[iconIndex];
    u8g2->drawXBMP(128 - icon.width - 2, 14, icon.width, icon.height, icon.bitmap);
}