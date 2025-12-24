#pragma once
#include <Arduino.h>
#include "AppEvents.h"

extern QueueHandle_t audioQueue;

void playSound(SoundType sound);
