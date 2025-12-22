#include "TaskInput.h"

extern QueueHandle_t eventQueue;

OneButton btnUp(PIN_BTN_UP, true);
OneButton btnDown(PIN_BTN_DOWN, true);
OneButton btnLeft(PIN_BTN_LEFT, true);
OneButton btnRight(PIN_BTN_RIGHT, true);
OneButton btnOk(PIN_BTN_OK, true);
OneButton btnBack(PIN_BTN_BACK, true);

void sendStateEvent(ButtonID btnId, bool isDown) {
    AppEvent e;
    e.type = isDown ? EVENT_BUTTON_DOWN : EVENT_BUTTON_UP;
    e.value = btnId;
    xQueueSend(eventQueue, &e, 0);
}

void clickUp() {
    AppEvent e = {EVENT_BUTTON_CLICK, BTN_UP};
    xQueueSend(eventQueue, &e, 0);
}

void clickDown() {
    AppEvent e = {EVENT_BUTTON_CLICK, BTN_DOWN};
    xQueueSend(eventQueue, &e, 0);
}

void clickLeft() {
    AppEvent e = {EVENT_BUTTON_CLICK, BTN_LEFT};
    xQueueSend(eventQueue, &e, 0);
}

void clickRight() {
    AppEvent e = {EVENT_BUTTON_CLICK, BTN_RIGHT};
    xQueueSend(eventQueue, &e, 0);
}

void clickOk() {
    AppEvent e = {EVENT_BUTTON_CLICK, BTN_OK};
    xQueueSend(eventQueue, &e, 0);
}

void clickBack() {
    AppEvent e = {EVENT_BUTTON_CLICK, BTN_BACK};
    xQueueSend(eventQueue, &e, 0);
}

void taskInput(void *pvParameters) {
    btnUp.attachClick(clickUp);
    btnDown.attachClick(clickDown);
    btnLeft.attachClick(clickLeft);
    btnRight.attachClick(clickRight);
    btnOk.attachClick(clickOk);
    btnBack.attachClick(clickBack);


    bool lastState[6] = {false};
    int pins[6] = {PIN_BTN_UP, PIN_BTN_DOWN, PIN_BTN_LEFT, PIN_BTN_RIGHT, PIN_BTN_OK, PIN_BTN_BACK};

    while (true) {
        btnUp.tick();
        btnDown.tick();
        btnLeft.tick();
        btnRight.tick();
        btnOk.tick();
        btnBack.tick();

        for(int i = 0; i < 6; i++) {
            bool currentState = !digitalRead(pins[i]); // Active LOW
            if (currentState != lastState[i]) {
                lastState[i] = currentState;
                
                AppEvent e;
                e.type = currentState ? EVENT_BUTTON_DOWN : EVENT_BUTTON_UP;
                e.value = i;
                xQueueSend(eventQueue, &e, 0);
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}