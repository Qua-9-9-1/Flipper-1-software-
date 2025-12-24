#include <Arduino.h>
#include "System.hpp"
#include "PageRegistry.hpp"

void setup() {
    System::init();
    PageRegistry::setupPages();
    System::startTasks();
}

void loop() {
    // Delete the default loop task as we are using FreeRTOS tasks
    vTaskDelete(NULL);
}