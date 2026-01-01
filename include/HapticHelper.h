#pragma once
#include <Arduino.h>

#include "AppEvents.h"

extern QueueHandle_t hapticQueue;

void playHaptic(int durationMs);
