#include <Arduino.h>
#include "AppEvents.h"
#include "tasks/TaskUI.h"
#include "tasks/TaskInput.h"
#include "tasks/TaskRadio.h"

QueueHandle_t eventQueue;

void setup() {
    Serial.begin(115200);
    eventQueue = xQueueCreate(10, sizeof(AppEvent));

    xTaskCreate(taskUI, "UITask", 4096, NULL, 1, NULL);
    xTaskCreate(taskInput, "InputTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskDummyRadio, "RadioTask", 2048, NULL, 1, NULL);
}

void loop() {
    vTaskDelay(1000 / portTICK_PERIOD_MS);
}