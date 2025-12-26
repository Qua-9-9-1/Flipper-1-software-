#pragma once
#include "AppEvents.h"
#include "PageList.hpp"
#include "core/PageBoot.hpp"
#include "core/PageError.hpp"
#include "core/PageMainMenu.hpp"
#include "managers/StorageManager.hpp"
#include "settings/PageControlTest.hpp"

namespace PageRegistry {
extern PageMainMenu mainMenu;
extern PageBoot     bootPage;
extern PageError    errorPage;

void setupPages();
}  // namespace PageRegistry