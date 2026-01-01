#include "TaskHaptic.h"

void taskHaptic(void* pvParameters) {
    HapticCommand cmd;

    pinMode(PIN_VIBRO, OUTPUT);
    digitalWrite(PIN_VIBRO, LOW);
    while (true) {
        if (xQueueReceive(hapticQueue, &cmd, portMAX_DELAY)) {
            if (cmd.duration > 0) {
                digitalWrite(PIN_VIBRO, HIGH);
                vTaskDelay(cmd.duration / portTICK_PERIOD_MS);
                digitalWrite(PIN_VIBRO, LOW);
            }
        }
    }
}