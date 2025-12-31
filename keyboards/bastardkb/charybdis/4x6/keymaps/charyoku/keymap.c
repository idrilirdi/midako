/**
 * Copyright 2025 Idril Geer
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
#include "../../../../common/common_keymap.h"

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
#    include "timer.h"
#endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

/** \brief Automatically enable sniping-mode on the pointer layer. */
#define CHARYBDIS_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
static uint16_t auto_pointer_layer_timer = 0;

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS 1000
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS

#    ifndef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#        define CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD 8
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD
#endif     // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#define ALT_SCL MT(MOD_RALT, KC_SCLN)
#define CTL_EQL MT(MOD_RCTL, KC_EQL)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_BASE] = LAYOUT_wrapper(HOME_ROW_MOD_GACS(
        // ╭───────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
                                                            COMMON_BASE_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                        ESC_MED,SPC_NAV,TAB_PTR,            ENT_SYM,BSP_NUM,
                                            ALT_SCL,CTL_EQL,            DEL_FUN
        //                            ╰───────────────────────────╯ ╰──────────────────╯
      )),
    [LAYER_FUNCTION] = LAYOUT_wrapper(
        // ╭───────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
                                                            COMMON_FUNCTION_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                        KC_APP,_______, KC_TAB,                                   KC_ENT,KC_BSPC, \
                                                _______,_______,                                  _______
        //                            ╰───────────────────────────╯ ╰──────────────────╯
        ),
    [LAYER_MEDIA] = LAYOUT_wrapper(
        // ╭───────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
                                                            COMMON_MEDIA_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                            _______,KC_MPLY,KC_MSTP,                                  KC_MSTP,KC_MPLY, \
                                    _______,_______,                                  XXXXXXX
  //                            ╰───────────────────────────╯ ╰──────────────────╯
    ),
    [LAYER_POINTER] = LAYOUT_wrapper(
        // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
                                                            COMMON_POINTER_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                             KC_ESC,KC_BTN1,_______,                                   KC_ENT,KC_BSPC, \
                                    KC_BTN2,KC_BTN3,                                   KC_DEL
  //                            ╰───────────────────────────╯ ╰──────────────────╯
    ),
    [LAYER_NAVIGATION] = LAYOUT_wrapper(
        // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
                                                          COMMON_NAVIGATION_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                             KC_ESC,_______,_______,                                   KC_ENT,KC_BSPC, \
                                    _______,_______,                                   KC_DEL
  //                            ╰───────────────────────────╯ ╰──────────────────╯
    ),
    [LAYER_NUMERAL] = LAYOUT_wrapper(
        // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
                                                            COMMON_NUMERAL_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                             KC_DOT,   KC_0,KC_SLSH,                                   KC_ENT,_______, \
                                    KC_LPRN,KC_RPRN,                                   KC_DEL
  //                            ╰───────────────────────────╯ ╰──────────────────╯
    ),
    [LAYER_SYMBOLS] = LAYOUT_wrapper(
        // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
                                                            COMMON_SYMBOLS_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                            KC_LPRN,KC_RPRN,KC_QUES,                                  _______,KC_BSPC, \
                                    _______,_______,                                   KC_DEL
  //                            ╰───────────────────────────╯ ╰──────────────────╯
    ),
    [LAYER_GAMING] = LAYOUT_wrapper(
        // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
                                                            COMMON_GAMING_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                              KC_LCTL,KC_SPC,KC_TAB,                                  ENT_SYM,BSP_NUM, \
                                    ALT_SCL,CTL_EQL,                                  DEL_FUN
  //                            ╰───────────────────────────╯ ╰──────────────────╯
    ),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE
report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    if (abs(mouse_report.x) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD || abs(mouse_report.y) > CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_THRESHOLD) {
        if (auto_pointer_layer_timer == 0) {
            layer_on(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
            rgb_matrix_mode_noeeprom(RGB_MATRIX_NONE);
            rgb_matrix_sethsv_noeeprom(HSV_GREEN);
#        endif // RGB_MATRIX_ENABLE
        }
        auto_pointer_layer_timer = timer_read();
    }
    return mouse_report;
}

void matrix_scan_user(void) {
    if (auto_pointer_layer_timer != 0 && TIMER_DIFF_16(timer_read(), auto_pointer_layer_timer) >= CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_TIMEOUT_MS) {
        auto_pointer_layer_timer = 0;
        layer_off(LAYER_POINTER);
#        ifdef RGB_MATRIX_ENABLE
        rgb_matrix_mode_noeeprom(RGB_MATRIX_DEFAULT_MODE);
#        endif // RGB_MATRIX_ENABLE
    }
}
#    endif // CHARYBDIS_AUTO_POINTER_LAYER_TRIGGER_ENABLE

#    ifdef CHARYBDIS_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    charybdis_set_pointer_sniping_enabled(layer_state_cmp(state, CHARYBDIS_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // CHARYBDIS_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLE

#ifdef MACCEL_ENABLE
//// MACCEL configs
//
// Design: https://www.desmos.com/calculator/p0etbmee57
// Design: https://www.desmos.com/calculator/fjjli56gow
// Mine butters with slow gnome-moute+accel
// #define MACCEL_TAKEOFF     0.7   // --/++ curve starts rising smoothlier/abruptlier
// #define MACCEL_GROWTH      0.25  // --/++ curve reaches max limit slower/faster
// #define MACCEL_OFFSET      4.7   // --/++ growth kicks in earlier/later
// #define MACCEL_LIMIT       9.0   // maximum acceleration factor

// #define MACCEL_TAKEOFF     1.0   // --/++ curve starts rising smoothlier/abruptlier
// #define MACCEL_GROWTH      0.56  // --/++ curve reaches max limit slower/faster
// #define MACCEL_OFFSET      4.8   // --/++ growth kicks in earlier/later
// #define MACCEL_LIMIT       8.0   // maximum acceleration factor

// #define MACCEL_TAKEOFF     1.07  // --/++ curve starts rising smoothlier/abruptlier
// #define MACCEL_GROWTH      0.56  // --/++ curve reaches max limit slower/faster
// #define MACCEL_OFFSET      4.5   // --/++ growth kicks in earlier/later
// #define MACCEL_LIMIT       8.0   // maximum acceleration factor

// Mine with Gnome-mouse: mid-speed + no-accel, 400DPI both mouse/scroll
#    define MACCEL_TAKEOFF 1.18 // --/++ curve starts rising smoothlier/abruptlier
#    define MACCEL_GROWTH 0.56  // --/++ curve reaches max limit slower/faster
#    define MACCEL_OFFSET 3.6   //--/++ growth kicks in earlier/later
#    define MACCEL_LIMIT 9.0    // maximum acceleration factor

// Wimads, old https://www.desmos.com/calculator/p0etbmee57
// New-letters: https://www.desmos.com/calculator/4ajz8f7bqb
// #define MACCEL_TAKEOFF  2.0   // --/++ curve starts rising smoothlier/abruptlier
// #define MACCEL_GROWTH   0.25  // --/++ curve reaches max limit slower/faster
// #define MACCEL_OFFSET   2.2   // --/++ growth kicks in earlier/later
// #define MACCEL_LIMIT    6.0   // maximum acceleration factor

// To view mouse's distance/velocity while configuring maccel,
// set `CONSOLE_ENABLE = yes` in `rules.mk` and uncomment the lines below,
// and run `qmk console` in the shell:
// #define MACCEL_DEBUG
// #undef PRINTF_SUPPORT_DECIMAL_SPECIFIERS
// #define PRINTF_SUPPORT_DECIMAL_SPECIFIERS 1

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
    return pointing_device_task_maccel(mouse_report);
}

void keyboard_post_init_user(void) {
    keyboard_post_init_maccel();
}

enum my_keycodes {
    MA_TAKEOFF = QK_USER, // mouse acceleration curve takeoff (initial acceleration) step key
    MA_GROWTH_RATE,       // mouse acceleration curve growth rate step key
    MA_OFFSET,            // mouse acceleration curve offset step key
    MA_LIMIT,             // mouse acceleration curve limit step key
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_maccel(keycode, record, MA_TAKEOFF, MA_GROWTH_RATE, MA_OFFSET, MA_LIMIT)) {
        return false;
    }
    /* insert your own macros here */
    return true;
}
#endif // MACCEL_ENABLE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in
// rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);

/**
 *
 * Led indices for RGBs.
 * C-strings so 0 cannot set the color of the 1st led.
 *
 * Indicate *caps-states* with top-inner keys
 * (left-side works only if `SPLIT_LAYER_STATE_ENABLE`)
 */
const int8_t caps_lock_leds[] = {20, // right-inner-top
                                 49, // right-inner-top
                                 -1};
const int8_t caps_word_leds[] = {
    // 21, // NOT WORKING, caps-word state not shared!
    50, // left-inner-2nd
    51, // left-inner-3rd
    -1};

const int8_t mod_shift_leds[] = {28, // left-3rd-thumb
                                 54, // right-2nd-thumb
                                 -1};
const int8_t mod_ctrl_leds[]  = {27, // left-2nd-thumb
                                 53, // right-1st-thumb
                                 -1};
const int8_t mod_alt_leds[]   = {26, // left-1st-thumb
                                 55, // right-bottom-thumb
                                 -1};

/**
 * On *Media-layer* just mark volume up/down/mute keys,
 * to let colors of other kbd funcs to shine.
 */
const int8_t media_layer_leds[] = {47, 42, 39, -1};

bool inarray(const int8_t *arr, int8_t led) {
    while (*arr != -1 && *arr != led)
        arr++;
    return *arr == led;
}

void rgb_matrix_colorify_led(uint8_t led, uint8_t mods, bool caps_lock, bool caps_word) {
    RGB  color     = {0, 0, 0};
    bool color_set = false;

    void set_color(int r, int g, int b) {
        color.r   = r;
        color.g   = g;
        color.b   = b;
        color_set = true;
    }

    if (led == 7 || led == 36) { // 2nd top-outers combine mod colors
        if (mods & MOD_MASK_SHIFT) {
            color.g   = 0xff;
            color_set = true;
        }
        if (mods & MOD_MASK_CTRL) {
            color.r   = 0xff;
            color_set = true;
        }
        if (mods & MOD_MASK_ALT) {
            color.b   = 0xff;
            color_set = true;
        }
    } else if (caps_lock && inarray(caps_lock_leds, led)) {
        set_color(RGB_WHITE);
    } else if (caps_word && inarray(caps_word_leds, led)) {
        set_color(RGB_WHITE);
    } else if ((mods & (MOD_MASK_SHIFT | MOD_MASK_GUI)) && inarray(mod_shift_leds, led)) {
        set_color(RGB_GREEN);
    } else if ((mods & (MOD_MASK_CTRL | MOD_MASK_GUI)) && inarray(mod_ctrl_leds, led)) {
        set_color(RGB_RED);
    } else if ((mods & (MOD_MASK_ALT | MOD_MASK_GUI)) && inarray(mod_alt_leds, led)) {
        set_color(RGB_BLUE);
    } else {
        switch (get_highest_layer(layer_state | default_layer_state)) {
            case LAYER_POINTER:
                set_color(RGB_YELLOW);
                break;
            case LAYER_FUNCTION:
                set_color(RGB_MAGENTA);
                break;
            case LAYER_NAVIGATION:
                set_color(RGB_CYAN);
                break;
            case LAYER_NUMERAL:
                set_color(RGB_BLUE);
                break;
            case LAYER_SYMBOLS:
                set_color(RGB_CORAL);
                break;
            case LAYER_MEDIA:
                if (inarray(media_layer_leds, led)) {
                    set_color(RGB_MAGENTA);
                }
                break;
            case LAYER_GAMING:
                set_color(RGB_GREEN);
                break;
            default:
                break;
        }
    }

    if (color_set) {
        rgb_matrix_set_color(led, color.r, color.g, color.b);
    }
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    uint8_t mods      = get_mods();
    bool    caps_lock = host_keyboard_led_state().caps_lock;
    bool    caps_word = is_caps_word_on();

    for (uint8_t led = led_min; led <= led_max; led++) {
        rgb_matrix_colorify_led(led, mods, caps_lock, caps_word);
    }

    return false;
}
#endif // RGB_MATRIX_ENABLE
