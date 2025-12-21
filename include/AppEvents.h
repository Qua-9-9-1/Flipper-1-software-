#pragma once
#include <Arduino.h>

enum EventType {
    EVENT_RADIO,
    EVENT_NFC,
    EVENT_BUTTON
};

struct AppEvent {
    EventType type; 
    int value;
};