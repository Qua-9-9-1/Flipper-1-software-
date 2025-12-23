#pragma once
#include <Arduino.h>
#include "DisplayDriver.h"
#include "AppEvents.h"
#include "PinDefinitions.h"
#include "settings/PageControlTest.h"
#include "PageList.h"
#include "Page.h"
#include "PageRegistry.h"

enum PageState {
    PAGE_MENU,
    PAGE_RADIO,
    PAGE_CONTROLS
};

void taskUI(void *pvParameters);