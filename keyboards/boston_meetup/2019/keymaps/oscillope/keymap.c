#include QMK_KEYBOARD_H

// Each layer gets a name for readability, which is then used in the keymap matrix below.
// The underscores don't mean anything - you can have a layer called STUFF or any other name.
// Layer names don't all need to be of the same length, obviously, and you can also skip them
// entirely and just use numbers.

enum custom_layers {
  _BASE,
  _LOWER,
  _RAISE,
  _ADJUST,
  _NUM
};

enum custom_keycodes {
  BASE = SAFE_RANGE,
  TM_1,
  TM_2,
  TM_3,
  TM_4,
  TM_5,
  TM_6,
  TM_W,
  TM_NXT,
  TM_Q,
  TM_VS,
  TM_HS,
  TM_CP
};

enum td_actions {
  WMMV1,
  WMMV2,
  WMMV3,
  WMMV4,
  WMMV5,
  WMMV6,
  WMMV7,
  WMMV8,
  WMMV9,
  LS_BASE,
  LS_LOWER,
  LS_RAISE,
  LS_ADJUST,
};

void td_wm_mv_finished(qk_tap_dance_state_t *state, void *user_data) {
  uint8_t mods = MOD_BIT(KC_LGUI);
  if (state->count > 1) {
    mods |= MOD_BIT(KC_LSHIFT);
  }
  register_mods(mods);
  register_code((state->keycode & 0xff) + 30);
  if (state->count > 2) {
    unregister_code((state->keycode & 0xff) + 30);
    unregister_mods(MOD_BIT(KC_LSHIFT));
    register_code((state->keycode & 0xff) + 30);
  }
}

void td_wm_mv_reset(qk_tap_dance_state_t *state, void *user_data) {
  uint8_t mods = MOD_BIT(KC_LGUI);
  unregister_code((state->keycode & 0xff) + 30);
  if (state->count == 2) {
    mods |= MOD_BIT(KC_LSHIFT);
  }
  unregister_mods(mods);
}

void td_layer_switch(qk_tap_dance_state_t *state, void *user_data) {
  uint8_t layer = state->keycode - LS_BASE;
  if (state->count == 1) {
    layer = (layer + 1) % 4;
  } else if (layer > _BASE) {
    layer--;
  } else {
    layer = _ADJUST;
  }
  layer_move(layer);
}

qk_tap_dance_action_t tap_dance_actions[] = {
  [WMMV1] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_wm_mv_finished, td_wm_mv_reset),
  [WMMV2] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_wm_mv_finished, td_wm_mv_reset),
  [WMMV3] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_wm_mv_finished, td_wm_mv_reset),
  [WMMV4] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_wm_mv_finished, td_wm_mv_reset),
  [WMMV5] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_wm_mv_finished, td_wm_mv_reset),
  [WMMV6] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_wm_mv_finished, td_wm_mv_reset),
  [WMMV7] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_wm_mv_finished, td_wm_mv_reset),
  [WMMV8] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_wm_mv_finished, td_wm_mv_reset),
  [WMMV9] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_wm_mv_finished, td_wm_mv_reset),
  [LS_BASE] = ACTION_TAP_DANCE_FN(td_layer_switch),
  [LS_LOWER] = ACTION_TAP_DANCE_FN(td_layer_switch),
  [LS_RAISE] = ACTION_TAP_DANCE_FN(td_layer_switch),
  [LS_ADJUST] = ACTION_TAP_DANCE_FN(td_layer_switch),
};

const rgblight_segment_t PROGMEM tmux_led_layer[] = RGBLIGHT_LAYER_SEGMENTS( {2, 2, HSV_GREEN} );
const rgblight_segment_t PROGMEM num_led_layer[] = RGBLIGHT_LAYER_SEGMENTS( {0, 6, HSV_PURPLE} );
const rgblight_segment_t PROGMEM util_led_layer[] = RGBLIGHT_LAYER_SEGMENTS( {0, 6, HSV_RED} );
const rgblight_segment_t* const PROGMEM my_rgb_layers[] = RGBLIGHT_LAYERS_LIST(
  tmux_led_layer,
  num_led_layer,
  util_led_layer
);

void keyboard_post_init_user(void) {
  rgblight_layers = my_rgb_layers;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Base - WM controls, move window to tag X
 * ,------.
 * | Next |
 * |------+------+-------------.
 * | Cycle|   7  |   8  |   9  | 
 * |------+------+------+------|
 * | Ctrl |   4  |   5  |   6  | 
 * |------+------+------+------|
 * | Alt  |   1  |   2  |   3  |
 * `---------------------------'
 */
[_BASE] = LAYOUT(
  TD(LS_BASE),
  LGUI(KC_J), TD(WMMV7), TD(WMMV8), TD(WMMV9),
  KC_LCTL, TD(WMMV4), TD(WMMV5), TD(WMMV6),
  KC_LALT, TD(WMMV1), TD(WMMV2), TD(WMMV3)
),

/* Lower - Tmux controls
 * ,------.
 * | Next |
 * |------+------+-------------.
 * | C-b n| C-b %| C-b "| C-b [|
 * |------+------+------+------|
 * | C-b q|   4  |   5  |   6  |
 * |------+------+------+------|
 * | C-b )|   1  |   2  |   3  |
 * `---------------------------'
 */
[_LOWER] = LAYOUT(
  TD(LS_LOWER),
  TM_NXT , TM_HS  , TM_VS  , TM_CP,
  TM_Q   , TM_4   , TM_5   , TM_6 ,
  TM_W   , TM_1   , TM_2   , TM_3
),

/* Raise
 * ,------.
 * | Next | 
 * |------+------+-------------.
 * |RGB TG|RGB M+|RGB M-|      |
 * |------+------+------+------|
 * |      |RGB H+|RGB S+|RGB V+| 
 * |------+------+------+------|
 * |      |RGB H-|RGB S-|RGB V-|
 * `---------------------------'
 */
[_RAISE] = LAYOUT(
  TD(LS_RAISE),
  RGB_TOG, RGB_MOD, RGB_RMOD, XXXXXXX,
  _______, RGB_HUI, RGB_SAI,  RGB_VAI,
  _______, RGB_HUD, RGB_SAD,  RGB_VAD

),

/* Adjust
 * ,------.
 * | Next | 
 * |------+------+-------------.
 * |HPT TG|HPT FB|HPT RS| BKSP | 
 * |------+------+------+------|
 * |      |HPT M+|      |      | 
 * |------+------+------+------|
 * | DFU  |HPT M-|Clk TG| Del  |
 * `---------------------------'
 */
[_ADJUST] = LAYOUT(
  TD(LS_ADJUST),
  HPT_TOG, HPT_FBK,  HPT_RST, KC_BSPC,
  _______, HPT_MODI, XXXXXXX, XXXXXXX,
  RESET,   HPT_MODD, CK_TOGG, KC_DEL
),

/* Num - Numpad
 * ,------.
 * | Esc  |
 * |------+------+-------------.
 * |      |   7  |   8  |   9  | 
 * |------+------+------+------|
 * |      |   4  |   5  |   6  | 
 * |------+------+------+------|
 * |   0  |   1  |   2  |   3  |
 * `---------------------------'
 */
[_NUM] = LAYOUT(
  TO(_BASE),
  XXXXXXX, KC_7   , KC_8   , KC_9   ,
  XXXXXXX, KC_4   , KC_5   , KC_6   ,
  KC_0   , KC_1   , KC_2   , KC_3
),

};

layer_state_t layer_state_set_user(layer_state_t state) {
  rgblight_set_layer_state(0, layer_state_cmp(state, _LOWER));
  rgblight_set_layer_state(1, layer_state_cmp(state, _NUM));
  rgblight_set_layer_state(2, layer_state_cmp(state, _ADJUST));
  return state;
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case TM_1:
    case TM_2:
    case TM_3:
    case TM_4:
    case TM_5:
    case TM_6:
      if (record->event.pressed) {
        const char num[2] = {(keycode - TM_1) + '1', '\0'};
        SEND_STRING(SS_LCTL("b"));
        send_string(num);
      }
      break;
    case TM_W:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL("b") "w");
        layer_on(_NUM);
      } else {
        layer_off(_NUM);
      }
      return false;
    case TM_NXT:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL("b") "n");
      }
      break;
    case TM_Q:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL("b") "q");
        layer_on(_NUM);
      } else {
        layer_off(_NUM);
      }
      return false;
    case TM_VS:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL("b") "%");
      }
      break;
    case TM_HS:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL("b") "\"");
      }
      break;
    case TM_CP:
      if (record->event.pressed) {
        SEND_STRING(SS_LCTL("b") "[");
      }
      break;
  }
  return true;
}

void matrix_init_user(void) {
}


void matrix_scan_user(void) {
}
