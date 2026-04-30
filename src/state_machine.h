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

    int lifepoints;
    float hunger;
};