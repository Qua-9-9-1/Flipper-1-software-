#include <Arduino.h>

#include "PageRegistry.hpp"
#include "System.hpp"
#include "managers/StorageManager.hpp"

void setup() {
    System::init();
    PageRegistry::setupPages();
    System::startTasks();
}

void loop() { vTaskDelete(NULL); }