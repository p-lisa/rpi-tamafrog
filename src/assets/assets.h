#pragma once // to incl only once per compil unit
#include <stdint.h> // incl standard header that defines fixed width int type

extern const uint16_t sleep_img[];
extern const uint16_t active_img[];

extern const uint16_t eat_img_0[];
extern const uint16_t eat_img_1[];
extern const uint16_t eat_img_2[];
extern const uint16_t eat_img_3[];
extern const uint16_t eat_img_4[];
extern const uint16_t* const eat_anim[];
extern const int eat_anim_count;

extern const uint16_t loot_img_0[];
extern const uint16_t loot_img_1[];
extern const uint16_t loot_img_2[];
extern const uint16_t* const loot_anim[];
extern const int loot_anim_count;

extern const uint16_t execute_img_0[];
extern const uint16_t execute_img_1[];
extern const uint16_t execute_img_2[];
extern const uint16_t* const execute_anim[];
extern const int execute_anim_count;

