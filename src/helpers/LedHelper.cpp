#include "LedHelper.h"

extern QueueHandle_t ledQueue;

void setLedMode(LedMode mode, int val) {
    LedCommand cmd;

    cmd.mode  = mode;
    cmd.value = val;
    xQueueSend(ledQueue, &cmd, 0);
}