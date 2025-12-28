#include "PageIR.hpp"

PageIR::PageIR() {
    lastCodeType = "None";
    lastCodeHex  = "Waiting...";
}

void PageIR::onEnter() {
    lastCodeType = "None";
    lastCodeHex  = "Waiting...";
    setLEDMode(LED_MODE_OFF);
    setLEDColor(255, 0, 0);
}

void PageIR::onEvent(AppEvent* event) {
    if (event->type == EVENT_IR_RX) {
        playSound(SOUND_SUCCESS);
        setLEDMode(LED_MODE_TIMEOUT, 100);

        char buffer[32];
        sprintf(buffer, "0x%08X", event->value);
        lastCodeHex  = String(buffer);
        lastCodeType = "NEC/Raw";
    }

    if (event->type == EVENT_BUTTON_CLICK) {
        if (event->value == BTN_OK) {
            IrCommand cmd;
            cmd.command = 0xFFA25D;
            xQueueSend(irQueue, &cmd, 0);

            lastCodeHex = "Sent: Power";
            playSound(SOUND_TICK);
        } else if (event->value == BTN_BACK) {
            PageManager::getInstance()->popPage();
        }
    }
}

void PageIR::draw(U8G2* u8g2) {
    u8g2->clearBuffer();
    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, 128, 16);
    u8g2->setDrawColor(0);
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->drawStr(4, 11, "Infrared Remote");
    u8g2->setDrawColor(1);
    u8g2->drawStr(0, 30, "Last Received:");
    u8g2->setFont(u8g2_font_ncenB08_tr);
    u8g2->drawStr(10, 45, lastCodeHex.c_str());
    u8g2->setFont(u8g2_font_5x7_tf);
    u8g2->drawFrame(0, 52, 128, 12);
    u8g2->drawStr(4, 60, "[OK] Send Test Signal");
    u8g2->sendBuffer();
}