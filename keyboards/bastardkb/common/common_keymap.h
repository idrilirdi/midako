// Common keymap definitions for charyoku and midako
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
#pragma once
#include QMK_KEYBOARD_H

enum midako_keymap_layers {
    LAYER_BASE = 0,
    LAYER_NAVIGATION,
    LAYER_POINTER,
    LAYER_NUMERAL,
    LAYER_SYMBOLS,
    LAYER_FUNCTION,
    LAYER_MEDIA,
    LAYER_GAMING,
};
#ifndef POINTING_DEVICE_ENABLE
#    define DRGSCRL KC_NO
#    define DPI_MOD KC_NO
#    define S_D_MOD KC_NO
#    define SNIPING KC_NO
#endif // !POINTING_DEVICE_ENABLE

#define LCK_SCRN LGUI(KC_L)
#define ESC_MED LT(LAYER_MEDIA, KC_ESC)
#define SPC_NAV LT(LAYER_NAVIGATION, KC_SPC)
#define LA2_NAV LT(LAYER_NAVIGATION, KC_DOT)
#define TAB_PTR LT(LAYER_POINTER, KC_TAB)
#define LA2_PTR LT(LAYER_POINTER, KC_COMM)
#define LA3_PTR LT(LAYER_POINTER, KC_END)
#define DEL_FUN LT(LAYER_FUNCTION, KC_DEL)
#define BSP_NUM LT(LAYER_NUMERAL, KC_BSPC)
#define ENT_SYM LT(LAYER_SYMBOLS, KC_ENT)
#define TGL_GM TG(LAYER_GAMING)

/** Convenience row shorthands. */
#define _________________DEAD_HALF_ROW_________________ XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX
#define _________________TRNS_HALF_ROW_________________ _______, _______, _______, _______, _______, _______
#define ________________HOME_ROW_GACS_L________________ _______, KC_LGUI, KC_LALT, KC_LCTL, KC_LSFT, XXXXXXX
#define ________________HOME_ROW_GACS_R________________ XXXXXXX, KC_LSFT, KC_LCTL, KC_LALT, KC_LGUI, _______
#define ________________KEYB_CTRL_ROW_L________________ QK_BOOT, QK_RBT, EE_CLR, DB_TOGG, XXXXXXX, KC_ESC
#define ________________KEYB_CTRL_ROW_R________________ KC_ESC, XXXXXXX, DB_TOGG, EE_CLR, QK_RBT, QK_BOOT

/**
 * \brief NUM+ColemakDH+miryoku hold-n-tap thumbs (4 rows, 2x6 columns, 5+3 thumbs)
 *
 */
// clang-format off
#define COMMON_BASE_ROWS \
/* ╭────────────────────────────────────────────────────╮ ╭───────────────────────────────────────────────────────╮ */ \
    KC_F11,   KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,       KC_F6,   KC_F7,   KC_F8,   KC_F9,  KC_F10, LCK_SCRN, \
/* ├────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    KC_APP,   KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,        KC_J,    KC_L,    KC_U,    KC_Y,   KC_QUOT, KC_SLSH, \
/* ├────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    KC_PSCR,  KC_A,    KC_R,    KC_S,    KC_T,    KC_G,        KC_M,    KC_N,    KC_E,    KC_I,   KC_O,    KC_SCLN, \
/* ├────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    CW_TOGG,  KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,        KC_K,    KC_H,    KC_MINS, LA2_PTR,LA2_NAV, TGL_GM
/* ╰────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯ */
// clang-format on

/**
 * \brief Function layer.
 *
 * Secondary left-hand layer has function keys mirroring the numerals on the
 * primary layer with above F9 on the top row, plus system keys on the inner
 * column. App is on the tertiary thumb key and other thumb keys are duplicated
 * from the base layer to enable auto-repeat.
 */
// clang-format off
#define COMMON_FUNCTION_ROWS \
/* ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮ */ \
    KC_GRV,   KC_1,   KC_2,   KC_3,   KC_4,   KC_5,           KC_6,     KC_7,   KC_8,   KC_9,   KC_0, KC_EQL, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______, KC_F12,  KC_F7,  KC_F8,  KC_F9,KC_PSCR,                _________________DEAD_HALF_ROW_________________, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______, KC_F11,  KC_F4,  KC_F5,  KC_F6,KC_SCRL,                ________________HOME_ROW_GACS_R________________, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______, KC_F10,  KC_F1,  KC_F2,  KC_F3,KC_PAUS,                XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,_______
/* ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯ */
// clang-format on

/**
 * \brief Media layer.
 *
 * Tertiary left- and right-hand layer is media and RGB control.  This layer is
 * symmetrical to accommodate the left- and right-hand trackball.
 */
// clang-format off
#define COMMON_MEDIA_ROWS \
/* ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮ */ \
________________KEYB_CTRL_ROW_L________________,  ________________KEYB_CTRL_ROW_R________________, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
_________________DEAD_HALF_ROW_________________,  RM_VALU,RM_HUEU,RM_SATU,RM_PREV,RM_SPDU,RM_TOGG, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
________________HOME_ROW_GACS_L________________,  KC_MPRV,KC_VOLD,KC_MUTE,KC_VOLU,KC_MNXT,RGB_M_P, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
_______,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,XXXXXXX,  XXXXXXX,KC_BRID,XXXXXXX,KC_BRIU,XXXXXXX,XXXXXXX  \
/* ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯ */
// clang-format on

/** \brief Mouse layer featuring common editing keys with *all other right layers*.
 *
 * All mouse buttons & layer activator are located close and around the right trackball,
 * along with copy, paste, undo and redo keys, to facilitate one-hand editing & browsing.
 *
 * Right thumb keys are duplicated from the base layer to avoid having to change layer
 * mid edit just to press Enter or delete a char, and to enable auto-repeat.
 *
 * Without mouse emulation (useless with trackball).
 *
 * **Rational:** with the right-thumb occupied on the trackball, it's easier
 * placing the x3 *mouse buttons* on the primary keys (vs on the right thumb-cluster).
 * And to allow using trackball single-handedly (eg. when browsing),
 * the *ring finger* is chosen as layer activator, as it provides better versasility vs
 * pinning the "short" pinky, which would make the top-alphas row harder to reach and
 * anything on the 5th column unreachable (the original *charybdis 4x6 uses the pinky).
 */
// clang-format off
#define COMMON_POINTER_ROWS \
/* ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮ */ \
    ________________KEYB_CTRL_ROW_L________________,  ________________KEYB_CTRL_ROW_R________________, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _________________DEAD_HALF_ROW_________________,  _______,_______,MS_BTN3,MS_BTN5,DPI_MOD, KC_TAB, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    ________________HOME_ROW_GACS_L________________,  _______,_______,_______,MS_BTN4,S_D_MOD, KC_SPC, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______,XXXXXXX,XXXXXXX,SNP_TOG,DRG_TOG,XXXXXXX,   MS_BTN3, MS_BTN1, MS_BTN2,_______,DRGSCRL,_______ \
/* ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯ */
// clang-format on

/**
 * \brief Navigation layer featuring common editing keys with *all other right layers*.
 *
 * Primary right-hand layer activated by left home thumb & right pinky is for navigation and
 * editing. Cursor keys and line and page movement are on a "cross" centered around
 * home position, while clipboard, undos and other editing keys on prinary & thumbs
 * *mimic the pointer layer*.
 * Practically, with alternating right ring & pinky pinning, single-handed
 * editing & mouse is possible.
 */
// clang-format off
#define COMMON_NAVIGATION_ROWS \
/* ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮ */ \
    ________________KEYB_CTRL_ROW_L________________,  ________________KEYB_CTRL_ROW_R________________, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _________________DEAD_HALF_ROW_________________,  _______,_______, KC_PASTE,KC_HOME,KC_PGUP, KC_TAB, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    ________________HOME_ROW_GACS_L________________,  KC_LEFT, KC_DOWN, KC_UP, KC_RGHT,KC_PGDN, KC_SPC, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______,_______,_______,_______,XXXXXXX,_______,   _______,_______, KC_INS,LA3_PTR,_______,_______ \
/* ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯ */
// clang-format on

/**
 * \brief Numeral layout.
 *
 * Primary left-hand layer (right home thumb) is numerals and symbols. Numerals
 * are in the standard numpad locations with symbols in the remaining positions.
 *
 * Editing keys (copy-paste, undo) shared with all other right layers.
 * The `-,.` punctuations on the other side (right) are preserved from the base layer.
 */
// clang-format off
#define COMMON_NUMERAL_ROWS \
/* ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮ */ \
    ________________KEYB_CTRL_ROW_L________________,  ________________KEYB_CTRL_ROW_R________________, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______,KC_LBRC,   KC_7,   KC_8,   KC_9,KC_RBRC,  _______,_______, KC_EQL,KC_SLSH,KC_BSLS, KC_TAB, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______,KC_SCLN,   KC_4,   KC_5,   KC_6, KC_EQL,  _______,KC_LSFT,KC_LCTL,KC_LALT,KC_LGUI, KC_SPC, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______, KC_GRV,   KC_1,   KC_2,   KC_3,KC_BSLS,   _______,_______,KC_MINS,KC_COMM, KC_DOT,_______ \
/* ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯ */
// clang-format on

/**
 * \brief Symbols layer.
 *
 * The symbols layer is the biggest deviation from both miryoku and charyoku. We use both halves of the keyboard for it
 * and it no longer couples to the shifted values of the Numeral rows (As these are still easily accessible by just
 * pressing shift while in that layer.
 *
 * The main inspiration comes from the following awesome reddit comment by user ShelZuuz:
 * https://web.archive.org/web/20260227095613/https://old.reddit.com/r/ErgoMechKeyboards/comments/1cim5s9/miryoku_for_programming/l2adh8g/
 *     ` ~ # & |    ^ { } [ ]
 *     ! _ : = $    @ ( ) _ ;
 *     % ? * + \    / - < > "
 */
// clang-format off
#define COMMON_SYMBOLS_ROWS \
/* ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮ */ \
    ________________KEYB_CTRL_ROW_L________________,          ________________KEYB_CTRL_ROW_R________________, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______, KC_GRV, KC_TILD, KC_HASH, KC_AMPR, KC_PIPE,      KC_CIRC, KC_LCBR, KC_RCBR, KC_LBRC, KC_RBRC, _______,\
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______, KC_EXLM, KC_UNDS, KC_COLN, KC_EQL, KC_DLR,       KC_AT,   KC_LBRC, KC_RBRC, KC_UNDS, KC_SCLN, _______,\
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______, KC_PERC, KC_QUES, KC_ASTR, KC_PLUS, KC_BSLS,     KC_SLSH, KC_MINS, KC_LABK, KC_RABK, KC_QUOT, _______ \
/* ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯ */
// clang-format on

/**
 * \brief Gaming layer.
 *
 * Layer used for gaming, to not have to remap everything on every game every time. Everything is shifted one column to the right so WASD sits on the natural hand position.
 * Editing keys (copy-paste, undo) shared with all other right layers.
 * The `-,.` punctuations on the other side (right) are preserved from the base layer.
 */
// clang-format off
#define COMMON_GAMING_ROWS \
/* ╭──────────────────────────────────────────────────────╮ ╭──────────────────────────────────────────────────────╮ */ \
     KC_ESC,   KC_1,   KC_2,   KC_3,   KC_4,   KC_5,     KC_6,    KC_7,    KC_8,   KC_9,    KC_0,   LCK_SCRN,\
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______,   _______,KC_Q,   KC_W,   KC_E,   KC_R,     KC_T,    KC_Y,    KC_U,   KC_I,    KC_O,   KC_P,    \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______,   KC_LSFT,KC_A,   KC_S,   KC_D,   KC_F,     KC_G,    KC_H,    KC_J,   KC_K,    KC_L,   KC_COLN, \
/* ├──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────┤ */ \
    _______,   KC_LCTL,KC_Z,   KC_X,   KC_C,   KC_V,     MS_BTN3, MS_BTN1, MS_BTN2,_______, DRGSCRL,TGL_GM  \
/* ╰──────────────────────────────────────────────────────┤ ├──────────────────────────────────────────────────────╯ */
// clang-format on

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
// clang-format off
#define _HOME_ROW_MOD_GACS(                                                     \
    L00, L01, L02, L03, L04, L05, R06, R07, R08, R09, R10, R11,                 \
    L12, L13, L14, L15, L16, L17, R18, R19, R20, R21, R22, R23,                 \
    L24, L25, L26, L27, L28, L29, R30, R31, R32, R33, R34, R35,                 \
    ...)                                                                        \
          L00,         L01,         L02,         L03,         L04,         L05, \
          R06,         R07,         R08,         R09,         R10,         R11, \
          L12,         L13,         L14,         L15,         L16,         L17, \
          R18,         R19,         R20,         R21,         R22,         R23, \
          L24, LGUI_T(L25), LALT_T(L26), LCTL_T(L27), LSFT_T(L28),         L29, \
          R30, RSFT_T(R31), RCTL_T(R32), LALT_T(R33), LGUI_T(R34),         R35, \
    __VA_ARGS__
// clang-format on

#define HOME_ROW_MOD_GACS(...) _HOME_ROW_MOD_GACS(__VA_ARGS__)
#define LAYOUT_wrapper(...) LAYOUT(__VA_ARGS__)

// Combos
#ifdef COMBO_ENABLE
const uint16_t PROGMEM combo_layer_lock_med[] = {ESC_MED, CW_TOGG, COMBO_END};
const uint16_t PROGMEM combo_layer_lock_nav[] = {SPC_NAV, CW_TOGG, COMBO_END};
const uint16_t PROGMEM combo_layer_lock_ptr[] = {TAB_PTR, CW_TOGG, COMBO_END};
const uint16_t PROGMEM combo_layer_lock_fun[] = {DEL_FUN, KC_CAPS, COMBO_END};
const uint16_t PROGMEM combo_layer_lock_num[] = {BSP_NUM, KC_CAPS, COMBO_END};
const uint16_t PROGMEM combo_layer_lock_sym[] = {ENT_SYM, KC_CAPS, COMBO_END};

combo_t key_combos[] = {
    COMBO(combo_layer_lock_med, TG(LAYER_MEDIA)), COMBO(combo_layer_lock_nav, TG(LAYER_NAVIGATION)), COMBO(combo_layer_lock_ptr, TG(LAYER_POINTER)), COMBO(combo_layer_lock_fun, TG(LAYER_FUNCTION)), COMBO(combo_layer_lock_num, TG(LAYER_NUMERAL)), COMBO(combo_layer_lock_sym, TG(LAYER_SYMBOLS)),
};
#endif // COMBO_ENABLE

// Caps word function
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

void keyboard_post_init_user(void) {
    // Customise these values to desired behaviour
    debug_enable = true;
    // debug_matrix=true;
    // debug_keyboard=true;
    debug_mouse = true;
}
