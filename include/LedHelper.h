#pragma once
#include "AppEvents.h"

void setLEDColor(uint8_t r, uint8_t g, uint8_t b);
void setLEDBrightness(uint8_t brightness);
void setLEDFadeSpeed(uint8_t speed);
void setLEDMode(LedMode mode, int val = 0);
