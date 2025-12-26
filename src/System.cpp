#include "System.hpp"

QueueHandle_t eventQueue;
QueueHandle_t ledQueue;
QueueHandle_t audioQueue;

void createTestFiles() {
    Serial.println("[System] Initializing Storage...");

    if (StorageManager::getInstance()->init()) {
        Serial.println("[System] Storage Init SUCCESS.");

        if (!SD.exists("/Flipper-1/badusb")) {
            Serial.println("[System] Creating missing folder /Flipper-1/badusb");
            SD.mkdir("/Flipper-1/badusb");
        }

        Serial.println("[System] Writing demo files...");
        bool s1 = StorageManager::getInstance()->writeFile("/Flipper-1/badusb/demo.txt",
                                                           "STRING Hello World");
        if (s1)
            Serial.println("[System] Wrote demo.txt: OK");
        else
            Serial.println("[System] Wrote demo.txt: FAILED");

        bool s2 = StorageManager::getInstance()->writeFile("/Flipper-1/badusb/hack.txt",
                                                           "GUI r\nDELAY 200\nSTRING cmd");
        if (s2)
            Serial.println("[System] Wrote hack.txt: OK");
        else
            Serial.println("[System] Wrote hack.txt: FAILED");

    } else {
        Serial.println("[System] CRITICAL: Storage Init FAILED. Check Pins!");
    }

    Serial.println("System Initialized.");
}

void System::init() {
    Serial.begin(115200);
    eventQueue = xQueueCreate(20, sizeof(AppEvent));
    ledQueue   = xQueueCreate(5, sizeof(LedCommand));
    audioQueue = xQueueCreate(10, sizeof(SoundType));
    createTestFiles();
}

void System::startTasks() {
    xTaskCreate(taskUI, "UITask", 4096, NULL, 1, NULL);
    xTaskCreate(taskInput, "InputTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskLed, "LedTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskAudio, "AudioTask", 2048, NULL, 1, NULL);
    xTaskCreate(taskDummyRadio, "RadioTask", 2048, NULL, 1, NULL);
}