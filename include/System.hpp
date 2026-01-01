#pragma once
#include <Arduino.h>

#include "AppEvents.h"
#include "tasks/TaskIR.h"

struct IrCommand {
    uint32_t address;
    uint32_t command;
};

struct IrSignal {
    decode_type_t protocol;
    uint64_t      value;
    uint16_t      bits;
    uint16_t      address;
};

#include "helpers/BatteryHelper.h"
#include "tasks/TaskAudio.h"
#include "tasks/TaskHaptic.h"
#include "tasks/TaskInput.h"
#include "tasks/TaskLED.h"
#include "tasks/TaskRadio.h"
#include "tasks/TaskUI.h"

extern QueueHandle_t eventQueue;
extern QueueHandle_t ledQueue;
extern QueueHandle_t audioQueue;
extern QueueHandle_t irQueue;

namespace System {
void            init();
void            startTasks();
extern IrSignal lastCapturedSignal;
}  // namespace System