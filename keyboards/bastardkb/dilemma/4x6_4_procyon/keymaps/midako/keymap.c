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

#define MENU_X TG(LAYER_MENU)

#ifdef ENCODER_MAP_ENABLE
#    define LENC XXXXXXX
#    define RENC XXXXXXX
#else
#    define LENC XXXXXXX
#    define RENC XXXXXXX
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [LAYER_BASE] = LAYOUT_wrapper(HOME_ROW_MOD_GACS(
        // ╭───────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
                                                            COMMON_BASE_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                LENC, ESC_MED, SPC_NAV, TAB_PTR,       DEL_FUN, ENT_SYM, BSP_NUM, RENC
        //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
      )),
    [LAYER_QWERTY] = LAYOUT_wrapper(HOME_ROW_MOD_GACS(
        // ╭───────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
                                                            COMMON_QWERTY_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                LENC, ESC_MED, SPC_NAV, TAB_PTR,       DEL_FUN, ENT_SYM, BSP_NUM, RENC
        //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
      )),
    [LAYER_FUNCTION] = LAYOUT_wrapper(
        // ╭───────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
                                                            COMMON_FUNCTION_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                LENC, KC_APP, _______, KC_TAB,         _______, KC_ENT, KC_BSPC, RENC
        //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
        ),
    [LAYER_MEDIA] = LAYOUT_wrapper(
        // ╭───────────────────────────────────────────────────────────────────────────────────────────────────────────────╮
                                                            COMMON_MEDIA_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                LENC, _______, KC_MPLY, KC_MSTP,       XXXXXXX, KC_MSTP, KC_MPLY, RENC
        //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
    ),
    [LAYER_POINTER] = LAYOUT_wrapper(
        // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
                                                            COMMON_POINTER_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                LENC, KC_ESC, MS_BTN1, _______,        KC_DEL, KC_ENT, KC_BSPC, RENC
        //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
    ),
    [LAYER_NAVIGATION] = LAYOUT_wrapper(
        // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
                                                          COMMON_NAVIGATION_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                LENC, KC_ESC, _______, _______,        KC_DEL, KC_ENT, KC_BSPC, RENC
        //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
    ),
    [LAYER_NUMERAL] = LAYOUT_wrapper(
        // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
                                                            COMMON_NUMERAL_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                LENC, KC_DOT, KC_0, KC_SLSH,           KC_DEL, KC_ENT, _______, RENC
        //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
    ),
    [LAYER_SYMBOLS] = LAYOUT_wrapper(
        // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
                                                            COMMON_SYMBOLS_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                LENC, SYM_ARW, KC_SPC,  KC_DQUO,       KC_DEL, _______, KC_BSPC, RENC
        //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
    ),
    [LAYER_GAMING] = LAYOUT_wrapper(
        // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
                                                            COMMON_GAMING_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                LENC, KC_LCTL, KC_SPC,KC_TAB,          DEL_FUN, ENT_SYM, BSP_NUM, RENC
        //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
    ),
    // LCD menu: up/down to pick an entry, leave the layer to run it.
    [LAYER_MENU] = LAYOUT(
        // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
             MENU_X,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,  MENU_X,
        // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
            XXXXXXX,  XXXXXXX, XXXXXXX, LCDPR,   XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, LCDPR,   XXXXXXX, XXXXXXX, XXXXXXX,
        // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
            XXXXXXX,  XXXXXXX, XXXXXXX, LCDNE,   XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, LCDNE,   XXXXXXX, XXXXXXX, XXXXXXX,
        // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
            XXXXXXX,  XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                LENC,  MENU_X,  MENU_X,  XXXXXXX,      XXXXXXX,  MENU_X,  MENU_X, RENC
        //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
    ),
};
// clang-format on

#ifdef POINTING_DEVICE_ENABLE
#    ifdef DILEMMA_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    dilemma_set_pointer_sniping_enabled(layer_state_cmp(state, DILEMMA_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // DILEMMA_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLEE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in
// rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);

// QWERTY indicator on the base layer only: dilemma.c paints momentary layers after this.
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    if (is_qwerty_on() && get_highest_layer(layer_state) == 0) {
        for (uint8_t led = led_min; led < led_max; led++) {
            qwerty_led_color(led);
        }
    }
    return true;
}
#endif // RGB_MATRIX_ENABLE

#ifdef ENCODER_MAP_ENABLE
// clang-format off
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [LAYER_BASE]        = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_QWERTY]      = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_NAVIGATION]  = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_UNDO,KC_AGAIN)},
    [LAYER_POINTER]     = {ENCODER_CCW_CW(RM_HUED, RM_HUEU),                            ENCODER_CCW_CW(RM_SATD, RM_SATU)},
    [LAYER_NUMERAL]     = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_SYMBOLS]     = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_FUNCTION]    = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_MEDIA]       = {ENCODER_CCW_CW(KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK),    ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_GAMING]      = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_MENU]        = {ENCODER_CCW_CW(LCDPR, LCDNE),                                ENCODER_CCW_CW(LCDPR, LCDNE)},
};
// clang-format on
#endif // ENCODER_MAP_ENABLE

#ifdef COMMUNITY_MODULE_LAYER_MAP_ENABLE
#    include "layer_map.h"

// Maps the LCD grid to the physical matrix, as { col, row }. Rows 0-4: left half,
// rows 5-9: right half, both left to right as seen from above. The right half's
// matrix columns run outer to inner, hence 5-c. Thumb rows follow the LAYOUT order
// (encoder, then three thumbs); { 255, 255 } slots are empty and hidden on the LCD.
// clang-format off
keypos_t layer_remap[LAYER_MAP_ROWS][LAYER_MAP_COLS] = {
    {{0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}},
    {{0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 1}, {5, 1}},
    {{0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}, {5, 2}},
    {{0, 3}, {1, 3}, {2, 3}, {3, 3}, {4, 3}, {5, 3}},
    {{255, 255}, {255, 255}, {3, 4}, {1, 4}, {2, 4}, {4, 4}},
    {{5, 5}, {4, 5}, {3, 5}, {2, 5}, {1, 5}, {0, 5}},
    {{5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6}, {0, 6}},
    {{5, 7}, {4, 7}, {3, 7}, {2, 7}, {1, 7}, {0, 7}},
    {{5, 8}, {4, 8}, {3, 8}, {2, 8}, {1, 8}, {0, 8}},
    {{4, 9}, {2, 9}, {1, 9}, {3, 9}, {255, 255}, {255, 255}},
};
// clang-format on
#endif // COMMUNITY_MODULE_LAYER_MAP_ENABLE

#ifdef COMMUNITY_MODULE_LCD_ENABLE
const char *keycode_label_user(uint16_t keycode) {
    switch (keycode) {
        case SYM_ARW:
            return "->";
        case BASE_TOGG:
            return "QWE";
        default:
            return NULL;
    }
}

const char *lcd_layer_name(uint8_t layer) {
    switch (layer) {
        case LAYER_BASE:
            return "BASE";
        case LAYER_QWERTY:
            return "QWERTY";
        case LAYER_NAVIGATION:
            return "NAV";
        case LAYER_POINTER:
            return "POINTER";
        case LAYER_NUMERAL:
            return "NUMERAL";
        case LAYER_SYMBOLS:
            return "SYMBOLS";
        case LAYER_FUNCTION:
            return "FUNCTION";
        case LAYER_MEDIA:
            return "MEDIA";
        case LAYER_GAMING:
            return "GAMING";
        case LAYER_MENU:
            return "MENU";
        default:
            return "?";
    }
}
#endif // COMMUNITY_MODULE_LCD_ENABLE
