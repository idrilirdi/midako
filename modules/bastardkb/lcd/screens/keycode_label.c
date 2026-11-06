#include QMK_KEYBOARD_H
#include <stdio.h>
#include <string.h>
#include "lvgl.h"
#include "keycode_label.h"

__attribute__((weak)) const char *keycode_label_user(uint16_t keycode) {
    return NULL;
}

__attribute__((weak)) const char *lcd_layer_name(uint8_t layer) {
    switch (layer) {
        case 0:
            return "BASE";
        case 1:
            return "NAV";
        case 2:
            return "POINTER";
        case 3:
            return "NUMERAL";
        case 4:
            return "SYMBOLS";
        case 5:
            return "FUNCTION";
        case 6:
            return "MEDIA";
        case 7:
            return "GAMING";
        default:
            return "LAYER";
    }
}

// Labels for KC_A (0x04) .. KC_SLSH (0x38), unshifted and shifted.
static const char *const basic_labels[] = {
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
    "1", "2", "3", "4", "5", "6", "7", "8", "9", "0",
    LV_SYMBOL_NEW_LINE, "ESC", LV_SYMBOL_BACKSPACE, "TAB", "SPC",
    "-", "=", "[", "]", "\\", "#", ";", "'", "`", ",", ".", "/",
};
static const char *const shifted_labels[] = {
    "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",
    "!", "@", "#", "$", "%", "^", "&", "*", "(", ")",
    LV_SYMBOL_NEW_LINE, "ESC", LV_SYMBOL_BACKSPACE, "TAB", "SPC",
    "_", "+", "{", "}", "|", "~", ":", "\"", "~", "<", ">", "?",
};

static const char *basic_label(uint8_t kc, bool shifted) {
    if (kc >= KC_A && kc <= KC_SLASH) {
        return shifted ? shifted_labels[kc - KC_A] : basic_labels[kc - KC_A];
    }
    if (kc >= KC_F1 && kc <= KC_F12) {
        static const char *const f[] = {"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12"};
        return f[kc - KC_F1];
    }
    if (kc >= KC_KP_1 && kc <= KC_KP_0) {
        static const char *const kp[] = {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0"};
        return kp[kc - KC_KP_1];
    }
    switch (kc) {
        case KC_NO:
            return " ";
        case KC_CAPS_LOCK:
            return "CAP";
        case KC_PRINT_SCREEN:
            return "PRT";
        case KC_SCROLL_LOCK:
            return "SCR";
        case KC_PAUSE:
            return "PAU";
        case KC_INSERT:
            return "INS";
        case KC_HOME:
            return "HOM";
        case KC_PAGE_UP:
            return "PG" LV_SYMBOL_UP;
        case KC_DELETE:
            return "DEL";
        case KC_END:
            return "END";
        case KC_PAGE_DOWN:
            return "PG" LV_SYMBOL_DOWN;
        case KC_RIGHT:
            return LV_SYMBOL_RIGHT;
        case KC_LEFT:
            return LV_SYMBOL_LEFT;
        case KC_DOWN:
            return LV_SYMBOL_DOWN;
        case KC_UP:
            return LV_SYMBOL_UP;
        case KC_NUM_LOCK:
            return "NUM";
        case KC_KP_SLASH:
            return "/";
        case KC_KP_ASTERISK:
            return "*";
        case KC_KP_MINUS:
            return "-";
        case KC_KP_PLUS:
            return "+";
        case KC_KP_ENTER:
            return LV_SYMBOL_NEW_LINE;
        case KC_KP_DOT:
            return ".";
        case KC_KP_EQUAL:
            return "=";
        case KC_NONUS_BACKSLASH:
            return "\\";
        case KC_APPLICATION:
            return "APP";
        case KC_UNDO:
            return "UND";
        case KC_AGAIN:
            return "RDO";
        case KC_CUT:
            return LV_SYMBOL_CUT;
        case KC_COPY:
            return LV_SYMBOL_COPY;
        case KC_PASTE:
            return LV_SYMBOL_PASTE;
        case KC_AUDIO_MUTE:
            return LV_SYMBOL_MUTE;
        case KC_AUDIO_VOL_UP:
            return LV_SYMBOL_VOLUME_MAX;
        case KC_AUDIO_VOL_DOWN:
            return LV_SYMBOL_VOLUME_MID;
        case KC_MEDIA_NEXT_TRACK:
            return LV_SYMBOL_NEXT;
        case KC_MEDIA_PREV_TRACK:
            return LV_SYMBOL_PREV;
        case KC_MEDIA_STOP:
            return LV_SYMBOL_STOP;
        case KC_MEDIA_PLAY_PAUSE:
            return LV_SYMBOL_PLAY;
        case KC_BRIGHTNESS_UP:
            return "BR+";
        case KC_BRIGHTNESS_DOWN:
            return "BR-";
        case MS_BTN1:
            return "M1";
        case MS_BTN2:
            return "M2";
        case MS_BTN3:
            return "M3";
        case MS_BTN4:
            return "M4";
        case MS_BTN5:
            return "M5";
        case MS_WHLU:
            return "W" LV_SYMBOL_UP;
        case MS_WHLD:
            return "W" LV_SYMBOL_DOWN;
        case MS_WHLL:
            return "W" LV_SYMBOL_LEFT;
        case MS_WHLR:
            return "W" LV_SYMBOL_RIGHT;
        case KC_LEFT_CTRL:
        case KC_RIGHT_CTRL:
            return "CTL";
        case KC_LEFT_SHIFT:
        case KC_RIGHT_SHIFT:
            return "SFT";
        case KC_LEFT_ALT:
            return "ALT";
        case KC_RIGHT_ALT:
            return "AGR";
        case KC_LEFT_GUI:
        case KC_RIGHT_GUI:
            return "GUI";
        default:
            return NULL;
    }
}

static const char *special_label(uint16_t kc) {
    switch (kc) {
        case KC_TRANSPARENT:
            return " ";
        case QK_BOOT:
            return "BTL";
        case QK_REBOOT:
            return "RBT";
        case QK_CLEAR_EEPROM:
            return "EEC";
        case DB_TOGG:
            return "DBG";
        case CW_TOGG:
            return "CW";
        case RM_TOGG:
            return "RGB";
        case RM_NEXT:
            return "RG>";
        case RM_PREV:
            return "RG<";
        case RM_VALU:
            return "VA+";
        case RM_VALD:
            return "VA-";
        case RM_HUEU:
            return "HU+";
        case RM_HUED:
            return "HU-";
        case RM_SATU:
            return "SA+";
        case RM_SATD:
            return "SA-";
        case RM_SPDU:
            return "SP+";
        case RM_SPDD:
            return "SP-";
        case RGB_M_P:
            return "RGP";
#if defined(POINTING_DEVICE_ENABLE) && !defined(NO_DILEMMA_KEYCODES)
        case POINTER_DEFAULT_DPI_FORWARD:
            return "DP+";
        case POINTER_DEFAULT_DPI_REVERSE:
            return "DP-";
        case POINTER_SNIPING_DPI_FORWARD:
            return "SD+";
        case POINTER_SNIPING_DPI_REVERSE:
            return "SD-";
        case SNIPING_MODE:
            return "SNP";
        case SNIPING_MODE_TOGGLE:
            return "SNT";
        case DRAGSCROLL_MODE:
            return "DRG";
        case DRAGSCROLL_MODE_TOGGLE:
            return "DRT";
#endif
        case LCD_MODULE_CHANGE_THEME:
            return "THM";
        case LCD_MODULE_MENU:
            return "MNU";
        case LCD_MENU_NEXT:
            return LV_SYMBOL_DOWN;
        case LCD_MENU_PREV:
            return LV_SYMBOL_UP;
        default:
            return NULL;
    }
}

// Layer keys show the first three letters of the target layer name, e.g. TG(GAMING) -> "GAM".
// Wider labels do not fit a key cell in montserrat 14.
static void layer_label(uint8_t layer, char *buf, size_t size) {
    snprintf(buf, size, "%.3s", lcd_layer_name(layer));
}

void keycode_label(uint16_t kc, char *buf, size_t size) {
    const char *s = keycode_label_user(kc);
    if (!s) s = special_label(kc);
    if (s) {
        snprintf(buf, size, "%s", s);
        return;
    }

    if (IS_QK_MOD_TAP(kc)) {
        kc = QK_MOD_TAP_GET_TAP_KEYCODE(kc);
    } else if (IS_QK_LAYER_TAP(kc)) {
        kc = QK_LAYER_TAP_GET_TAP_KEYCODE(kc);
    } else if (IS_QK_TOGGLE_LAYER(kc)) {
        layer_label(QK_TOGGLE_LAYER_GET_LAYER(kc), buf, size);
        return;
    } else if (IS_QK_MOMENTARY(kc)) {
        layer_label(QK_MOMENTARY_GET_LAYER(kc), buf, size);
        return;
    } else if (IS_QK_TO(kc)) {
        layer_label(QK_TO_GET_LAYER(kc), buf, size);
        return;
    } else if (IS_QK_ONE_SHOT_LAYER(kc)) {
        layer_label(QK_ONE_SHOT_LAYER_GET_LAYER(kc), buf, size);
        return;
    } else if (IS_QK_DEF_LAYER(kc)) {
        layer_label(QK_DEF_LAYER_GET_LAYER(kc), buf, size);
        return;
    }

    if (IS_QK_BASIC(kc)) {
        s = basic_label(kc, false);
        snprintf(buf, size, "%s", s ? s : "?");
        return;
    }

    if (IS_QK_MODS(kc)) {
        uint8_t mods  = QK_MODS_GET_MODS(kc) & 0x0F; // left/right does not matter for display
        uint8_t basic = QK_MODS_GET_BASIC_KEYCODE(kc);
        if (mods == MOD_LSFT) {
            s = basic_label(basic, true);
            snprintf(buf, size, "%s", s ? s : "?");
            return;
        }
        char prefix[5] = {0};
        uint8_t n      = 0;
        if (mods & MOD_LCTL) prefix[n++] = 'C';
        if (mods & MOD_LALT) prefix[n++] = 'A';
        if (mods & MOD_LGUI) prefix[n++] = 'G';
        if (mods & MOD_LSFT) prefix[n++] = 'S';
        s = basic_label(basic, false);
        snprintf(buf, size, "%s%s", prefix, s ? s : "?");
        return;
    }

    snprintf(buf, size, "?");
}
