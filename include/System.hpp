#pragma once
#include <Arduino.h>

#include "AppEvents.h"
#include "tasks/TaskAudio.h"
#include "tasks/TaskIR.h"
#include "tasks/TaskInput.h"
#include "tasks/TaskLED.h"
#include "tasks/TaskRadio.h"
#include "tasks/TaskUI.h"

struct IrCommand {
    uint32_t address;
    uint32_t command;
    // TODO: add protocol, nbits, etc.
};

extern QueueHandle_t eventQueue;
extern QueueHandle_t ledQueue;
extern QueueHandle_t audioQueue;
extern QueueHandle_t irQueue;

namespace System {
void init();
void startTasks();
}  // namespace System