#pragma once
#include <Arduino.h>

#include "AppEvents.h"
#include "tasks/TaskAudio.h"
#include "tasks/TaskInput.h"
#include "tasks/TaskLED.h"
#include "tasks/TaskRadio.h"
#include "tasks/TaskUI.h"

extern QueueHandle_t eventQueue;
extern QueueHandle_t ledQueue;
extern QueueHandle_t audioQueue;

namespace System {
void init();
void startTasks();
}  // namespace System