#pragma once
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRutils.h>

#include "AppEvents.h"
#include "PinDefinitions.h"
#include "System.hpp"

extern QueueHandle_t eventQueue;
extern QueueHandle_t irQueue;

void taskIR(void* pvParameters);