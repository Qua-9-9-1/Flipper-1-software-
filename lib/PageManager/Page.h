#pragma once
#include <U8g2lib.h>
#include "AppEvents.h"

class IPage {
public:
    virtual ~IPage() {}
    virtual void onEnter() = 0; 
    virtual void draw(U8G2 *u8g2) = 0;     
    virtual void onEvent(AppEvent *event) = 0;
};