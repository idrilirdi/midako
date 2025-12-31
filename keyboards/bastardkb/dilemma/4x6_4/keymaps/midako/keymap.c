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
#include "keycodes.h"

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
                                LENC, KC_LPRN, KC_RPRN, KC_QUES,       KC_DEL, _______, KC_BSPC, RENC
        //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
    ),
    [LAYER_GAMING] = LAYOUT_wrapper(
        // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
                                                            COMMON_GAMING_ROWS,
        // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                                LENC, KC_LCTL, KC_SPC,KC_TAB,          DEL_FUN, ENT_SYM, BSP_NUM, RENC
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
#endif // RGB_MATRIX_ENABLE

#ifdef ENCODER_MAP_ENABLE
// clang-format off
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [LAYER_BASE]        = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_NAVIGATION]  = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_UNDO,KC_AGAIN)},
    [LAYER_POINTER]     = {ENCODER_CCW_CW(RM_HUED, RM_HUEU),                            ENCODER_CCW_CW(RM_SATD, RM_SATU)},
    [LAYER_NUMERAL]     = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_SYMBOLS]     = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_FUNCTION]    = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_MEDIA]       = {ENCODER_CCW_CW(KC_MEDIA_PREV_TRACK, KC_MEDIA_NEXT_TRACK),    ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    [LAYER_GAMING]      = {ENCODER_CCW_CW(MS_WHLU, MS_WHLD),                            ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
};
// clang-format on
#endif // ENCODER_MAP_ENABLE
