#include "buttons.h"
#include "led.h"
#include "lcd.h"
#include "sensor.h"
#include "servo.h"
#include "assets/assets.h"

#include "state_machine.h"

#include <Arduino.h>

int lootFrame = 0; // index de l'img current
unsigned long lootLastFrameTime = 0; // millis of when last changed
const unsigned long lootFrameDuration = 150; // ms/frame

int stateDuration = 2000;
int servo_ch = 4;

FrogState state;
SensorData sensorData;

LED leds[] = { // choose diff phaseoffsets for most
    LED(20, 0.0f),
    LED(13, 41.7f),
    LED(14, 93.2f),
    LED(15, 158.9f),
};

const int LED_COUNT = sizeof(leds) / sizeof(leds[0]);

LCD lcd(LCD::Params{.sck = 2, .tx = 3, .cs = 1, .res = 5, .dc = 4});
Sensor sensor;
PCAServo servo(6, 7);
Button eatBtn(21);
Button lootBtn(18);
Button executeBtn(16);

void execute_sleep() {
    lcd.display_img(sleep_img);
}

void execute_active() {
    lcd.display_img(active_img);
}

void execute_eat() {
    lcd.display_img(eat_img);
}

void execute_loot() {
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i].fire();
  }

  if (millis() - lootLastFrameTime >= lootFrameDuration) {
    lootLastFrameTime = millis();
    lootFrame = (lootFrame + 1) % loot_anim_count;
    lcd.display_img(loot_anim[lootFrame]);
  }
}

void execute_execute() {
    lcd.display_img(execute_img);
    servo.add_angle_loop(servo_ch, 90);
}

void app_init() {
  Serial.begin(9600);

  for (int i = 0; i < LED_COUNT; i++) {
    leds[i].init();
  }

  lcd.init();
  sensor.init();
  servo.init();
  servo.set_angle(servo_ch, 0);

  eatBtn.init();
  lootBtn.init();
  executeBtn.init();
}

void app_update() {
  eatBtn.update();
  lootBtn.update();
  executeBtn.update();

  if (sensor.read(sensorData) && sensorData.validity) {
    Serial.printf(
      "dist: %.3f m, str: %u, status: %u\n",
      sensorData.distance_m,
      sensorData.strength,
      sensorData.status
    );
  }

  switch (state.action) {

    case FrogState::Action::SLEEP:
      execute_sleep();

      if (sensor.is_near(sensorData, 0.10f)) {
        state.action = FrogState::Action::ACTIVE;
        state.stateEntered = false;
      }
      break;

    case FrogState::Action::ACTIVE:
      execute_active();

      if (sensor.is_far(sensorData, 1.0f)) {
        state.action = FrogState::Action::SLEEP;
        state.stateEntered = false;
      }

      if (eatBtn.pressed()) {
        state.action = FrogState::Action::EAT;
        state.stateEntered = false;
      } else if (lootBtn.pressed()) {
        state.action = FrogState::Action::LOOT;
        state.stateEntered = false;
      } else if (executeBtn.pressed()) {
        state.action = FrogState::Action::EXECUTE;
        state.stateEntered = false;
      }
      break;

    case FrogState::Action::EAT:
      if (!state.stateEntered) {
        execute_eat();
        state.stateStart = millis();
        state.stateEntered = true;
      }

      if (millis() - state.stateStart >= stateDuration) {
        state.action = FrogState::Action::ACTIVE;
        state.stateEntered = false;
      }
      break;

    case FrogState::Action::LOOT:
      if (!state.stateEntered) {
        lootFrame = 0;
        lootLastFrameTime = millis();
        lcd.display_img(loot_anim[lootFrame]);

        state.stateStart = millis();
        state.stateEntered = true;
      }

      execute_loot();

      if (millis() - state.stateStart >= stateDuration) {
        for (int i = 0; i < LED_COUNT; i++) {
          leds[i].off();
        }
        state.action = FrogState::Action::ACTIVE;
        state.stateEntered = false;
      }
      break;

    case FrogState::Action::EXECUTE:
      if (!state.stateEntered) {
        execute_execute();
        state.stateStart = millis();
        state.stateEntered = true;
      }

      if (millis() - state.stateStart >= stateDuration) {
        state.action = FrogState::Action::ACTIVE;
        state.stateEntered = false;
      }
      break;
  }
}

void app_run() {
    app_init();
    while (true) {
        app_update();
    }
}
