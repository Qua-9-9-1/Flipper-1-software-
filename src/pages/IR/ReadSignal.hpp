#pragma once

#include <Arduino.h>

#include "Page.hpp"
#include "System.hpp"
#include "helpers/AudioHelper.h"
#include "helpers/LedHelper.h"

class ReadSignal : public IPage {
   public:
    ReadSignal();

    void onEnter() override;
    void onEvent(AppEvent* event) override;
    void draw(U8G2* u8g2) override;

   private:
    void updateProtocolName();
    void drawWaitingScreen(U8G2* u8g2) const;
    void drawCapturedScreen(U8G2* u8g2) const;
    void drawRemoteIllustration(U8G2* u8g2, int startX, int startY) const;

    IrSignal _storedSignal;
    bool     _hasSignal;
    String   _protoName;
    uint32_t _lastSendTime;
};
