#include "state_machine.h"
#include "assets/assets.h"
#include "animation.h"

static Animation lootAnimation(loot_anim, loot_anim_count, 150); // defined in assets.h

static void enter_sleep(FrogState& state, AppContext& ctx) {
  ctx.lcd.display_img(sleep_img);
  state.mark_entered();
}

static void update_sleep(FrogState& state, AppContext& ctx) {
  if (ctx.sensor.is_near(ctx.sensorData, 0.10f)) {
    state.change(FrogState::Action::ACTIVE);
  }
}

static void enter_active(FrogState& state, AppContext& ctx) {
  ctx.lcd.display_img(active_img);
  state.mark_entered();
}

static void update_active(FrogState& state, AppContext& ctx) {
  if (ctx.sensor.is_far(ctx.sensorData, 1.0f)) {
    state.change(FrogState::Action::SLEEP);
    return;
  }

  if (ctx.eatBtn.pressed()) {
    state.change(FrogState::Action::EAT);
  } else if (ctx.lootBtn.pressed()) {
    state.change(FrogState::Action::LOOT);
  } else if (ctx.executeBtn.pressed()) {
    state.change(FrogState::Action::EXECUTE);
  }
}

static void enter_eat(FrogState& state, AppContext& ctx) {
  ctx.lcd.display_img(eat_img);
  state.mark_entered();
}

static void update_eat(FrogState& state, AppContext& ctx) {
  if (state.elapsed(ctx.stateDuration)) {
    state.change(FrogState::Action::ACTIVE);
  }
}

static void enter_loot(FrogState& state, AppContext& ctx) {
  lootAnimation.reset(ctx.lcd);
  state.mark_entered();
}

static void update_loot(FrogState& state, AppContext& ctx) {
  for (int i = 0; i < ctx.ledCount; i++) {
    ctx.leds[i].fire();
  }

  lootAnimation.update(ctx.lcd);

  if (state.elapsed(ctx.stateDuration)) {
    for (int i = 0; i < ctx.ledCount; i++) {
      ctx.leds[i].off();
    }

    state.change(FrogState::Action::ACTIVE);
  }
}

static void enter_execute(FrogState& state, AppContext& ctx) {
  ctx.lcd.display_img(execute_img);
  ctx.servo.add_angle_loop(ctx.servoChannel, 90);

  state.mark_entered();
}

static void update_execute(FrogState& state, AppContext& ctx) {
  if (state.elapsed(ctx.stateDuration)) {
    state.change(FrogState::Action::ACTIVE);
  }
}

void update_state_machine(FrogState& state, AppContext& ctx) {
  switch (state.action) {
    case FrogState::Action::SLEEP:
      if (!state.stateEntered) {
        enter_sleep(state, ctx);
      }
      update_sleep(state, ctx);
      break;

    case FrogState::Action::ACTIVE:
      if (!state.stateEntered) {
        enter_active(state, ctx);
      }
      update_active(state, ctx);
      break;

    case FrogState::Action::EAT:
      if (!state.stateEntered) {
        enter_eat(state, ctx);
      }
      update_eat(state, ctx);
      break;

    case FrogState::Action::LOOT:
      if (!state.stateEntered) {
        enter_loot(state, ctx);
      }
      update_loot(state, ctx);
      break;

    case FrogState::Action::EXECUTE:
      if (!state.stateEntered) {
        enter_execute(state, ctx);
      }
      update_execute(state, ctx);
      break;
  }
}