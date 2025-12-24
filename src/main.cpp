#include <Arduino.h>

#include "PageRegistry.hpp"
#include "System.hpp"

void setup() {
    System::init();
    PageRegistry::setupPages();
    System::startTasks();
}

void loop() {
    // Delete the default loop task as we are using FreeRTOS tasks
    vTaskDelete(NULL);
}