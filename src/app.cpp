#include "buttons.h"
#include "led.h"
#include "lcd.h"
#include "assets/assets.h"
// ...

#include "state_machine.h"

#include <Arduino.h>

FrogState state;

LED led(19);
LCD lcd(LCD::Params{.sck = 2, .tx = 3, .cs = 1, .res = 5, .dc = 4});
Button eatBtn(21);
Button lootBtn(18);
Button execBtn(16);


void app_init() {
    // state_init()
    // buttons_init();
    // sensor_init();
    led.init();
    lcd.init();
    eatBtn.init();
    lootBtn.init();
    execBtn.init();
    // servo_init();

}



void app_update() {
    led.fire();

    eatBtn.update();
    lootBtn.update();
    execBtn.update();


    // read inputs
    // update state
    // update outputs


    switch(state.action){
        case FrogState::Action::SLEEP:
            // execute_sleep();
            // if(snesor.dist < 40) state.action = FrogState::Action::ACTIVE;
            break;
        case FrogState::Action::ACTIVE:
            break;
        case FrogState::Action::EAT:
            break;
        case FrogState::Action::LOOT:
            break;
        case FrogState::Action::EXECUTE:
            break;
    }


}

void app_run() {
    app_init();
    while (true) {
        app_update();
    }
}



// // pour animation ? 

// struct Frame{
//   uint16_t* frameData;
//   float duration;
// };

// Frame frames[10];





// // TODO remove animations for v1

// void app_update() {
//     buttons_update();
//     sensor_update();

//     bool near = sensor_is_near();
//     uint32_t now = to_ms_since_boot(get_absolute_time());

//     switch (ctx.state) {
//         case AppState::SLEEP:
//             if (near) enter_state(AppState::ACTIVE, now);
//             break;

//         case AppState::ACTIVE:
//             if (!near) {
//                 enter_state(AppState::SLEEP, now);
//             } else if (button1_pressed()) {
//                 enter_state(AppState::A_EAT, now);
//             } else if (button2_pressed()) {
//                 enter_state(AppState::A_LOOT, now);
//             } else if (button3_pressed()) {
//                 enter_state(AppState::A_EXECUTE, now);
//             }
//             break;

//         case AppState::A_EAT:
//             if (animation_finished(now)) {
//                 enter_state(near ? AppState::ACTIVE : AppState::SLEEP, now);
//             }
//             break;

//         case AppState::A_LOOT:
//             if (animation_finished(now)) {
//                 enter_state(near ? AppState::ACTIVE : AppState::SLEEP, now);
//             }
//             break;

//         case AppState::A_EXECUTE:
//             if (animation_finished(now)) {
//                 enter_state(near ? AppState::ACTIVE : AppState::SLEEP, now);
//             }
//             break;
//     }
