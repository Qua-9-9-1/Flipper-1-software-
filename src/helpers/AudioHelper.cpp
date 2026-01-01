#include "helpers/AudioHelper.h"

void playSound(SoundType sound) {
    if (audioQueue != NULL) {
        xQueueSend(audioQueue, &sound, 0);
    }
}