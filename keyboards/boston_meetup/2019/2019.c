/* Copyright 2019 ishtob
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "2019.h"
#include "qwiic.h"
#include "action_layer.h"
#include "haptic.h"
#include <printf.h>

#ifdef RGB_MATRIX_ENABLE
#include "rgb_matrix.h"

led_config_t g_led_config = { {
  {   5, NO_LED, NO_LED,   0 },
  { NO_LED, NO_LED, NO_LED, NO_LED },
  {   4, NO_LED, NO_LED,   1 },
  {   3, NO_LED, NO_LED,   2 }
}, {
    { 188,  16 }, { 187,  48 }, { 149,  64 }, { 112,  64 }, {  37,  48 }, {  38,  16 }
}, {
    4, 4, 4, 4, 4, 4
} };
#endif

uint8_t *o_fb;

uint16_t counterst = 0;



#ifdef QWIIC_MICRO_OLED_ENABLE

/* screen off after this many milliseconds */
#include "timer.h"
#define ScreenOffInterval 60000 /* milliseconds */
static uint16_t last_flush;

volatile uint8_t led_numlock = false;
volatile uint8_t  led_capslock = false;
volatile uint8_t  led_scrolllock = false;

static uint8_t layer;
static bool queue_for_send = false;
static bool screen_is_off = false;
static uint8_t encoder_value = 32;

__attribute__ ((weak))
void draw_ui(void) {
  clear_buffer();
  last_flush = timer_read();
  if (screen_is_off) {
    send_command(DISPLAYON);
    screen_is_off = false;
  }

/* Layer indicator is 61 x 10 pixels */
#define LAYER_INDICATOR_X 0
#define LAYER_INDICATOR_Y 0

  draw_string(LAYER_INDICATOR_X + 1, LAYER_INDICATOR_Y + 2, "LAYER", PIXEL_ON, NORM, 0);
  switch (layer) {
    case 0:
      draw_rect_filled_soft(LAYER_INDICATOR_X + 32, LAYER_INDICATOR_Y + 1, 15, 9, PIXEL_ON, NORM);
      draw_string(LAYER_INDICATOR_X + 34, LAYER_INDICATOR_Y + 2, "WM", PIXEL_ON, XOR, 0);
      break;
    case 1:
      draw_rect_filled_soft(LAYER_INDICATOR_X + 32, LAYER_INDICATOR_Y + 1, 27, 9, PIXEL_ON, NORM);
      draw_string(LAYER_INDICATOR_X + 34, LAYER_INDICATOR_Y + 2, "TMUX", PIXEL_ON, XOR, 0);
      break;
    case 2:
      draw_rect_filled_soft(LAYER_INDICATOR_X + 32, LAYER_INDICATOR_Y + 1, 21, 9, PIXEL_ON, NORM);
      draw_string(LAYER_INDICATOR_X + 34, LAYER_INDICATOR_Y + 2, "RGB", PIXEL_ON, XOR, 0);
      break;
    case 3:
      draw_rect_filled_soft(LAYER_INDICATOR_X + 32, LAYER_INDICATOR_Y + 1, 27, 9, PIXEL_ON, NORM);
      draw_string(LAYER_INDICATOR_X + 34, LAYER_INDICATOR_Y + 2, "UTIL", PIXEL_ON, XOR, 0);
      break;
    case 4:
      draw_rect_filled_soft(LAYER_INDICATOR_X + 32, LAYER_INDICATOR_Y + 1, 21, 9, PIXEL_ON, NORM);
      draw_string(LAYER_INDICATOR_X + 34, LAYER_INDICATOR_Y + 2, "NUM", PIXEL_ON, XOR, 0);
      break;
  }

/* Matrix display is 12 x 12 pixels */
#define MATRIX_DISPLAY_X 8
#define MATRIX_DISPLAY_Y 16

  for (uint8_t x = 0; x < MATRIX_ROWS; x++) {
    for (uint8_t y = 0; y < MATRIX_COLS; y++) {
      draw_pixel(MATRIX_DISPLAY_X + y + y + 2, MATRIX_DISPLAY_Y + x + x + 2,(matrix_get_row(x) & (1 << y)) > 0, NORM);
      draw_pixel(MATRIX_DISPLAY_X + y + y + 3, MATRIX_DISPLAY_Y + x + x + 2,(matrix_get_row(x) & (1 << y)) > 0, NORM);
      draw_pixel(MATRIX_DISPLAY_X + y + y + 2, MATRIX_DISPLAY_Y + x + x + 3,(matrix_get_row(x) & (1 << y)) > 0, NORM);
      draw_pixel(MATRIX_DISPLAY_X + y + y + 3, MATRIX_DISPLAY_Y + x + x + 3,(matrix_get_row(x) & (1 << y)) > 0, NORM);

    }
  }
  draw_rect_soft(MATRIX_DISPLAY_X, MATRIX_DISPLAY_Y, 12, 12, PIXEL_ON, NORM);
  /* hadron oled location on thumbnail */
  draw_rect_filled_soft(MATRIX_DISPLAY_X + 5, MATRIX_DISPLAY_Y + 2, 6, 2, PIXEL_ON, NORM);
/*
  draw_rect_soft(0, 13, 64, 6, PIXEL_ON, NORM);
  draw_line_vert(encoder_value, 13, 6, PIXEL_ON, NORM);

*/

/* Mod display is 41 x 16 pixels */
#define MOD_DISPLAY_X 70
#define MOD_DISPLAY_Y 0

  uint8_t mods = get_mods();
  if (mods & MOD_LSFT) {
    draw_rect_filled_soft(MOD_DISPLAY_X + 0, MOD_DISPLAY_Y, 5 + (1 * 6), 9, PIXEL_ON, NORM);
    draw_string(MOD_DISPLAY_X + 3, MOD_DISPLAY_Y + 1, "S", PIXEL_OFF, NORM, 0);
  } else {
    draw_string(MOD_DISPLAY_X + 3, MOD_DISPLAY_Y + 1, "S", PIXEL_ON, NORM, 0);
  }
  if (mods & MOD_LCTL) {
    draw_rect_filled_soft(MOD_DISPLAY_X + 10, MOD_DISPLAY_Y, 5 + (1 * 6), 9, PIXEL_ON, NORM);
    draw_string(MOD_DISPLAY_X + 13, MOD_DISPLAY_Y + 1, "C", PIXEL_OFF, NORM, 0);
  } else {
    draw_string(MOD_DISPLAY_X + 13, MOD_DISPLAY_Y + 1, "C", PIXEL_ON, NORM, 0);
  }
  if (mods & MOD_LALT) {
    draw_rect_filled_soft(MOD_DISPLAY_X + 20, MOD_DISPLAY_Y, 5 + (1 * 6), 9, PIXEL_ON, NORM);
    draw_string(MOD_DISPLAY_X + 23, MOD_DISPLAY_Y + 1, "A", PIXEL_OFF, NORM, 0);
  } else {
    draw_string(MOD_DISPLAY_X + 23, MOD_DISPLAY_Y + 1, "A", PIXEL_ON, NORM, 0);
  }
  if (mods & MOD_LGUI) {
    draw_rect_filled_soft(MOD_DISPLAY_X + 30, MOD_DISPLAY_Y, 5 + (1 * 6), 9, PIXEL_ON, NORM);
    draw_string(MOD_DISPLAY_X + 33, MOD_DISPLAY_Y + 1, "G", PIXEL_OFF, NORM, 0);
  } else {
    draw_string(MOD_DISPLAY_X + 33, MOD_DISPLAY_Y + 1, "G", PIXEL_ON, NORM, 0);
  }

/* Lock display is 23 x 32 */
#define LOCK_DISPLAY_X 117
#define LOCK_DISPLAY_Y 0

  if (led_numlock == true) {
    draw_rect_filled_soft(LOCK_DISPLAY_X, LOCK_DISPLAY_Y, 11, 9, PIXEL_ON, NORM);
    draw_string(LOCK_DISPLAY_X + 3, LOCK_DISPLAY_Y + 1, "N", PIXEL_OFF, NORM, 0);
  } else if (led_numlock == false) {
    draw_string(LOCK_DISPLAY_X + 3, LOCK_DISPLAY_Y + 1, "N", PIXEL_ON, NORM, 0);
  }
  if (led_capslock == true) {
    draw_rect_filled_soft(LOCK_DISPLAY_X + 0, LOCK_DISPLAY_Y + 11, 11, 9, PIXEL_ON, NORM);
    draw_string(LOCK_DISPLAY_X + 3, LOCK_DISPLAY_Y + 11 +1, "C", PIXEL_OFF, NORM, 0);
  } else if (led_capslock == false) {
    draw_string(LOCK_DISPLAY_X + 3, LOCK_DISPLAY_Y + 11 +1, "C", PIXEL_ON, NORM, 0);
  }

  if (led_scrolllock == true) {
    draw_rect_filled_soft(LOCK_DISPLAY_X + 0, LOCK_DISPLAY_Y + 22, 11, 9, PIXEL_ON, NORM);
    draw_string(LOCK_DISPLAY_X + 3, LOCK_DISPLAY_Y + 22 +1, "S", PIXEL_OFF, NORM, 0);
  } else if (led_scrolllock == false) {
    draw_string(LOCK_DISPLAY_X + 3, LOCK_DISPLAY_Y + 22 +1, "S", PIXEL_ON, NORM, 0);
  }

/* LED mode display is 40 x 10 */

#define LED_MODE_DISPLAY_X 30
#define LED_MODE_DISPLAY_Y 12

  draw_string(LED_MODE_DISPLAY_X, LED_MODE_DISPLAY_Y + 1, "LED", PIXEL_ON, NORM, 0);
  uint8_t mode = rgblight_get_mode();
  switch (mode) {
    case RGBLIGHT_MODE_STATIC_LIGHT:
      draw_rect_filled_soft(LED_MODE_DISPLAY_X + 19, LED_MODE_DISPLAY_Y, 3 + (6 * 6), 9, PIXEL_ON, NORM);
      draw_string(LED_MODE_DISPLAY_X + 21, LED_MODE_DISPLAY_Y + 1, "STATIC", PIXEL_ON, XOR, 0);
      break;
    case RGBLIGHT_MODE_BREATHING:
    case RGBLIGHT_MODE_BREATHING+1:
    case RGBLIGHT_MODE_BREATHING+2:
    case RGBLIGHT_MODE_BREATHING+3:
      draw_rect_filled_soft(LED_MODE_DISPLAY_X + 19, LED_MODE_DISPLAY_Y, 3 + (6 * 9), 9, PIXEL_ON, NORM);
      draw_string(LED_MODE_DISPLAY_X + 21, LED_MODE_DISPLAY_Y + 1, "BREATHE ", PIXEL_ON, XOR, 0);
      draw_char(LED_MODE_DISPLAY_X + 21 + (6 * 8), LED_MODE_DISPLAY_Y + 1,
                (mode - RGBLIGHT_MODE_BREATHING) + 0x30, PIXEL_ON, XOR, 0);
      break;
    case RGBLIGHT_MODE_RAINBOW_MOOD:
    case RGBLIGHT_MODE_RAINBOW_MOOD+1:
    case RGBLIGHT_MODE_RAINBOW_MOOD+2:
      draw_rect_filled_soft(LED_MODE_DISPLAY_X + 19, LED_MODE_DISPLAY_Y, 3 + (6 * 6), 9, PIXEL_ON, NORM);
      draw_string(LED_MODE_DISPLAY_X + 21, LED_MODE_DISPLAY_Y + 1, "MOOD ", PIXEL_ON, XOR, 0);
      draw_char(LED_MODE_DISPLAY_X + 21 + (6 * 5), LED_MODE_DISPLAY_Y + 1,
                (mode - RGBLIGHT_MODE_RAINBOW_MOOD) + 0x30, PIXEL_ON, XOR, 0);
      break;
    case RGBLIGHT_MODE_RAINBOW_SWIRL:
    case RGBLIGHT_MODE_RAINBOW_SWIRL+1:
    case RGBLIGHT_MODE_RAINBOW_SWIRL+2:
    case RGBLIGHT_MODE_RAINBOW_SWIRL+3:
    case RGBLIGHT_MODE_RAINBOW_SWIRL+4:
    case RGBLIGHT_MODE_RAINBOW_SWIRL+5:
      draw_rect_filled_soft(LED_MODE_DISPLAY_X + 19, LED_MODE_DISPLAY_Y, 3 + (6 * 7), 9, PIXEL_ON, NORM);
      draw_string(LED_MODE_DISPLAY_X + 21, LED_MODE_DISPLAY_Y + 1, "SWIRL ", PIXEL_ON, XOR, 0);
      draw_char(LED_MODE_DISPLAY_X + 21 + (6 * 6), LED_MODE_DISPLAY_Y + 1,
                (mode - RGBLIGHT_MODE_RAINBOW_SWIRL) + 0x30, PIXEL_ON, XOR, 0);
      break;
    case RGBLIGHT_MODE_SNAKE:
    case RGBLIGHT_MODE_SNAKE+1:
    case RGBLIGHT_MODE_SNAKE+2:
    case RGBLIGHT_MODE_SNAKE+3:
    case RGBLIGHT_MODE_SNAKE+4:
    case RGBLIGHT_MODE_SNAKE+5:
      draw_rect_filled_soft(LED_MODE_DISPLAY_X + 19, LED_MODE_DISPLAY_Y, 3 + (6 * 7), 9, PIXEL_ON, NORM);
      draw_string(LED_MODE_DISPLAY_X + 21, LED_MODE_DISPLAY_Y + 1, "SNAKE ", PIXEL_ON, XOR, 0);
      draw_char(LED_MODE_DISPLAY_X + 21 + (6 * 6), LED_MODE_DISPLAY_Y + 1,
                (mode - RGBLIGHT_MODE_SNAKE) + 0x30, PIXEL_ON, XOR, 0);
      break;
    case RGBLIGHT_MODE_KNIGHT:
    case RGBLIGHT_MODE_KNIGHT+1:
    case RGBLIGHT_MODE_KNIGHT+2:
      draw_rect_filled_soft(LED_MODE_DISPLAY_X + 19, LED_MODE_DISPLAY_Y, 3 + (6 * 8), 9, PIXEL_ON, NORM);
      draw_string(LED_MODE_DISPLAY_X + 21, LED_MODE_DISPLAY_Y + 1, "KNIGHT ", PIXEL_ON, XOR, 0);
      draw_char(LED_MODE_DISPLAY_X + 21 + (6 * 7), LED_MODE_DISPLAY_Y + 1,
                (mode - RGBLIGHT_MODE_KNIGHT) + 0x30, PIXEL_ON, XOR, 0);
      break;
    case RGBLIGHT_MODE_CHRISTMAS:
      draw_rect_filled_soft(LED_MODE_DISPLAY_X + 19, LED_MODE_DISPLAY_Y, 3 + (6 * 4), 9, PIXEL_ON, NORM);
      draw_string(LED_MODE_DISPLAY_X + 21, LED_MODE_DISPLAY_Y + 1, "XMAS", PIXEL_ON, XOR, 0);
      break;
    case RGBLIGHT_MODE_STATIC_GRADIENT:
    case RGBLIGHT_MODE_STATIC_GRADIENT+1:
    case RGBLIGHT_MODE_STATIC_GRADIENT+2:
    case RGBLIGHT_MODE_STATIC_GRADIENT+3:
    case RGBLIGHT_MODE_STATIC_GRADIENT+4:
    case RGBLIGHT_MODE_STATIC_GRADIENT+5:
    case RGBLIGHT_MODE_STATIC_GRADIENT+6:
    case RGBLIGHT_MODE_STATIC_GRADIENT+7:
    case RGBLIGHT_MODE_STATIC_GRADIENT+8:
    case RGBLIGHT_MODE_STATIC_GRADIENT+9:
      draw_rect_filled_soft(LED_MODE_DISPLAY_X + 19, LED_MODE_DISPLAY_Y, 3 + (6 * 6), 9, PIXEL_ON, NORM);
      draw_string(LED_MODE_DISPLAY_X + 21, LED_MODE_DISPLAY_Y + 1, "GRAD ", PIXEL_ON, XOR, 0);
      draw_char(LED_MODE_DISPLAY_X + 21 + (6 * 5), LED_MODE_DISPLAY_Y + 1,
                (mode - RGBLIGHT_MODE_STATIC_GRADIENT) + 0x30, PIXEL_ON, XOR, 0);
      break;
    case RGBLIGHT_MODE_RGB_TEST:
      draw_rect_filled_soft(LED_MODE_DISPLAY_X + 19, LED_MODE_DISPLAY_Y, 3 + (6 * 4), 9, PIXEL_ON, NORM);
      draw_string(LED_MODE_DISPLAY_X + 21, LED_MODE_DISPLAY_Y + 1, "TEST", PIXEL_ON, XOR, 0);
      break;
    case RGBLIGHT_MODE_ALTERNATING:
      draw_rect_filled_soft(LED_MODE_DISPLAY_X + 19, LED_MODE_DISPLAY_Y, 3 + (6 * 3), 9, PIXEL_ON, NORM);
      draw_string(LED_MODE_DISPLAY_X + 21, LED_MODE_DISPLAY_Y + 1, "ALT", PIXEL_ON, XOR, 0);
      break;
    case RGBLIGHT_MODE_TWINKLE:
    case RGBLIGHT_MODE_TWINKLE+1:
    case RGBLIGHT_MODE_TWINKLE+2:
    case RGBLIGHT_MODE_TWINKLE+3:
    case RGBLIGHT_MODE_TWINKLE+4:
    case RGBLIGHT_MODE_TWINKLE+5:
      draw_rect_filled_soft(LED_MODE_DISPLAY_X + 19, LED_MODE_DISPLAY_Y, 3 + (6 * 9), 9, PIXEL_ON, NORM);
      draw_string(LED_MODE_DISPLAY_X + 21, LED_MODE_DISPLAY_Y + 1, "TWINKLE ", PIXEL_ON, XOR, 0);
      draw_char(LED_MODE_DISPLAY_X + 21 + (6 * 8), LED_MODE_DISPLAY_Y + 1,
                (mode - RGBLIGHT_MODE_TWINKLE) + 0x30, PIXEL_ON, XOR, 0);
      break;
    default:
      draw_string(LED_MODE_DISPLAY_X, LED_MODE_DISPLAY_Y, "LED ???", PIXEL_ON, NORM, 0);
      break;
  };

#define LED_HSV_DISPLAY_X 35
#define LED_HSV_DISPLAY_Y 23
  HSV val = rgblight_get_hsv();
  char h[3];
  char s[3];
  char v[3];
  sprintf(h, "%02x", val.h);
  sprintf(s, "%02x", val.s);
  sprintf(v, "%02x", val.v);
  draw_char(LED_HSV_DISPLAY_X, LED_HSV_DISPLAY_Y + 1, 'H', PIXEL_ON, NORM, 0);
  draw_rect_filled_soft(LED_HSV_DISPLAY_X + 7, LED_HSV_DISPLAY_Y, 14, 9, PIXEL_ON, NORM);
  draw_string(LED_HSV_DISPLAY_X + 9, LED_HSV_DISPLAY_Y + 1, h, PIXEL_ON, XOR, 0);
  draw_char(LED_HSV_DISPLAY_X + 23, LED_HSV_DISPLAY_Y + 1, 'S', PIXEL_ON, NORM, 0);
  draw_rect_filled_soft(LED_HSV_DISPLAY_X + 30, LED_HSV_DISPLAY_Y, 14, 9, PIXEL_ON, NORM);
  draw_string(LED_HSV_DISPLAY_X + 32, LED_HSV_DISPLAY_Y + 1, s, PIXEL_ON, XOR, 0);
  draw_char(LED_HSV_DISPLAY_X + 46, LED_HSV_DISPLAY_Y + 1, 'V', PIXEL_ON, NORM, 0);
  draw_rect_filled_soft(LED_HSV_DISPLAY_X + 53, LED_HSV_DISPLAY_Y, 14, 9, PIXEL_ON, NORM);
  draw_string(LED_HSV_DISPLAY_X + 55, LED_HSV_DISPLAY_Y + 1, v, PIXEL_ON, XOR, 0);


  send_buffer();
}

void led_set_user(uint8_t usb_led) {
  if (IS_LED_ON(usb_led, USB_LED_NUM_LOCK)) {
    if (led_numlock == false){led_numlock = true;}
  } else {
    if (led_numlock == true){led_numlock = false;}
  }
  if (IS_LED_ON(usb_led, USB_LED_CAPS_LOCK)) {
    if (led_capslock == false){led_capslock = true;}
  } else {
    if (led_capslock == true){led_capslock = false;}
  }
  if (IS_LED_ON(usb_led, USB_LED_SCROLL_LOCK)) {
    if (led_scrolllock == false){led_scrolllock = true;}
  } else {
    if (led_scrolllock == true){led_scrolllock = false;}
  }
}

uint32_t layer_state_set_kb(uint32_t state) {
  state = layer_state_set_user(state);
  layer = biton32(state);
  queue_for_send = true;
  return state;
}

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
  queue_for_send = true;
  return process_record_user(keycode, record);
}

void encoder_update_kb(uint8_t index, bool clockwise) {
  encoder_value = (encoder_value + (clockwise ? 1 : -1)) % 64;
  queue_for_send = true;
}

#endif

void matrix_init_kb(void) {
  queue_for_send = true;
  matrix_init_user();
}

void matrix_scan_kb(void) {
  matrix_scan_user();
  if (queue_for_send) {
#ifdef QWIIC_MICRO_OLED_ENABLE
    draw_ui();
#endif
    queue_for_send = false;
  }
#ifdef QWIIC_MICRO_OLED_ENABLE
  if (!screen_is_off && (timer_elapsed(last_flush) > ScreenOffInterval)) {
    send_command(DISPLAYOFF);      /* 0xAE */
    screen_is_off = true;
  }
#endif
}
