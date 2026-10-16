#include "System.hpp"

QueueHandle_t eventQueue;
QueueHandle_t ledQueue;
QueueHandle_t audioQueue;
QueueHandle_t irQueue;
QueueHandle_t hapticQueue;

IrSignal System::lastCapturedSignal = {UNKNOWN, 0, 0, 0};

void createTestFiles() {
    Serial.println("[System] Initializing Storage...");
    if (StorageManager::getInstance()->init()) {
        Serial.println("[System] Storage Init SUCCESS.");
    } else {
        Serial.println("[System] WARNING: Storage Init Failed (No SD card).");
    }
    Serial.println("[System] System Initialized.");
}

void System::init() {
    Serial.begin(115200);
    eventQueue  = xQueueCreate(20, sizeof(AppEvent));
    ledQueue    = xQueueCreate(5, sizeof(LedCommand));
    audioQueue  = xQueueCreate(10, sizeof(SoundType));
    irQueue     = xQueueCreate(5, sizeof(IrSignal));
    hapticQueue = xQueueCreate(5, sizeof(HapticCommand));
    BatteryHelper::init();
    createTestFiles();
}

void System::startTasks() {
    xTaskCreate(taskUI, "UITask", 4096, NULL, 1, NULL);
    xTaskCreate(taskInput, "InputTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskLed, "LedTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskAudio, "AudioTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskIR, "IRTask", 4096, NULL, 5, NULL);
    xTaskCreate(taskHaptic, "HapticTask", 2048, NULL, 1, NULL);
}