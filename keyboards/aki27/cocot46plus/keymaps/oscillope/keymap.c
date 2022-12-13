/*
Copyright 2022 aki27

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include <stdio.h>
#include "quantum.h"


// Defines names for use in layer keycodes and the keymap
enum layer_number {
    _BASE = 0,
    _LOWER = 1,
    _RAISE = 2,
    _TRACKBALL = 3,
    _NAV = 4,
    _Layer5 = 5
};

enum custom_keycodes {
    CC_ARRW = COCOT_SAFE_RANGE,
    CC_PRN,
    CC_BRC,
    CC_CBR
};

#define MO_RS   MO(_RAISE)
#define MO_LO   MO(_LOWER)
#define MO_TB   MO(_TRACKBALL)
#define GRV_ALT ALT_T(KC_GRV)
#define BS_RS   LT(_RAISE, KC_BSPC)
#define BS_SFT  MT(MOD_LSFT, KC_BSPC)
#define SPC_NAV LT(_NAV, KC_SPC)
#define SPC_GUI MT(MOD_RGUI, KC_SPC)

/*
#define CPI_SW USER00
#define SCRL_SW USER01
#define ROT_R15 USER02
#define ROT_L15 USER03
#define SCRL_MO USER04
#define SCRL_TO USER05
#define SCRL_IN USER06
*/

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_ESC,    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,                                          KC_Y,    KC_U,    KC_I,    KC_O,   KC_P,   KC_DEL,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_TAB,    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,                                          KC_H,    KC_J,    KC_K,    KC_L, KC_SCLN,  KC_ENT,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       GRV_ALT,    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,                                          KC_N,    KC_M, KC_COMM,  KC_DOT, KC_SLSH, KC_QUOT,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                         KC_LGUI, KC_LCTL,  BS_SFT,   BS_RS,   KC_MS_BTN1,             KC_MS_BTN2, SPC_NAV, SPC_GUI,   MO_LO,   MO_TB,
                                                                 KC_WH_U, KC_MS_BTN3,    KC_WH_D, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_LOWER] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
        KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,   KC_F6,                                         KC_F7,   KC_F8,   KC_F9,  KC_F10,  KC_F11,  KC_F12,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_INS, XXXXXXX, KC_PSCR,  CC_PRN,  CC_BRC,  CC_CBR,                                       _______, _______, _______, _______, _______, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, _______, _______, _______, _______, KC_MSTP,                                       KC_MPLY, _______, _______, _______, _______, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______,   TG(3),   TG(2),      KC_MPRV,                KC_MNXT, _______, _______, _______, _______,
                                                                 KC_VOLD,    KC_MUTE,    KC_VOLU, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_RAISE] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      KC_CAPS, KC_AMPR, KC_ASTR,  KC_UNDS, KC_LPRN, KC_RPRN,                                          KC_7,    KC_8,    KC_9,  KC_EQL, KC_BSPC,  KC_DEL,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
       KC_TAB,  KC_DLR, KC_PERC,  KC_CIRC, KC_LBRC, KC_RBRC,                                          KC_4,    KC_5,    KC_6, KC_MINS, KC_PLUS, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      CC_ARRW, KC_EXLM,   KC_AT,  KC_HASH, KC_LCBR, KC_RCBR,                                          KC_1,    KC_2,    KC_3, _______, KC_BSLS, KC_PIPE,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______,  _______,   TG(3), _______,   KC_MS_BTN4,             KC_MS_BTN5,    KC_0, _______, _______, _______,
                                                                  KC_WH_L, KC_MS_BTN3,    KC_WH_R, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_TRACKBALL] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
    TO(_BASE), XXXXXXX, XXXXXXX, XXXXXXX, VK_TOGG, RGB_TOG,                                       SCRL_TO,  CPI_SW, SCRL_SW, ROT_L15, ROT_R15, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, RGB_VAI, RGB_SAI, RGB_HUI, RGB_MOD,                                       SCRL_MO, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, RGB_VAD, RGB_SAD, RGB_HUD,RGB_RMOD,                                       SCRL_IN, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______, _______,   KC_MS_BTN1,             KC_MS_BTN2, _______, _______, _______, _______,
                                                                 KC_PGUP, KC_MS_BTN3,    KC_PGDN, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    ),
  [_NAV] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, _______, _______, _______, _______, _______,                                       KC_HOME, KC_PGDN, KC_PGUP,  KC_END, _______, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, _______, _______, _______, _______, _______,                                       KC_LEFT, KC_DOWN,   KC_UP, KC_RGHT, _______, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      _______, _______, _______, _______, _______, _______,                                       _______, _______, _______, _______, _______, _______,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        _______, _______, _______,  _______,   _______,             _______, _______, _______, _______,  _______,
                                                                 KC_PGUP, _______,  KC_PGDN, _______, _______, _______
                                                            //`--------------'  `--------------'
    ),
  [_Layer5] = LAYOUT(
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
      XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,                                       XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
  //|-------------------------------------------------------|                                   |-------------------------------------------------------|
                        XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,   XXXXXXX,             XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX,  XXXXXXX,
                                                                 XXXXXXX, XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
                                                            //`--------------'  `--------------'
    )
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  if (record->event.pressed) {
    switch(keycode) {
      case CC_ARRW:
        SEND_STRING("->");
        return false;
      case CC_PRN:
        SEND_STRING("()"SS_TAP(X_LEFT));
        return false;
      case CC_BRC:
        SEND_STRING("[]"SS_TAP(X_LEFT));
        return false;
      case CC_CBR:
        SEND_STRING("{}"SS_TAP(X_LEFT));
        return false;
    }
  }
  return true;
}

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case SPC_NAV:
        case SPC_GUI:
            // Space bar should err towards tap behavior
            return false;
        default:
            // We want this behavior for everything except the spacebar
            return true;
    }
}

keyevent_t encoder1_ccw = {
    .key = (keypos_t){.row = 4, .col = 2},
    .pressed = false
};

keyevent_t encoder1_cw = {
    .key = (keypos_t){.row = 4, .col = 5},
    .pressed = false
};

bool encoder_update_user(uint8_t index, bool clockwise) {
    if (index == 0) { /* First encoder */
        if (clockwise) {
            encoder1_cw.pressed = true;
            encoder1_cw.time = (timer_read() | 1);
            action_exec(encoder1_cw);
        } else {
            encoder1_ccw.pressed = true;
            encoder1_ccw.time = (timer_read() | 1);
            action_exec(encoder1_ccw);
        }
    }

    return true;
}

void matrix_scan_user(void) {

    if (IS_PRESSED(encoder1_ccw)) {
        encoder1_ccw.pressed = false;
        encoder1_ccw.time = (timer_read() | 1);
        action_exec(encoder1_ccw);
    }

    if (IS_PRESSED(encoder1_cw)) {
        encoder1_cw.pressed = false;
        encoder1_cw.time = (timer_read() | 1);
        action_exec(encoder1_cw);
    }

}

const rgblight_segment_t PROGMEM capslock_led_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 1, HSV_RED}
);
const rgblight_segment_t PROGMEM lower_led_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 2, HSV_GOLD}
);
const rgblight_segment_t PROGMEM raise_led_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 2, HSV_PURPLE}
);
const rgblight_segment_t PROGMEM mouse_led_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 2, HSV_TEAL}
);
const rgblight_segment_t PROGMEM nav_led_layer[] = RGBLIGHT_LAYER_SEGMENTS(
    {0, 2, HSV_CYAN}
);

const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
    lower_led_layer,
    raise_led_layer,
    mouse_led_layer,
    nav_led_layer,
    capslock_led_layer
);

void keyboard_post_init_user(void) {
    rgblight_layers = my_rgb_layers;
}

bool led_update_user(led_t led_state) {
    rgblight_set_layer_state(4, led_state.caps_lock);
    return true;
}

#define LAYER_COND(n, l)      rgblight_set_layer_state(n, layer_state_cmp(state, l))
layer_state_t layer_state_set_user(layer_state_t state) {
    LAYER_COND(0, _LOWER);
    LAYER_COND(1, _RAISE);
    LAYER_COND(2, _TRACKBALL);
    LAYER_COND(3, _NAV);
    switch (get_highest_layer(state)) {
    case _LOWER:
    case _RAISE:
        break;
    case _TRACKBALL:
    case _NAV:
        cocot_set_scroll_mode(true);
        break;
    default:
        cocot_set_scroll_mode(false);
        break;
    }
    //rgblight_set_effect_range( 2, 10);
    return state;
};

#ifdef OLED_ENABLE

#define ANIM_STR_MULTI(a)       case RGBLIGHT_MODE_##a ... RGBLIGHT_MODE_##a##_end: *mode = *mode - RGBLIGHT_MODE_##a; return PSTR(#a);
#define ANIM_STR(a)             case RGBLIGHT_MODE_##a: *mode = *mode - RGBLIGHT_MODE_##a; return PSTR(#a);
const char* PROGMEM get_anim_str(uint8_t* mode)
{
    switch (*mode) {
#ifdef RGBLIGHT_EFFECT_TWINKLE
    ANIM_STR_MULTI(TWINKLE)
#endif
#ifdef RGBLIGHT_EFFECT_ALTERNATING
    ANIM_STR(ALTERNATING)
#endif
#ifdef RGBLIGHT_EFFECT_RGB_TEST
    ANIM_STR(RGB_TEST)
#endif
#ifdef RGBLIGHT_EFFECT_STATIC_GRADIENT
    ANIM_STR_MULTI(STATIC_GRADIENT)
#endif
#ifdef RGBLIGHT_EFFECT_CHRISTMAS
    ANIM_STR(CHRISTMAS)
#endif
#ifdef RGBLIGHT_EFFECT_KNIGHT
    ANIM_STR_MULTI(KNIGHT)
#endif
#ifdef RGBLIGHT_EFFECT_SNAKE
    ANIM_STR_MULTI(SNAKE)
#endif
#ifdef RGBLIGHT_EFFECT_RAINBOW_SWIRL
    ANIM_STR_MULTI(RAINBOW_SWIRL)
#endif
#ifdef RGBLIGHT_EFFECT_RAINBOW_MOOD
    ANIM_STR_MULTI(RAINBOW_MOOD)
#endif
#ifdef RGBLIGHT_EFFECT_BREATHING
    ANIM_STR_MULTI(BREATHING)
#endif
    ANIM_STR(STATIC_LIGHT)
    default: return "???";
    }
}

bool oled_task_user(void) {
    if (IS_LAYER_ON(_TRACKBALL)) {
        oled_write_P(PSTR("\nMode:\n"), false);
        uint8_t mode = rgblight_get_mode();
        oled_write_P(get_anim_str(&mode), false);
        char modebuf[5];
        snprintf(modebuf, 5, " %2d\n", mode);
        oled_write(modebuf, false);
    } else {
        render_logo();
    }
    oled_write_layer_state();
    return false;
}
#endif

