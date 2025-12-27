#include "TaskIR.h"

const uint16_t kCaptureBufferSize = 1024;
const uint8_t  kTimeout           = 50;

IRrecv irrecv(PIN_IR_RX, kCaptureBufferSize, kTimeout, true);
IRsend irsend(PIN_IR_TX);

decode_results results;

extern QueueHandle_t eventQueue;
extern QueueHandle_t irQueue;

void taskIR(void* pvParameters) {
    Serial.println("[TaskIR] Starting Library Mode...");
    pinMode(PIN_IR_RX, INPUT_PULLUP);
    irrecv.enableIRIn();
    irsend.begin();

    Serial.printf("[TaskIR] Ready on RX: %d, TX: %d\n", PIN_IR_RX, PIN_IR_TX);

    IrCommand cmdToSend;

    while (true) {
        if (xQueueReceive(irQueue, &cmdToSend, 0)) {
            Serial.printf("[TaskIR] Sending NEC: %08X\n", cmdToSend.command);
            irsend.sendNEC(cmdToSend.command, 32);
            delay(20);
            irrecv.enableIRIn();
        }

        if (irrecv.decode(&results)) {
            if (results.bits > 10 && !results.repeat) {
                Serial.printf("[TaskIR] Decoded: 0x%llX (Type: %d)\n", results.value,
                              results.decode_type);
                AppEvent e;
                e.type  = EVENT_IR_RX;
                e.value = (int)results.value;
                xQueueSend(eventQueue, &e, 0);
            }
            irrecv.resume();
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}