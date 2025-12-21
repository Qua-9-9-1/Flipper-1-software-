#include <Arduino.h>
#include "AppEvents.h"
#include "tasks/TaskUI.h"

QueueHandle_t eventQueue;

void taskDummyRadio(void *pvParameters) {
    while(true) {
        vTaskDelay(2000 / portTICK_PERIOD_MS);
        AppEvent e = {EVENT_RADIO, (int)random(100)};
        xQueueSend(eventQueue, &e, portMAX_DELAY);
    }
}

void setup() {
    Serial.begin(115200);
    eventQueue = xQueueCreate(10, sizeof(AppEvent));

    xTaskCreate(taskUI, "UITask", 4096, NULL, 1, NULL);
    xTaskCreate(taskDummyRadio, "RadioTask", 2048, NULL, 1, NULL);
}

void loop() {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}