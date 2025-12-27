#include "AppEvents.h"
#include "BuzzerDriver.hpp"
#include "PinDefinitions.h"

extern QueueHandle_t audioQueue;

void taskAudio(void* pvParameters);
