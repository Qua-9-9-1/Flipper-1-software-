#pragma once
#include <U8g2lib.h>

#include "AppEvents.h"
#include "Assets.h"

class ShutdownPopup {
   public:
    ShutdownPopup();
    void show();
    void hide();
    bool active();
    bool onEvent(AppEvent* event);
    void draw(U8G2* u8g2);

   private:
    bool _isVisible;
    int  _cursor;
};