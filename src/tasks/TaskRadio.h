#pragma once
#include <Arduino.h>

#include "AppEvents.h"

extern QueueHandle_t eventQueue;

void taskDummyRadio(void* pvParameters);
