#pragma once
#include <Arduino.h>

enum EventType {
    EVENT_RADIO,
    EVENT_NFC,
    EVENT_BUTTON_CLICK,
    EVENT_BUTTON_DOWN,
    EVENT_BUTTON_UP,
    EVENT_LED_SET_MODE,
    EVENT_LED_SET_COLOR,
    EVENT_PLAY_SOUND,
    EVENT_SHUTDOWN_POPUP,
    EVENT_IR_RX,
    EVENT_IR_TX
};

enum ButtonID { BTN_UP, BTN_DOWN, BTN_LEFT, BTN_RIGHT, BTN_OK, BTN_BACK };

enum LedMode {
    LED_MODE_OFF,
    LED_MODE_BOOT,
    LED_MODE_BATTERY,
    LED_MODE_SCAN,
    LED_MODE_ACTION,
    LED_MODE_MANUAL
};

enum SoundType {
    SOUND_MUTE,
    SOUND_TICK,
    SOUND_OK,
    SOUND_BACK,
    SOUND_ERROR,
    SOUND_SUCCESS,
    SOUND_MELODY_BOOT
};

struct AppEvent {
    EventType type;
    int       value;
    int       extra;
};

struct LedCommand {
    LedMode mode;
    int     value;
};