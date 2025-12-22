#include "Page.h"
#include <vector>
#include "PageManager.h"
#include "Assets.h"

struct ListItem {
    const char* title;
    IPage* targetPage;
    uint8_t iconIndex;
    // int actionID; // to do an action instead of navigating
};

struct icon_list_t {
    const unsigned char* bitmap;
    const uint8_t width;
    const uint8_t height;
};

static icon_list_t ListIcons[] = {
    { icon_windows_bits, SYSTEM_ICON_DIM, SYSTEM_ICON_DIM },
    { icon_macos_bits, SYSTEM_ICON_DIM, SYSTEM_ICON_DIM }
};

class PageList : public IPage {
    public:
        PageList(const char* t);
        void addItem(const char* name, IPage* target, uint8_t iconIndex = -1);
        void onEvent(AppEvent *event) override;
        void draw(U8G2 *u8g2) override;
        void onEnter() override {}
    private:
        void drawIcon(U8G2 *u8g2, uint8_t iconIndex);

        const char* title;
        std::vector<ListItem> items;
        int selectedIndex;
        int scrollOffset;
};