#include "IR/PageIR.hpp"

PageIR::PageIR() : _hasSignal(false), _protoName("Waiting...") {
    _storedSignal = {UNKNOWN, 0, 0, 0};
}

void PageIR::onEnter() {
    _hasSignal = false;
    _protoName = "Waiting...";
    setLEDMode(LED_MODE_OFF);
    setLEDColor(255, 0, 0);
}

void PageIR::onEvent(AppEvent* event) {
    if (event->type == EVENT_IR_RX) {
        _storedSignal = System::lastCapturedSignal;
        _hasSignal    = true;
        playSound(SOUND_SUCCESS);
        setLEDColor(0, 0, 255);
        setLEDMode(LED_MODE_TIMEOUT, 100);
        switch (_storedSignal.protocol) {
            case NEC:
                _protoName = "NEC";
                break;
            case SONY:
                _protoName = "SONY";
                break;
            case RC5:
                _protoName = "RC5";
                break;
            case SAMSUNG:
                _protoName = "SAMSUNG";
                break;
            default:
                _protoName = "UNKNOWN";
                break;
        }
    }
    if (event->type == EVENT_BUTTON_CLICK) {
        if (event->value == BTN_OK) {
            if (_hasSignal) {
                xQueueSend(irQueue, &_storedSignal, 0);
                playSound(SOUND_TICK);
                setLEDColor(255, 0, 0);
                setLEDMode(LED_MODE_TIMEOUT, 100);
            } else {
                playSound(SOUND_ERROR);
            }
        } else if (event->value == BTN_BACK) {
            PageManager::getInstance()->popPage();
        }
    }
}

void PageIR::draw(U8G2* u8g2) {
    u8g2->clearBuffer();
    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, 128, 12);
    u8g2->setDrawColor(0);
    u8g2->setFont(u8g2_font_5x7_tf);
    u8g2->drawStr(2, 9, "IR Cloner");
    u8g2->setDrawColor(1);
    if (!_hasSignal) {
        u8g2->setFont(u8g2_font_6x10_tf);
        u8g2->drawStr(20, 35, "Waiting for");
        u8g2->drawStr(35, 48, "Remote...");
        u8g2->drawFrame(10, 55, 108, 2);
    } else {
        u8g2->setFont(u8g2_font_6x10_tf);
        u8g2->drawStr(0, 24, "Proto:");
        u8g2->drawStr(40, 24, _protoName.c_str());
        u8g2->setFont(u8g2_font_ncenB08_tr);
        char hexBuf[20];
        sprintf(hexBuf, "0x%llX", _storedSignal.value);
        u8g2->drawStr(10, 42, hexBuf);
        u8g2->setFont(u8g2_font_5x7_tf);
        char bitBuf[20];
        sprintf(bitBuf, "%d bits", _storedSignal.bits);
        u8g2->drawStr(80, 24, bitBuf);
        u8g2->drawFrame(0, 50, 128, 14);
        u8g2->drawBox(0, 50, 128, 14);
        u8g2->setDrawColor(0);
        u8g2->drawStr(25, 60, "[OK] REPLAY");
    }
}