#include "TaskUI.h"
#include "DisplayDriver.h"
#include "AppEvents.h"
#include "PinDefinitions.h"

extern QueueHandle_t eventQueue;

void taskUI(void *pvParameters) {
    DisplayDriver display(PIN_OLED_SDA, PIN_OLED_SCL);
    
    display.init();
    display.showBootScreen();
    vTaskDelay(1000 / portTICK_PERIOD_MS);

    AppEvent receivedEvent;

    while (true) {
        if (xQueueReceive(eventQueue, &receivedEvent, portMAX_DELAY)) {
            display.updateStatus(receivedEvent.type, receivedEvent.value);
        }
    }
}