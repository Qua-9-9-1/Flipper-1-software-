#pragma once
#include <Arduino.h>

#include "AppEvents.h"
#include "DisplayDriver.hpp"
#include "Page.hpp"
#include "PageList.hpp"
#include "PageRegistry.hpp"
#include "PinDefinitions.h"
#include "settings/PageControlTest.hpp"

enum PageState { PAGE_MENU, PAGE_RADIO, PAGE_CONTROLS };

void taskUI(void* pvParameters);