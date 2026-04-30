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

    int lifepoints;
    float hunger;
};