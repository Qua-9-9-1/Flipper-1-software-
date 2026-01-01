#include "HapticHelper.h"

void playHaptic(int durationMs) {
    if (hapticQueue != NULL) {
        HapticCommand cmd;
        cmd.duration = durationMs;
        xQueueSend(hapticQueue, &cmd, 0);
    }
}