#pragma once
#include <OneButton.h>

#include "AppEvents.h"
#include "PinDefinitions.h"

extern QueueHandle_t eventQueue;

void taskInput(void* pvParameters);