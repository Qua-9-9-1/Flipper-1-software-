#include "TaskInput.h"
#include <OneButton.h>
#include "PinDefinitions.h"
#include "AppEvents.h"

extern QueueHandle_t eventQueue;

OneButton btnUp(PIN_BTN_UP, true);
OneButton btnDown(PIN_BTN_DOWN, true);
OneButton btnLeft(PIN_BTN_LEFT, true);
OneButton btnRight(PIN_BTN_RIGHT, true);
OneButton btnOk(PIN_BTN_OK, true);
OneButton btnBack(PIN_BTN_BACK, true);

void sendButtonEvent(ButtonID btnId, EventType type) {
    AppEvent e;
    e.type = type;
    e.value = btnId;
    xQueueSend(eventQueue, &e, 0); // 0 = ne pas attendre si la queue est pleine
}

// Callbacks (fonctions appelées quand on clique)
// Note: On doit faire des fonctions statiques ou globales pour OneButton
void clickUp() { sendButtonEvent(BTN_UP, EVENT_BUTTON_PRESS); }
void clickDown() { sendButtonEvent(BTN_DOWN, EVENT_BUTTON_PRESS); }
void clickLeft() { sendButtonEvent(BTN_LEFT, EVENT_BUTTON_PRESS); }
void clickRight() { sendButtonEvent(BTN_RIGHT, EVENT_BUTTON_PRESS); }
void clickOk() { sendButtonEvent(BTN_OK, EVENT_BUTTON_PRESS); }
void clickBack() { sendButtonEvent(BTN_BACK, EVENT_BUTTON_PRESS); }

void longClickUp() { sendButtonEvent(BTN_UP, EVENT_BUTTON_LONG); }
void longClickDown() { sendButtonEvent(BTN_DOWN, EVENT_BUTTON_LONG); }
void longClickLeft() { sendButtonEvent(BTN_LEFT, EVENT_BUTTON_LONG); }
void longClickRight() { sendButtonEvent(BTN_RIGHT, EVENT_BUTTON_LONG); }
void longClickOk() { sendButtonEvent(BTN_OK, EVENT_BUTTON_LONG); }
void longClickBack() { sendButtonEvent(BTN_BACK, EVENT_BUTTON_LONG); }


void taskInput(void *pvParameters) {
    // Attachement des fonctions aux boutons
    btnUp.attachClick(clickUp);
    btnDown.attachClick(clickDown);
    btnLeft.attachClick(clickLeft);
    btnRight.attachClick(clickRight);
    btnOk.attachClick(clickOk);
    btnBack.attachClick(clickBack);

    btnUp.attachLongPressStart(longClickUp);
    btnDown.attachLongPressStart(longClickDown);
    btnLeft.attachLongPressStart(longClickLeft);
    btnRight.attachLongPressStart(longClickRight);
    btnOk.attachLongPressStart(longClickOk);
    btnBack.attachLongPressStart(longClickBack);
    

    while (true) {
        // Il faut appeler tick() très souvent pour détecter les clics
        btnUp.tick();
        btnDown.tick();
        btnLeft.tick();
        btnRight.tick();
        btnOk.tick();
        btnBack.tick();
        
        // On dort 10ms pour laisser respirer le CPU (polling rate 100Hz)
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}