#include "buttons.h"
#include "led.h"
#include "lcd.h"
#include "sensor.h"
#include "servo.h"
#include "assets/assets.h"
#include "state_machine.h"

#include <Arduino.h>

FrogState state;
SensorData sensorData;

unsigned long stateDuration = 2000;
int servo_ch = 4;

LED leds[] = {
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

AppContext ctx {
  lcd,
  sensor,
  sensorData,
  servo,
  eatBtn,
  lootBtn,
  executeBtn,
  leds,
  LED_COUNT,
  servo_ch,
  stateDuration
};

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

  update_state_machine(state, ctx);
}

void app_run() {
  app_init();

  while (true) {
    app_update();
  }
}