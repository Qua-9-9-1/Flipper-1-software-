#include "LedHelper.h"

extern QueueHandle_t ledQueue;

void setLEDColor(uint8_t r, uint8_t g, uint8_t b) {
    LedCommand cmd;

    cmd.mode  = LED_MODE_SET_COLOR;
    cmd.value = (r << 16) | (g << 8) | b;
    xQueueSend(ledQueue, &cmd, 0);
}

void setLEDBrightness(uint8_t brightness) {
    LedCommand cmd;

    cmd.mode  = LED_MODE_SET_BRIGHTNESS;
    cmd.value = brightness;
    xQueueSend(ledQueue, &cmd, 0);
}

void setLEDFadeSpeed(uint8_t speed) {
    LedCommand cmd;

    cmd.mode  = LED_MODE_SET_FADE_SPEED;
    cmd.value = speed;
    xQueueSend(ledQueue, &cmd, 0);
}

void setLEDMode(LedMode mode, int val) {
    LedCommand cmd;

    cmd.mode  = mode;
    cmd.value = val;
    xQueueSend(ledQueue, &cmd, 0);
}
