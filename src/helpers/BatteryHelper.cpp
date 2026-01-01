#include "helpers/BatteryHelper.h"

void BatteryHelper::init() {
    analogReadResolution(12);
    pinMode(PIN_BATTERY_ADC, INPUT);
}

int BatteryHelper::getLevel() {
    int raw = analogRead(PIN_BATTERY_ADC);
    int pct = map(raw, 0, 4095, 0, 100);

    return constrain(pct, 0, 100);
}

float BatteryHelper::getVoltage() {
    int raw = analogRead(PIN_BATTERY_ADC);

    return (raw / 4095.0) * 4.2;
}

bool BatteryHelper::isCharging() { return false; }