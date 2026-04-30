# pragma once

#include <Arduino.h>

// INPUT PULLUP: pressed is LOW, unpressed is HIGH

class Button{
    public:
    Button(int p){
        _pin = p;
        _lastState = false;
        _pressed = false;
    }

    void init(){
        pinMode(_pin, INPUT_PULLUP);
    }

    void update() {
        bool current = !digitalRead(_pin); // inverted for intuition. When pressed -> true

        // detect when the button is pressed
        _pressed = (_lastState == false && current == true);

        _lastState = current;
    }

    bool pressed() {
        return _pressed;
    }
    
    private:
    int _pin;
    bool _lastState;
    bool _pressed;
};