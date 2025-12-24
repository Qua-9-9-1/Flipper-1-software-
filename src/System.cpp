#include "System.h"

QueueHandle_t eventQueue;
QueueHandle_t ledQueue;
QueueHandle_t audioQueue;

void System::init() {
    Serial.begin(115200);    
    eventQueue = xQueueCreate(20, sizeof(AppEvent));
    ledQueue = xQueueCreate(5, sizeof(LedCommand));
    audioQueue = xQueueCreate(10, sizeof(SoundType));
}

void System::startTasks() {
    xTaskCreate(taskUI, "UITask", 4096, NULL, 1, NULL);
    xTaskCreate(taskInput, "InputTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskLed, "LedTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskAudio, "AudioTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskDummyRadio, "RadioTask", 2048, NULL, 1, NULL);
}