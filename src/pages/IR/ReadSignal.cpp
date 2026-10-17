#include "IR/ReadSignal.hpp"

namespace {
constexpr int      HEADER_HEIGHT             = 13;
constexpr int      SCREEN_WIDTH              = 128;
constexpr uint32_t SEND_FEEDBACK_DURATION_MS = 300;
}  // namespace

ReadSignal::ReadSignal()
    : _storedSignal{UNKNOWN, 0, 0, 0},
      _hasSignal(false),
      _protoName("Waiting..."),
      _lastSendTime(0) {}

void ReadSignal::onEnter() {
    _hasSignal    = false;
    _protoName    = "Waiting...";
    _lastSendTime = 0;
    setLEDMode(LED_MODE_OFF);
    setLEDColor(255, 0, 0);
}

void ReadSignal::updateProtocolName() {
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
        case RC6:
            _protoName = "RC6";
            break;
        case SAMSUNG:
            _protoName = "Samsung";
            break;
        case PANASONIC:
            _protoName = "Panasonic";
            break;
        case LG:
            _protoName = "LG";
            break;
        case JVC:
            _protoName = "JVC";
            break;
        default:
            _protoName = "RAW/Other";
            break;
    }
}

void ReadSignal::onEvent(AppEvent* event) {
    if (event->type == EVENT_IR_RX) {
        _storedSignal = System::lastCapturedSignal;
        _hasSignal    = true;
        updateProtocolName();

        playSound(SOUND_SUCCESS);
        setLEDColor(0, 0, 255);
        setLEDMode(LED_MODE_TIMEOUT, 100);
    }

    if (event->type == EVENT_BUTTON_CLICK) {
        switch (event->value) {
            case BTN_OK:
                if (_hasSignal) {
                    _lastSendTime = millis();
                    xQueueSend(irQueue, &_storedSignal, 0);
                    playSound(SOUND_TICK);
                    setLEDColor(255, 0, 0);
                    setLEDMode(LED_MODE_TIMEOUT, 100);
                } else {
                    playSound(SOUND_ERROR);
                }
                break;

            case BTN_LEFT:
            case BTN_UP:
                if (_hasSignal) {
                    _hasSignal = false;
                    _protoName = "Waiting...";
                    playSound(SOUND_TICK);
                }
                break;

            case BTN_BACK:
                PageManager::getInstance()->popPage();
                break;

            default:
                break;
        }
    }
}

void ReadSignal::drawRemoteIllustration(U8G2* u8g2, int startX, int startY) const {
    // Remote body
    u8g2->drawRFrame(startX, startY, 18, 33, 2);

    // Top IR transmitter diode
    u8g2->drawBox(startX + 7, startY - 2, 4, 2);

    // Power button
    u8g2->drawDisc(startX + 9, startY + 5, 2, U8G2_DRAW_ALL);

    // D-Pad navigation cross
    u8g2->drawBox(startX + 6, startY + 14, 6, 2);
    u8g2->drawBox(startX + 8, startY + 12, 2, 6);

    // Numeric keypad dots
    u8g2->drawPixel(startX + 5, startY + 23);
    u8g2->drawPixel(startX + 9, startY + 23);
    u8g2->drawPixel(startX + 13, startY + 23);
    u8g2->drawPixel(startX + 5, startY + 27);
    u8g2->drawPixel(startX + 9, startY + 27);
    u8g2->drawPixel(startX + 13, startY + 27);

    // Radiating IR wave animation above the diode
    const int     centerX   = startX + 9;
    const int     topY      = startY - 2;
    const uint8_t wavePhase = static_cast<uint8_t>((millis() / 250) % 4);

    if (wavePhase >= 1) {
        u8g2->drawCircle(centerX, topY, 4, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_UPPER_LEFT);
    }
    if (wavePhase >= 2) {
        u8g2->drawCircle(centerX, topY, 7, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_UPPER_LEFT);
    }
    if (wavePhase >= 3) {
        u8g2->drawCircle(centerX, topY, 10, U8G2_DRAW_UPPER_RIGHT | U8G2_DRAW_UPPER_LEFT);
    }
}

void ReadSignal::drawWaitingScreen(U8G2* u8g2) const {
    // Header Banner
    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, SCREEN_WIDTH, HEADER_HEIGHT);
    u8g2->setDrawColor(0);
    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->drawStr(4, 10, "Learning IR...");

    // Main Body
    u8g2->setDrawColor(1);
    drawRemoteIllustration(u8g2, 12, 26);

    u8g2->setFont(u8g2_font_6x10_tf);
    u8g2->drawStr(42, 27, "Point remote");
    u8g2->drawStr(42, 39, "at the top");

    u8g2->setFont(u8g2_font_5x7_tf);
    u8g2->drawStr(42, 51, "and press");
    u8g2->drawStr(42, 60, "any button...");
}

void ReadSignal::drawCapturedScreen(U8G2* u8g2) const {
    // Header Banner
    u8g2->setDrawColor(1);
    u8g2->drawBox(0, 0, SCREEN_WIDTH, HEADER_HEIGHT);
    u8g2->setDrawColor(0);
    u8g2->setFont(u8g2_font_6x10_tf);

    char titleBuf[32];
    snprintf(titleBuf, sizeof(titleBuf), "Captured: %s", _protoName.c_str());
    u8g2->drawStr(4, 10, titleBuf);

    // Card frame in center
    u8g2->setDrawColor(1);
    u8g2->drawRFrame(3, 15, 122, 33, 2);

    u8g2->setFont(u8g2_font_6x10_tf);

    // Protocol line with bit count
    u8g2->drawStr(8, 25, "Proto:");
    u8g2->drawStr(46, 25, _protoName.c_str());

    char bitsBuf[12];
    snprintf(bitsBuf, sizeof(bitsBuf), "%db", _storedSignal.bits);
    u8g2->drawStr(98, 25, bitsBuf);

    // Address line
    char addrBuf[24];
    if (_storedSignal.address > 0) {
        snprintf(addrBuf, sizeof(addrBuf), "Addr:  0x%04X", _storedSignal.address);
    } else {
        snprintf(addrBuf, sizeof(addrBuf), "Addr:  none");
    }
    u8g2->drawStr(8, 35, addrBuf);

    // Command/Value line
    char cmdBuf[28];
    snprintf(cmdBuf, sizeof(cmdBuf), "Cmd:   0x%llX", static_cast<unsigned long long>(_storedSignal.value));
    u8g2->drawStr(8, 45, cmdBuf);

    // Bottom action bar
    u8g2->setFont(u8g2_font_5x7_tf);
    u8g2->drawStr(4, 61, "< Back");

    const bool isTransmitting = (_lastSendTime > 0) && ((millis() - _lastSendTime) < SEND_FEEDBACK_DURATION_MS);
    if (isTransmitting) {
        u8g2->drawRBox(64, 50, 61, 13, 2);
        u8g2->setDrawColor(0);
        u8g2->setFont(u8g2_font_6x10_tf);
        u8g2->drawStr(68, 60, "Sending...");
        u8g2->setDrawColor(1);
    } else {
        u8g2->drawRBox(70, 50, 55, 13, 2);
        u8g2->setDrawColor(0);
        u8g2->setFont(u8g2_font_6x10_tf);
        u8g2->drawStr(76, 60, "> Send");
        u8g2->setDrawColor(1);
    }
}

void ReadSignal::draw(U8G2* u8g2) {
    u8g2->clearBuffer();
    if (!_hasSignal) {
        drawWaitingScreen(u8g2);
    } else {
        drawCapturedScreen(u8g2);
    }
}
