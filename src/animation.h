#pragma once
#include <Arduino.h>
#include "lcd.h"

int lootFrame = 0; // index de l'img current
unsigned long lootLastFrameTime = 0; // millis of when last changed
const unsigned long lootFrameDuration = 150; // ms/frame

class Animation {
    public:
    Animation(const uint16_t* const* frames, int count, unsigned long frameDuration) {
        _frames = frames; // imgs in anim, defined in assets.h
        _count= count; // count of img in anim, defined in assets.H
        _frameDuration = frameDuration; // ms/frame
    }

    void reset(LCD& lcd) {
        currentFrame = 0;
        lastFrameTime = millis();
        lcd.display_img(_frames[currentFrame]);
    }

    void update(LCD& lcd) {
        if (millis() - lastFrameTime >= _frameDuration) {
        lastFrameTime = millis();
        currentFrame = (currentFrame + 1) % _count;
        lcd.display_img(_frames[currentFrame]);
        }
    }

    private:
    const uint16_t* const* _frames;
    int _count;
    unsigned long _frameDuration;
    int currentFrame = 0; // index de l'img current
    unsigned long lastFrameTime = 0; // millis of when last changed
};