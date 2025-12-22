#include "TaskRadio.h"

extern QueueHandle_t eventQueue;

void taskDummyRadio(void *pvParameters) {
    while(true) {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        AppEvent e = {EVENT_RADIO, (int)random(100)};
        xQueueSend(eventQueue, &e, portMAX_DELAY);
    }
}