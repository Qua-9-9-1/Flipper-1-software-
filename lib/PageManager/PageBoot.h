#include "Page.h"
#include "PageManager.h"
#include "AppEvents.h"

class PageBoot : public IPage {
    public:
        PageBoot();
        void onEnter() override;
        void onEvent(AppEvent *e) override {}
        void draw(U8G2 *u8g2) override;
    private:
        int _progress;
        unsigned long _lastUpdate;
};