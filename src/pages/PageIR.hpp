#pragma once
#include "AudioHelper.h"
#include "LedHelper.h"
#include "Page.hpp"
#include "System.hpp"

class PageIR : public IPage {
   public:
    PageIR();
    void onEnter() override;
    void onEvent(AppEvent* event) override;
    void draw(U8G2* u8g2) override;

   private:
    String lastCodeType;
    String lastCodeHex;
};