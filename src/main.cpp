#include <Arduino.h>

#include "PageRegistry.hpp"
#include "System.hpp"
#include "managers/StorageManager.hpp"

void setup() {
    System::init();
    PageRegistry::setupPages();
    System::startTasks();

    if (StorageManager::getInstance()->init()) {
        if (!SD.exists("/Flipper-1/badusb/demo_windows.txt")) {
            Serial.println("[System] Generating Dummy Files for Simulator...");
            StorageManager::getInstance()->writeFile("/Flipper-1/badusb/demo_windows.txt",
                                                     "DELAY 500\nSTRING Hello Windows!");
            StorageManager::getInstance()->writeFile("/Flipper-1/badusb/mac_hack.txt",
                                                     "DELAY 200\nGUI SPACE\nSTRING Terminal");
            StorageManager::getInstance()->writeFile("/Flipper-1/badusb/rickroll.txt",
                                                     "STRING never gonna give you up");
        }
    }
}

void loop() { vTaskDelete(NULL); }