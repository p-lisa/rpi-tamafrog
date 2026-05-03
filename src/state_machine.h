#pragma once

#include <Arduino.h>

#include "lcd.h"
#include "led.h"
#include "sensor.h"
#include "servo.h"
#include "buttons.h"

struct FrogState{

    enum class Action{
        SLEEP,
        ACTIVE,
        EAT,
        LOOT,
        EXECUTE
    };
    Action action = Action::SLEEP;
    uint32_t stateStart = 0;
    bool stateEntered = false;

    int lifepoints; // placeholder
    float hunger; // placeholder

    void change(Action next) {
        action = next; // replace current state with new state in switch
        stateEntered = false;
    }

    void mark_entered() {
        stateStart = millis();
        stateEntered = true;
    }

    bool elapsed(unsigned long duration) const {
        return millis() - stateStart >= duration;
    }
};

struct AppContext {
    LCD& lcd;
    Sensor& sensor;
    SensorData& sensorData;
    PCAServo& servo;

    Button& eatBtn;
    Button& lootBtn;
    Button& executeBtn;

    LED* leds;
    int ledCount;

    int servoChannel;
    unsigned long stateDuration;
};

void update_state_machine(FrogState& state, AppContext& ctx);