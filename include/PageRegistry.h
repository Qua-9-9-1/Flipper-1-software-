#pragma once
#include "PageMainMenu.h"
#include "utils/PageList.h"
#include "PageBoot.h"
#include "settings/PageControlTest.h"
#include "AppEvents.h"

extern PageMainMenu mainMenu;
extern PageBoot bootPage;

namespace PageRegistry {    
    void setupPages();
}