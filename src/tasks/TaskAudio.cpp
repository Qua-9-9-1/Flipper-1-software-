#include "TaskAudio.h"

void taskAudio(void* pvParameters) {
    BuzzerDriver buzzer(PIN_BUZZER);
    SoundType    soundToPlay;

    buzzer.init();
    buzzer.tone(1000, 100);
    vTaskDelay(150 / portTICK_PERIOD_MS);
    buzzer.tone(2000, 100);
    vTaskDelay(150 / portTICK_PERIOD_MS);
    buzzer.noTone();
    while (true) {
        if (xQueueReceive(audioQueue, &soundToPlay, portMAX_DELAY)) {
            buzzer.playSound(soundToPlay);
            vTaskDelay(buzzer.getSoundDuration(soundToPlay) / portTICK_PERIOD_MS);
            buzzer.noTone();
        }
    }
}