#pragma once

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