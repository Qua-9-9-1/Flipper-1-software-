#include "TaskUI.h"

extern QueueHandle_t eventQueue;
extern PageMainMenu  mainMenu;
extern PageBoot      bootPage;

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
        if (xQueueReceive(eventQueue, &e, 0)) {
            if (currentPage) {
                currentPage->onEvent(&e);
                needRedraw = true;
            }
        }

        if (PageManager::getInstance()->getCurrentPage() == &bootPage) {
            // TODO : manage boot in bootPage class switch
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
            needRedraw = false;
        }
        vTaskDelay(30 / portTICK_PERIOD_MS);
    }
}