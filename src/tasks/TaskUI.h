#pragma once
#include <Arduino.h>

enum PageState {
    PAGE_MENU,
    PAGE_RADIO,
    PAGE_CONTROLS
};

void taskUI(void *pvParameters);