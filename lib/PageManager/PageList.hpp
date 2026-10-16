#pragma once
#include <functional>
#include <vector>

#include "Assets.h"
#include "Page.hpp"
#include "PageActions.hpp"
#include "PageManager.hpp"
#include "helpers/AudioHelper.h"
#include "helpers/LedHelper.h"

struct ItemOption {
    const char* name;
    PageAction  action;

    ItemOption(const char* optName = "", PageAction act = nullptr)
        : name(optName), action(act) {}

    ItemOption(PageAction act)
        : name(""), action(act) {}
};

struct ListItem {
    const char*             title;
    std::vector<ItemOption> options;
    uint8_t                 iconIndex;
    int                     currentOptionIndex;
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
    void addItem(const char* name, std::vector<ItemOption> options = {});
    void addItemWithIcon(const char* name, std::vector<ItemOption> options = {}, uint8_t iconIndex = 0);
    void onEvent(AppEvent* event) override;
    void draw(U8G2* u8g2) override;

   private:
    void normalizeCursor();
    void drawIcon(U8G2* u8g2, uint8_t iconIndex);
    void drawScrollbar(U8G2* u8g2);
    void handleSelection(const ItemOption& item);

    const char*           _title;
    std::vector<ListItem> _items;
    int                   _selectedIndex;
    int                   _scrollOffset;
};