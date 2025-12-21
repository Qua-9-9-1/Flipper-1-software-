#include "Page.h"
#include <vector>
#include "PageManager.h"

struct ListItem {
    const char* title;
    IPage* targetPage;
    // int actionID; // to do an action instead of navigating
};

class PageList : public IPage {
    public:
        PageList(const char* t);
        void addItem(const char* name, IPage* target);
        void onEvent(AppEvent *event) override;
        void draw(U8G2 *u8g2) override;
        void onEnter() override {}
    private:
        const char* title;
        std::vector<ListItem> items;
        int selectedIndex;
        int scrollOffset;

};