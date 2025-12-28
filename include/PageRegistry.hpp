#pragma once
#include "AppEvents.h"
#include "PageIR.hpp"
#include "PageList.hpp"
#include "core/PageBoot.hpp"
#include "core/PageError.hpp"
#include "core/PageMainMenu.hpp"
#include "managers/StorageManager.hpp"
#include "settings/PageControlTest.hpp"
#include "settings/PageLEDTest.hpp"

namespace PageRegistry {
extern PageMainMenu mainMenuPage;
extern PageBoot     bootPage;
extern PageError    errorPage;

void setupPages();
}  // namespace PageRegistry