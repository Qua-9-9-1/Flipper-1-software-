#pragma once
#include <Arduino.h>
#include <functional>

#include "Page.hpp"
#include "PageManager.hpp"

using PageAction = std::function<void()>;

namespace PageActions {

inline PageAction openPage(IPage* targetPage) {
    return [targetPage]() {
        if (targetPage) {
            PageManager::getInstance()->pushPage(targetPage);
        }
    };
}

inline PageAction runBadUsb(const String& scriptPath) {
    return [scriptPath]() {
        Serial.printf("[BadUSB] Executing script: %s\n", scriptPath.c_str());
        // BadUsbManager::getInstance()->runScript(scriptPath);
    };
}

};
