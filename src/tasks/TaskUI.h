#pragma once
#include <Arduino.h>
#include "DisplayDriver.h"
#include "AppEvents.h"
#include "PinDefinitions.h"
#include "PageControlTest.h"
#include "PageMainMenu.h"
#include "PageList.h"
#include "PageBoot.h"
#include "Page.h"

enum PageState {
    PAGE_MENU,
    PAGE_RADIO,
    PAGE_CONTROLS
};

void taskUI(void *pvParameters);