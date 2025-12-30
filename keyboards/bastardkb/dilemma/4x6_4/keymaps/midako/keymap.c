/**
 * Copyright 2021 Charly Delay <charly@codesink.dev> (@0xcharly)
 * Copyright 2023 casuanoob <casuanoob@hotmail.com> (@casuanoob)
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
#include QMK_KEYBOARD_H

enum dilemma_keymap_layers {
    LAYER_BASE = 0,
    LAYER_NAVIGATION,
    LAYER_POINTER,
    LAYER_NUMERAL,
    LAYER_SYMBOLS,
    LAYER_FUNCTION,
    LAYER_MEDIA,
};

// Automatically enable sniping-mode on the pointer layer.
// #define DILEMMA_AUTO_SNIPING_ON_LAYER LAYER_POINTER

#define LOWER MO(LAYER_LOWER)
#define RAISE MO(LAYER_RAISE)
#define PT_Z LT(LAYER_POINTER, KC_Z)
#define PT_SLSH LT(LAYER_POINTER, KC_SLSH)

#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

#ifdef ENCODER_MAP_ENABLE
#    define LENC XXXXXXX
#    define RENC XXXXXXX
#else
#    define LENC XXXXXXX
#    define RENC XXXXXXX
#endif

#define ESC_MED LT(LAYER_MEDIA, KC_ESC)
#define SPC_NAV LT(LAYER_NAVIGATION, KC_SPC)
#define LA2_NAV LT(LAYER_NAVIGATION, KC_DOT)
#define TAB_PTR LT(LAYER_POINTER, KC_TAB)
#define LA2_PTR LT(LAYER_POINTER, KC_COMM)
#define LA3_PTR LT(LAYER_POINTER, KC_END)
#define DEL_FUN LT(LAYER_FUNCTION, KC_DEL)
#define BSP_NUM LT(LAYER_NUMERAL, KC_BSPC)
#define ENT_SYM LT(LAYER_SYMBOLS, KC_ENT)

// Combos
const uint16_t PROGMEM combo_layer_lock_med[] = {ESC_MED, CW_TOGG, COMBO_END};
const uint16_t PROGMEM combo_layer_lock_nav[] = {SPC_NAV, CW_TOGG, COMBO_END};
const uint16_t PROGMEM combo_layer_lock_ptr[] = {TAB_PTR, CW_TOGG, COMBO_END};
const uint16_t PROGMEM combo_layer_lock_fun[] = {DEL_FUN, KC_CAPS, COMBO_END};
const uint16_t PROGMEM combo_layer_lock_num[] = {BSP_NUM, KC_CAPS, COMBO_END};
const uint16_t PROGMEM combo_layer_lock_sym[] = {ENT_SYM, KC_CAPS, COMBO_END};

/**
 * \brief Add Home Row mod to a layout.
 *
 * Expects a 12-key per row layout, at least x3 rows (at least 36 keycodes).
 * Adds support for GACS (Gui, Alt, Ctl, Shift) home row.
 *
 * This is meant to be used with `LAYER_ALPHAS_QWERTY` defined above, eg.:
 *
 *     HOME_ROW_MOD_GACS(LAYER_ALPHAS_QWERTY)
 */
#define _HOME_ROW_MOD_GACS(L00, L01, L02, L03, L04, L05, R06, R07, R08, R09, R10, R11, L12, L13, L14, L15, L16, L17, R18, R19, R20, R21, R22, R23, L24, L25, L26, L27, L28, L29, R30, R31, R32, R33, R34, R35, ...) L00, L01, L02, L03, L04, L05, R06, R07, R08, R09, R10, R11, L12, L13, L14, L15, L16, L17, R18, R19, R20, R21, R22, R23, L24, LGUI_T(L25), LALT_T(L26), LCTL_T(L27), LSFT_T(L28), L29, R30, RSFT_T(R31), RCTL_T(R32), LALT_T(R33), LGUI_T(R34), R35, __VA_ARGS__
#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)

#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [LAYER_BASE] = LAYOUT_wrapper(HOME_ROW_MOD_GACS(
  // ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮
        KC_F11,  KC_F1,  KC_F2,  KC_F3,  KC_F4,  KC_F5,         KC_F6,  KC_F7,  KC_F8,  KC_F9, KC_F10, KC_F12,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_APP,   KC_Q,   KC_W,   KC_F,   KC_P,   KC_B,         KC_J,   KC_L,   KC_U,   KC_Y,KC_QUOT,KC_SLSH,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        KC_PSCR,   KC_A,   KC_R,   KC_S,   KC_T,   KC_G,        KC_M,   KC_N,   KC_E,   KC_I,   KC_O,KC_SCLN,
  // ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤
        CW_TOGG,   KC_Z,   KC_X,   KC_C,   KC_D,   KC_V,        KC_K,   KC_H,KC_MINS,LA2_PTR,LA2_NAV,KC_CAPS,
  // ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯
                         LENC, ESC_MED,SPC_NAV,TAB_PTR,         DEL_FUN,ENT_SYM,BSP_NUM,RENC
  //                    ╰───────────────────────────────────╯ ╰───────────────────────────────────╯
  )),
};
// clang-format on

combo_t key_combos[] = {
    COMBO(combo_layer_lock_med, TG(LAYER_MEDIA)), COMBO(combo_layer_lock_nav, TG(LAYER_NAVIGATION)), COMBO(combo_layer_lock_ptr, TG(LAYER_POINTER)), COMBO(combo_layer_lock_fun, TG(LAYER_FUNCTION)), COMBO(combo_layer_lock_num, TG(LAYER_NUMERAL)), COMBO(combo_layer_lock_sym, TG(LAYER_SYMBOLS)),
};

bool caps_word_press_user(uint16_t keycode) {
    switch (keycode) {
        // Keycodes that continue Caps Word, with shift applied.
        case KC_A ... KC_Z:
        case KC_MINS:
            add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
            return true;

        // Keycodes that continue Caps Word, without shifting.
        case KC_1 ... KC_0:
        case KC_BSPC:
        case KC_DEL:
        case KC_UNDS:
        // extra from defaults
        case KC_SLSH:
        case KC_LEFT:
        case KC_RGHT:
            return true;

        default:
            return false; // Deactivate Caps Word.
    }
}

#ifdef POINTING_DEVICE_ENABLE
#    ifdef DILEMMA_AUTO_SNIPING_ON_LAYER
layer_state_t layer_state_set_user(layer_state_t state) {
    dilemma_set_pointer_sniping_enabled(layer_state_cmp(state, DILEMMA_AUTO_SNIPING_ON_LAYER));
    return state;
}
#    endif // DILEMMA_AUTO_SNIPING_ON_LAYER
#endif     // POINTING_DEVICE_ENABLEE

#ifdef RGB_MATRIX_ENABLE
// Forward-declare this helper function since it is defined in rgb_matrix.c.
void rgb_matrix_update_pwm_buffers(void);
#endif // RGB_MATRIX_ENABLE

#ifdef ENCODER_MAP_ENABLE
// clang-format off
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [LAYER_BASE]       = {ENCODER_CCW_CW(KC_WH_U, KC_WH_D), ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
    // [LAYER_POINTER]    = {ENCODER_CCW_CW(RGB_HUD, RGB_HUI), ENCODER_CCW_CW(RGB_SAD, RGB_SAI)},
};
// clang-format on
#endif // ENCODER_MAP_ENABLE
