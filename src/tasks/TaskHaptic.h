#pragma once
#include <Arduino.h>

#include "AppEvents.h"
#include "PinDefinitions.h"

extern QueueHandle_t hapticQueue;

void taskHaptic(void* pvParameters);
