#include "TaskLed.h"

extern QueueHandle_t ledQueue;

void taskLed(void* pvParameters) {
    LedDriver  led(PIN_LED);
    LedMode    currentMode = LED_MODE_BOOT;
    LedCommand cmd;
    int        batteryLevel = 100;

    led.init();
    while (true) {
        if (xQueueReceive(ledQueue, &cmd, 0) == pdTRUE) {
            currentMode = cmd.mode;
            if (currentMode == LED_MODE_BATTERY && cmd.value > 0) {
                batteryLevel = cmd.value;
            }
            // TODO: handle other modes with values if needed
        }
        led.tick(currentMode, batteryLevel);

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
