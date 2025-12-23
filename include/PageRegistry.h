#pragma once
#include "core/PageMainMenu.h"
#include "core/PageBoot.h"
#include "PageList.h"
#include "settings/PageControlTest.h"
#include "AppEvents.h"

extern PageMainMenu mainMenu;
extern PageBoot bootPage;

namespace PageRegistry {    
    void setupPages();
}