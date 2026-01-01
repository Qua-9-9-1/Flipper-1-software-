#pragma once
#include "Page.hpp"
#include "System.hpp"
#include "helpers/AudioHelper.h"
#include "helpers/LedHelper.h"

class PageIR : public IPage {
   public:
    PageIR();
    void onEnter() override;
    void onEvent(AppEvent* event) override;
    void draw(U8G2* u8g2) override;

   private:
    IrSignal _storedSignal;
    bool     _hasSignal;
    String   _protoName;
};