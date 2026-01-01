#include "TaskLed.h"

void taskLed(void* pvParameters) {
    LedDriver  led(PIN_LED);
    LedMode    currentMode = LED_MODE_BOOT;
    LedCommand cmd;
    int        batteryLevel = 100;

    led.init();
    while (true) {
        if (xQueueReceive(ledQueue, &cmd, 0) == pdTRUE) {
            if (cmd.mode == LED_MODE_SET_COLOR) {
                uint8_t r = (cmd.value >> 16) & 0xFF;
                uint8_t g = (cmd.value >> 8) & 0xFF;
                uint8_t b = cmd.value & 0xFF;
                led.setColor(r, g, b);
            } else if (cmd.mode == LED_MODE_SET_BRIGHTNESS) {
                led.setBrightness(cmd.value);
            } else if (cmd.mode == LED_MODE_SET_FADE_SPEED) {
                led.setFadeSpeed(cmd.value);
            } else if (cmd.mode == LED_MODE_TIMEOUT) {
                led.startTimeout(cmd.value);
                currentMode = LED_MODE_TIMEOUT;
            } else {
                currentMode = cmd.mode;
            }
        }
        led.tick(currentMode, BatteryHelper::getLevel());
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
