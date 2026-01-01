#include <vector>

#include "AppEvents.h"
#include "Assets.h"
#include "AudioHelper.h"
#include "BatteryHelper.h"
#include "LedHelper.h"
#include "Page.hpp"
#include "PageManager.hpp"
#include "ShutdownPopup.hpp"

enum Icon { GHZ, NFC_RFID, IR, USB, SETTINGS, POWEROFF };

class PageMainMenu : public IPage {
   public:
    PageMainMenu();
    ~PageMainMenu();
    void onEnter() override;
    void onEvent(AppEvent* event) override;
    void draw(U8G2* u8g2) override;
    void addIcon(Icon icon, IPage* linkedPage);

   private:
    void normalizePos();
    void drawPageHeader(U8G2* u8g2, char const* title);
    void drawBattery(U8G2* u8g2, int& cursorX);
    void drawUSB(U8G2* u8g2, int& cursorX);
    void drawSD(U8G2* u8g2, int& cursorX);
    void drawSound(U8G2* u8g2, int& cursorX);

    int                                 _pos[2];
    std::vector<std::pair<int, IPage*>> _linkedPages;
};