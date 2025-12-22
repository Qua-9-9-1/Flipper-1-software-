#include "TaskUI.h"
#include "DisplayDriver.h"
#include "AppEvents.h"
#include "PinDefinitions.h"
#include "PageControlTest.h"
#include "PageMainMenu.h"
#include "PageList.h"
#include "PageBoot.h"
#include "Page.h"

extern QueueHandle_t eventQueue;

PageMainMenu mainMenu;
PageList subGhzMenu("Sub-GHz");
PageList nfcMenu("NFC / RFID");
PageList IR("IR");
PageList badUSB("System / BadUSB");
PageList settings("Settings");

PageBoot bootPage;
PageControlTest controlTestPage;

void setupMenus() {
    subGhzMenu.addItem("Read / Sniff", nullptr);
    subGhzMenu.addItem("Saved", nullptr);
    subGhzMenu.addItem("Add Manually", nullptr);

    nfcMenu.addItem("Read Card", nullptr);
    nfcMenu.addItem("Emulate", nullptr);

    IR.addItem("Universal Remote", nullptr);
    IR.addItem("Capture Signal", nullptr);
    IR.addItem("Saved Signals", nullptr);

    badUSB.addItem("Windows", nullptr, 0);
    badUSB.addItem("macOS", nullptr, 1);
    badUSB.addItem("Linux", nullptr);
    // badUSB.addItem("Payloads", nullptr);
    // badUSB.addItem("Create Payload", nullptr);

    settings.addItem("Display", nullptr);
    settings.addItem("Sound", nullptr);
    settings.addItem("System Info", nullptr);
    settings.addItem("About", nullptr);
    settings.addItem("Controls Test", &controlTestPage);

    mainMenu.addIcon(ICON_RADIO, &subGhzMenu);
    mainMenu.addIcon(ICON_NFC, &nfcMenu);
    mainMenu.addIcon(ICON_IR, &IR);
    mainMenu.addIcon(ICON_USB, &badUSB);
    mainMenu.addIcon(ICON_SETTINGS, &settings);
}

void taskUI(void *pvParameters) {
    DisplayDriver display(PIN_OLED_SDA, PIN_OLED_SCL);
    IPage* currentPage = nullptr;
    display.init();
    setupMenus();

    PageManager::getInstance()->pushPage(&bootPage);
    // TODO : remove this line when boot page is complete
    PageManager::getInstance()->pushPage(&mainMenu);

    AppEvent e;
    
    while (true) {
        currentPage = PageManager::getInstance()->getCurrentPage();
        if (xQueueReceive(eventQueue, &e, 0)) { 
             if (currentPage) {
                 currentPage->onEvent(&e);
             }
        }

        if (PageManager::getInstance()->getCurrentPage() == &bootPage) {
            // TODO : manage boot in bootPage class switch
            static long startBoot = millis();
            if (millis() - startBoot > 3000) {
                PageManager::getInstance()->switchPage(&mainMenu);
            }
        }

        if (currentPage) {
            currentPage->draw(display.getU8g2());
        } else {
            display.showError(NO_PAGE);
        }
        
        vTaskDelay(30 / portTICK_PERIOD_MS);
    }
}