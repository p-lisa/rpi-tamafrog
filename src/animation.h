#pragma once
#include <Arduino.h>
#include "lcd.h"

class Animation {
public:
  Animation(const uint16_t* const* frames, int count, unsigned long frameDuration)
    : _frames(frames), _count(count), _frameDuration(frameDuration) {}

  void reset(LCD& lcd) { // when the animation is entered
    _currentFrame = 0;
    _lastFrameTime = millis();
    lcd.display_img(_frames[_currentFrame]);
  }

  void update(LCD& lcd) { // to display the actual animation
    if (millis() - _lastFrameTime >= _frameDuration) {
      _lastFrameTime = millis();
      _currentFrame = (_currentFrame + 1) % _count;
      lcd.display_img(_frames[_currentFrame]);
    }
  }

private:
  const uint16_t* const* _frames; // imgs in anim, defined in assets.h
  int _count; // count of img in anim, defined in assets.h
  unsigned long _frameDuration; // ms/frame

  int _currentFrame = 0; // index de l'img current
  unsigned long _lastFrameTime = 0; // millis of when last changed
};