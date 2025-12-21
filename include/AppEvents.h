#pragma once
#include <Arduino.h>

enum EventType {
    EVENT_RADIO,
    EVENT_NFC,
    EVENT_BUTTON_CLICK,
    EVENT_BUTTON_DOWN,
    EVENT_BUTTON_UP
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