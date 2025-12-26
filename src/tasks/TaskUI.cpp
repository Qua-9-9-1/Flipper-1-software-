#include "TaskUI.h"

extern QueueHandle_t eventQueue;
extern PageMainMenu  mainMenu;
extern PageBoot      bootPage;

ShutdownPopup shutdownPopup;

void taskUI(void* pvParameters) {
    DisplayDriver display(PIN_OLED_SDA, PIN_OLED_SCL);
    IPage*        currentPage = nullptr;
    bool          needRedraw  = true;
    display.init();

    PageManager::getInstance()->pushPage(&bootPage);
    // TODO : remove this line when boot page is complete
    PageManager::getInstance()->pushPage(&mainMenu);

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
        }

        if (currentPage == &bootPage) {
            // TODO : manage redirection to main menu in bootPage class
            static long startBoot = millis();
            if (millis() - startBoot > 3000) {
                PageManager::getInstance()->switchPage(&mainMenu);
            }
        }

        if (needRedraw) {
            if (currentPage) {
                currentPage->draw(display.getU8g2());
            } else {
                display.showError(NO_PAGE);
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