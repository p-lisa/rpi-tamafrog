#include "app.h"
#include <Arduino.h>


void setup() {
  Serial.begin(9600);  
  app_run();
}

void loop() {
}


// stdio_init_all() (needed?? within wrapper)

// buttons init
// sensor init
// lcd init
// led init
// servo init

// app init


// >> or put it in app_init and just call that here ? with:
// #include "pico/stdlib.h"
// #include "app.h


// loop:

// sleep if sensor_is_near, then show tamafrog_sleep
// active if sensor_is_near, then show tamafrog_active
// a_eat if active and b1 low, then show tamafrog_eat[i] where i rand and back to state active
// a_loot if active and b2 low, then turn on led, show tamafrog_loot and back to state active
// a_execute if active and b3 low, then turn on motor, show tamafrog_execute and back to state active
