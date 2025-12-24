#pragma once
#include "AppEvents.h"
#include "core/PageMainMenu.hpp"
#include "core/PageBoot.hpp"
#include "PageList.hpp"
#include "settings/PageControlTest.hpp"

extern PageMainMenu mainMenu;
extern PageBoot bootPage;

namespace PageRegistry {    
    void setupPages();
}