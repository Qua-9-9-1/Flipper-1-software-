#pragma once
#include <Arduino.h>

enum EventType {
    EVENT_RADIO,
    EVENT_NFC,
    EVENT_BUTTON_PRESS,
    EVENT_BUTTON_LONG
};

enum ButtonID {
    BTN_UP,
    BTN_DOWN,
    BTN_LEFT,
    BTN_RIGHT,
    BTN_OK,
    BTN_BACK
};

struct AppEvent {
    EventType type; 
    int value;
};