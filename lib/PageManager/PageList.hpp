#pragma once
#include <vector>

#include "Assets.h"
#include "AudioHelper.h"
#include "LedHelper.h"
#include "Page.hpp"
#include "PageManager.hpp"

struct ListItem {
    const char* title;
    IPage*      targetPage;
    uint8_t     iconIndex;
    int         actionID;
};

struct icon_list_t {
    const unsigned char* bitmap;
    const uint8_t        width;
    const uint8_t        height;
};

static icon_list_t ListIcons[] = {{icon_windows_bits, OS_ICON_DIM, OS_ICON_DIM},
                                  {icon_macos_bits, OS_ICON_DIM, OS_ICON_DIM}};

class PageList : public IPage {
   public:
    PageList(const char* title);
    void onEnter() override;
    void addItem(const char* name, IPage* target, uint8_t iconIndex = -1, int actionID = -1);
    void onEvent(AppEvent* event) override;
    void draw(U8G2* u8g2) override;

   private:
    void normalizeCursor();
    void drawIcon(U8G2* u8g2, uint8_t iconIndex);
    void drawScrollbar(U8G2* u8g2);
    void handleSelection(ListItem item);

    const char*           _title;
    std::vector<ListItem> _items;
    int                   _selectedIndex;
    int                   _scrollOffset;
    const int             LINE_HEIGHT      = 12;
    const int             HEADER_HEIGHT    = 16;
    const int             ITEMS_PER_SCREEN = 4;
};