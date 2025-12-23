#include <vector>
#include "Page.h"
#include "PageManager.h"
#include "AppEvents.h"
#include "Assets.h"
#include "helpers/LedHelper.h"

#define COLS 4
#define ROWS 2

class PageMainMenu : public IPage {
    public:
        PageMainMenu();
        ~PageMainMenu();
        void onEnter() override;
        void onEvent(AppEvent *event) override;
        void draw(U8G2 *u8g2) override;
        void addIcon(int icon, IPage* linkedPage);
    private:
        void normalizePos();

        int _pos[2];
        std::vector<std::pair<int, IPage*>> _linkedPages;
        const unsigned char* _icons[6] = {
            icon_ghz,
            icon_nfc_rfid,
            icon_ir,
            icon_usb,
            icon_settings,
            icon_poweroff
        };
};