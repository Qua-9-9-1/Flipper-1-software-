#pragma once
#include <Arduino.h>

#include "PinDefinitions.h"

class BatteryHelper {
   public:
    static void  init();
    static int   getLevel();
    static float getVoltage();
    static bool  isCharging();
};