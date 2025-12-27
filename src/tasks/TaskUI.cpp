#include "TaskUI.h"

ShutdownPopup shutdownPopup;

void taskUI(void* pvParameters) {
    DisplayDriver display(PIN_OLED_SDA, PIN_OLED_SCL);
    IPage*        currentPage   = nullptr;
    IPage*        lastDrawnPage = nullptr;
    IPage*        realPage      = nullptr;
    bool          needRedraw    = true;
    display.init();

    PageManager::getInstance()->pushPage(&PageRegistry::bootPage);
    // TODO : remove this line when boot page is complete
    PageManager::getInstance()->pushPage(&PageRegistry::mainMenuPage);

    AppEvent e;

    while (true) {
        currentPage = PageManager::getInstance()->getCurrentPage();
        while (xQueueReceive(eventQueue, &e, 0)) {
            if (e.type == EVENT_SHUTDOWN_POPUP) {
                shutdownPopup.show();
                needRedraw = true;
                continue;
            }
            if (shutdownPopup.onEvent(&e)) {
                needRedraw = true;
                continue;
            }
            if (currentPage) {
                currentPage->onEvent(&e);
                needRedraw = true;
            }
            realPage = PageManager::getInstance()->getCurrentPage();
            if (realPage != lastDrawnPage) {
                currentPage = realPage;
                needRedraw  = true;
            }
        }

        if (currentPage == &PageRegistry::bootPage) {
            // TODO : manage redirection to main menu in bootPage class
            static long startBoot = millis();
            if (millis() - startBoot > 3000) {
                PageManager::getInstance()->switchPage(&PageRegistry::mainMenuPage);
            }
        }

        if (needRedraw) {
            if (currentPage) {
                currentPage->draw(display.getU8g2());
                lastDrawnPage = currentPage;
            } else {
                PageRegistry::errorPage.setError("No Page Loaded!");
                PageManager::getInstance()->pushPage(&PageRegistry::errorPage);
            }
            if (shutdownPopup.active()) {
                shutdownPopup.draw(display.getU8g2());
            }
            display.getU8g2()->sendBuffer();
            needRedraw = false;
        }

        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}