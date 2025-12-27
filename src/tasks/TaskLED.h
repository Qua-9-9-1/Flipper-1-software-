#pragma once
#include "AppEvents.h"
#include "LedDriver.hpp"
#include "PinDefinitions.h"

extern QueueHandle_t ledQueue;

void taskLed(void* pvParameters);
