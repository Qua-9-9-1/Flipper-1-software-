#include "TaskIR.h"

const uint16_t kCaptureBufferSize = 1024;
const uint8_t  kTimeout           = 50;

IRrecv irrecv(PIN_IR_RX, kCaptureBufferSize, kTimeout, true);
IRsend irsend(PIN_IR_TX);

decode_results results;

extern QueueHandle_t eventQueue;
extern QueueHandle_t irQueue;

String getProtocolName(decode_type_t type) {
    switch (type) {
        case NEC:
            return "NEC";
        case SONY:
            return "SONY";
        case RC5:
            return "RC5";
        case RC6:
            return "RC6";
        case SAMSUNG:
            return "SAMSUNG";
        case UNKNOWN:
            return "RAW/UNKNOWN";
        default:
            return "OTHER";
    }
}

void taskIR(void* pvParameters) {
    IrSignal signalToSend;
    AppEvent e;

    pinMode(PIN_IR_RX, INPUT_PULLUP);
    irrecv.enableIRIn();
    irsend.begin();
    while (true) {
        if (xQueueReceive(irQueue, &signalToSend, 0)) {
            Serial.printf("[TaskIR] Replaying Protocol %d, Value %llX\n", signalToSend.protocol,
                          signalToSend.value);
            irrecv.disableIRIn();
            delay(10);
            switch (signalToSend.protocol) {
                case NEC:
                    irsend.sendNEC(signalToSend.value, signalToSend.bits);
                    break;
                case SONY:
                    irsend.sendSony(signalToSend.value, signalToSend.bits);
                    break;
                case SAMSUNG:
                    irsend.sendSamsung36(signalToSend.value, signalToSend.bits);
                    break;
                case RC5:
                    irsend.sendRC5(signalToSend.value, signalToSend.bits);
                    break;
                case RC6:
                    irsend.sendRC6(signalToSend.value, signalToSend.bits);
                    break;
                default:
                    Serial.println("Protocol not supported for replay, forcing NEC");
                    irsend.sendNEC(signalToSend.value, 32);
                    break;
            }
            delay(20);
            irrecv.enableIRIn();
        }
        if (irrecv.decode(&results)) {
            if (!results.repeat && results.bits > 5) {
                Serial.printf("[TaskIR] Captured: Proto=%d, Val=%llX, Bits=%d\n",
                              results.decode_type, results.value, results.bits);
                System::lastCapturedSignal.protocol = results.decode_type;
                System::lastCapturedSignal.value    = results.value;
                System::lastCapturedSignal.bits     = results.bits;
                System::lastCapturedSignal.address  = results.address;
                e.type                              = EVENT_IR_RX;
                e.value                             = (uint32_t)results.value;
                xQueueSend(eventQueue, &e, 0);
            }
            irrecv.resume();
        }
        vTaskDelay(20 / portTICK_PERIOD_MS);
    }
}