#include "LedHelper.h"

extern QueueHandle_t ledQueue;

void setLedColor(uint8_t r, uint8_t g, uint8_t b) {
    LedCommand cmd;

    cmd.mode  = LED_MODE_SET_COLOR;
    cmd.value = (r << 16) | (g << 8) | b;
    xQueueSend(ledQueue, &cmd, 0);
}

void setLedMode(LedMode mode, int val) {
    LedCommand cmd;

    cmd.mode  = mode;
    cmd.value = val;
    xQueueSend(ledQueue, &cmd, 0);
}
