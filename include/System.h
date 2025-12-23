#pragma once
#include <Arduino.h>
#include "AppEvents.h"
#include "tasks/TaskUI.h"
#include "tasks/TaskInput.h"
#include "tasks/TaskRadio.h"
#include "tasks/TaskLED.h"

extern QueueHandle_t eventQueue;
extern QueueHandle_t ledQueue;

namespace System {
    void init();
    void startTasks();
}