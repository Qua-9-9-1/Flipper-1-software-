#pragma once
#include <Arduino.h>
#include "AppEvents.h"
#include "PinDefinitions.h"
#include "settings/PageControlTest.hpp"
#include "DisplayDriver.hpp"
#include "PageList.hpp"
#include "Page.hpp"
#include "PageRegistry.hpp"

enum PageState {
    PAGE_MENU,
    PAGE_RADIO,
    PAGE_CONTROLS
};

void taskUI(void *pvParameters);