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
    };
}

inline PageAction showSubGhzDetails(const String& filename) {
    return [filename]() {
        Serial.printf("[Sub-GHz] Loading file details: %s\n", filename.c_str());
    };
}

inline PageAction showIrDetails(const String& filename) {
    return [filename]() {
        Serial.printf("[IR] Loading file details: %s\n", filename.c_str());
    };
}

inline PageAction showNfcDetails(const String& filename) {
    return [filename]() {
        Serial.printf("[NFC] Loading file details: %s\n", filename.c_str());
    };
}

}  // namespace PageActions
