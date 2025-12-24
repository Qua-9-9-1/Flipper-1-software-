#pragma once
#include "AppEvents.h"
#include "PageList.hpp"
#include "core/PageBoot.hpp"
#include "core/PageMainMenu.hpp"
#include "settings/PageControlTest.hpp"

extern PageMainMenu mainMenu;
extern PageBoot     bootPage;

namespace PageRegistry {
void setupPages();
}