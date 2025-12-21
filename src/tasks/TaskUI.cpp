#include "TaskUI.h"
#include "DisplayDriver.h"
#include "AppEvents.h"
#include "PinDefinitions.h"
#include "PageControlTest.h"
#include "Page.h"

extern QueueHandle_t eventQueue;

void taskUI(void *pvParameters) {
    DisplayDriver display(PIN_OLED_SDA, PIN_OLED_SCL);
    display.init();
    // display.showBootScreen();
    
    // vTaskDelay(1000 / portTICK_PERIOD_MS);

    // Instanciation des pages
    PageControlTest pageControls;
    // PageMenu pageMenu; 
    
    IPage* currentPage = &pageControls;
    currentPage->onEnter();

    AppEvent e;

    while (true) {
        if (xQueueReceive(eventQueue, &e, 0)) { // 0 for non blocking event
             currentPage->onEvent(&e);             
        }

        currentPage->draw(display.getU8g2());       
        vTaskDelay(30 / portTICK_PERIOD_MS);
    }
}