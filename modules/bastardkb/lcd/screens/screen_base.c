#include QMK_KEYBOARD_H
#include "layer_map.h"
#include "introspection.h"
#include "lvgl.h"
#include <ctype.h>

#include "config.h"
#include "screen_base.h"
#include "screen_pomodoro.h"
#include "lcd.h"
#include "menu.h"
#include "ui_elements.h"
#include "transactions.h"
#include "dilemma_sync.h"

LV_FONT_DECLARE(jetbrainsmono13);

typedef struct {
    lv_obj_t *obj;
    void (*update_function)(lv_obj_t *, dilemma_status_t current_status, dilemma_status_t prev_status);
} obj_update_dilemma_lcd_status_t;

dilemma_status_t dilemma_lcd_status_prev = {0};
dilemma_status_t dilemma_lcd_status      = {0};

static void update_layer_name(lv_obj_t *obj, const dilemma_status_t current_status, const dilemma_status_t prev_status);
static void update_mod_shift(lv_obj_t *obj, const dilemma_status_t current_status, const dilemma_status_t prev_status);
static void update_mod_ctrl(lv_obj_t *obj, const dilemma_status_t current_status, const dilemma_status_t prev_status);
static void update_mod_alt(lv_obj_t *obj, const dilemma_status_t current_status, const dilemma_status_t prev_status);
static void update_mod_gui(lv_obj_t *obj, const dilemma_status_t current_status, const dilemma_status_t prev_status);
static void update_keymap_display(lv_obj_t *obj, const dilemma_status_t current_status, const dilemma_status_t prev_status);
static void update_mod_xx(lv_obj_t *obj, uint8_t mod_mask, const dilemma_status_t current_status, const dilemma_status_t prev_status);
static void menu_base_go_pomodoro(void);
static void menu_base_change_theme(void);
static void load_screen_base_menu(void);
static void load_screen_base_base(void);
static void refresh_screen_base(void);

static lv_obj_t *ui_screen_base;
static lv_obj_t *ui_screen_base_menu;

static obj_update_dilemma_lcd_status_t widgets[6];
static obj_update_dilemma_menu_t       menus[3];
static uint8_t                         screen_index = 0;
static uint8_t                         menu_index   = 0;

void load_module_base(void) {
    load_screen_base_base();
}

void init_screen_base(void) {
    ui_screen_base = lv_obj_create(NULL);
    lv_obj_t *cont = ui_create_container(ui_screen_base);

    ui_screen_base_menu = lv_obj_create(NULL);
    lv_obj_t *cont_menu = ui_create_container(ui_screen_base_menu);

    /* ----- Widgets ----- */
    widgets[0] = (obj_update_dilemma_lcd_status_t){
        ui_create_layer_label(cont),
        &update_layer_name,
    };
    widgets[1] = (obj_update_dilemma_lcd_status_t){
        ui_create_mod_button(cont, "SHFT", true, MOD_MASK_SHIFT),
        &update_mod_shift,
    };
    widgets[2] = (obj_update_dilemma_lcd_status_t){
        ui_create_mod_button(cont, "CTRL", false, MOD_MASK_CTRL),
        &update_mod_ctrl,
    };
    widgets[3] = (obj_update_dilemma_lcd_status_t){
        ui_create_mod_button(cont, "ALT", false, MOD_MASK_ALT),
        &update_mod_alt,
    };
    widgets[4] = (obj_update_dilemma_lcd_status_t){
        ui_create_mod_button(cont, "META", false, MOD_MASK_GUI),
        &update_mod_gui,
    };

    ui_create_line_separator(cont, 1, 3);

    // Create the text label for the Keymap
    lv_obj_t *keymap_label = lv_label_create(cont);

    lv_obj_set_width(keymap_label, LV_PCT(100)); // Prevents separator bar from shrinking
    lv_label_set_text(keymap_label, "Loading...");

    ui_styles_t *styles = get_current_ui_styles();
    lv_obj_add_style(keymap_label, &styles->value_labels, 0);
    lv_obj_set_style_text_font(keymap_label, &jetbrainsmono13, 0); // Monospace font for perfect grid
    lv_obj_set_style_text_color(keymap_label, lv_color_hex(0xFFFFFF), 0);

    widgets[5] = (obj_update_dilemma_lcd_status_t){keymap_label, &update_keymap_display};

    /* ----- menus ----- */
    menus[0] = (obj_update_dilemma_menu_t){
        ui_create_menu_line(cont_menu, "Back to main"),
        NULL,
    };
    menus[1] = (obj_update_dilemma_menu_t){
        ui_create_menu_line(cont_menu, "Pomodoro"),
        &menu_base_go_pomodoro,
    };
    // TODO later, create a "theme options" module? but then at each bootmagic flash it will be erased :(
    menus[2] = (obj_update_dilemma_menu_t){
        ui_create_menu_line(cont_menu, "Change theme"),
        &menu_base_change_theme,
    };
}

void housekeeping_task_screen_base(void) {
    dilemma_lcd_status = get_dilemma_status();
    refresh_screen_base();
    dilemma_lcd_status_prev = dilemma_lcd_status;
}

static void menu_base_go_pomodoro(void) {
    set_current_module(MODULE_POMODORO);
}

static void menu_base_change_theme(void) {
    cycle_theme_and_save_in_eeprom();
    // TODO remove this? should only be called by left anyway
    // if (is_keyboard_left()) {
    update_styles_from_current_theme();
    // }
}

static void update_layer_name(lv_obj_t *obj, const dilemma_status_t current_status, const dilemma_status_t prev_status) {
    if (current_status.layer != prev_status.layer) {
        switch (current_status.layer) {
            case 0:
            default:
                lv_label_set_text(obj, "LAYER: BASE");
                break;
            case 1:
                lv_label_set_text(obj, "LAYER: NAV");
                break;
            case 2:
                lv_label_set_text(obj, "LAYER: POINTER");
                break;
            case 3:
                lv_label_set_text(obj, "LAYER: NUMERAL");
                break;
            case 4:
                lv_label_set_text(obj, "LAYER: SYMBOLS");
                break;
            case 5:
                lv_label_set_text(obj, "LAYER: FUNCTION");
                break;
            case 6:
                lv_label_set_text(obj, "LAYER: MEDIA");
                break;
            case 7:
                lv_label_set_text(obj, "LAYER: GAMING");
                break;
        }
    }
}

static void update_mod_xx(lv_obj_t *obj, uint8_t mod_mask, const dilemma_status_t current_status, const dilemma_status_t prev_status) {
    if ((current_status.mods & mod_mask) != (prev_status.mods & mod_mask)) {
        if ((current_status.mods & mod_mask)) {
            lv_event_send(obj, LV_EVENT_PRESSED, NULL);
        } else {
            lv_event_send(obj, LV_EVENT_RELEASED, NULL);
        }
    }
}

static void update_mod_shift(lv_obj_t *obj, const dilemma_status_t current_status, const dilemma_status_t prev_status) {
    update_mod_xx(obj, MOD_MASK_SHIFT, current_status, prev_status);
}
static void update_mod_ctrl(lv_obj_t *obj, const dilemma_status_t current_status, const dilemma_status_t prev_status) {
    update_mod_xx(obj, MOD_MASK_CTRL, current_status, prev_status);
}
static void update_mod_alt(lv_obj_t *obj, const dilemma_status_t current_status, const dilemma_status_t prev_status) {
    update_mod_xx(obj, MOD_MASK_ALT, current_status, prev_status);
}
static void update_mod_gui(lv_obj_t *obj, const dilemma_status_t current_status, const dilemma_status_t prev_status) {
    update_mod_xx(obj, MOD_MASK_GUI, current_status, prev_status);
}

// Safely extract readable names without relying on conflicting external modules
static const char *stringify_keycode(uint16_t kc) {
    if (kc == KC_NO || kc == 0) return "    "; // 4 blank spaces for gaps
    if (kc == KC_TRANSPARENT) return "TRNS";

    // Handle Layer Toggles (TG, TO, MO)
    if ((kc & 0xFF00) == 0x5000) return "TG";
    if ((kc & 0xFF00) == 0x5100) return "TO";
    if ((kc & 0xFF00) == 0x5200) return "MO";

    // Strip Mod-Taps (0x2000) and Layer-Taps (0x4000) to get the basic keycode
    if (kc > 0x00FF) {
        uint16_t basic_kc = kc & 0xFF;
        if (basic_kc > 0) kc = basic_kc;
    }

    // Direct lookup for all standard keys (Matches your common_keymap.h comments)
    static const char *const basic_keys[] = {"    ", "    ", "    ", "    ", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "ENT", "ESC", "BSPC", "TAB", "SPC", "MINS", "EQL", "LBRC", "RBRC", "BSLS", "NUHS", "SCLN", "QUOT", "GRV", "COMM", "DOT", "SLSH", "CAPS", "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "PSCR", "SCRL", "PAUS", "INS", "HOME", "PGUP", "DEL", "END", "PGDN", "RGHT", "LEFT", "DOWN", "UP", "NUM", "PSLS", "PAST", "PMNS", "PPLS", "PENT", "P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8", "P9", "P0", "PDOT", "NUBS", "APP"};

    if (kc < sizeof(basic_keys) / sizeof(basic_keys[0])) return basic_keys[kc];

    // Explicit overrides for modifiers and media
    switch (kc) {
        case KC_LCTL:
            return "LCTL";
        case KC_LSFT:
            return "LSFT";
        case KC_LALT:
            return "LALT";
        case KC_LGUI:
            return "LGUI";
        case KC_RCTL:
            return "RCTL";
        case KC_RSFT:
            return "RSFT";
        case KC_RALT:
            return "RALT";
        case KC_RGUI:
            return "RGUI";
        case KC_MPLY:
            return "PLAY";
        case KC_MSTP:
            return "STOP";
        case KC_MPRV:
            return "PREV";
        case KC_MNXT:
            return "NEXT";
        case KC_MUTE:
            return "MUTE";
        case KC_VOLU:
            return "VOLU";
        case KC_VOLD:
            return "VOLD";
    }

    return " ???";
}

// Declare Drashna's internal function so we can force it to run on the slave half
extern void populate_layer_map(void);

static void update_keymap_display(lv_obj_t *obj, const dilemma_status_t current_status, const dilemma_status_t prev_status) {
    bool is_initial_load = (strcmp(lv_label_get_text(obj), "Loading...") == 0);

    // If the master synced a layer change over TRRS, or we just booted
    if (current_status.layer != prev_status.layer || is_initial_load) {
        if (!is_keyboard_master()) {
            // FIX: Drashna's module refuses to run this on the slave side.
            // We bypass his check and force it to populate the array right now!
            populate_layer_map();
            set_layer_map_has_updated(true);
        } else {
            // On master, just mark it dirty and let the background task handle it normally
            set_layer_map_dirty();
        }
    }

    // Only draw if the map was just populated
    if (get_layer_map_has_updated() || is_initial_load) {
        // Safety abort: wait for background task to fill the array if on master
        if (layer_map[0][0] == 0 && layer_map[0][1] == 0 && is_keyboard_master()) {
            return;
        }

        char km_str[512] = "";

        for (int row = 0; row < LAYER_MAP_ROWS; row++) {
            for (int col = 0; col < LAYER_MAP_COLS; col++) {
                uint16_t kc = layer_map[row][col];

                char key_str[10];

                // SPACING FIX: Removed the trailing space.
                // "%-4.4s" means every column will take EXACTLY 4 characters of space.
                snprintf(key_str, sizeof(key_str), "%-4.4s", stringify_keycode(kc));

                strcat(km_str, key_str);
            }
            strcat(km_str, "\n");
        }

        lv_label_set_text(obj, km_str);

        // Reset the flag so we don't waste CPU cycles drawing the exact same grid again
        set_layer_map_has_updated(false);
    }
}

static void load_screen_base_base(void) {
    load_screen_xx_base(menus, sizeof(menus) / sizeof(obj_update_dilemma_menu_t), ui_screen_base);
}

static void load_screen_base_menu(void) {
    load_screen_xx_menu(menus, sizeof(menus) / sizeof(obj_update_dilemma_menu_t), ui_screen_base_menu);
}

void refresh_screen_base(void) {
    const dilemma_status_t current_status = (const dilemma_status_t)dilemma_lcd_status;
    const dilemma_status_t prev_status    = (const dilemma_status_t)dilemma_lcd_status_prev;
    static int             last_layer;
    int                    current_layer = get_highest_layer(layer_state);

    if (current_layer != last_layer) {
        switch (current_layer) {
            case 0:
                // default:
                // we only trigger things if we come back from the menu layer
                if (screen_index == 1) {
                    load_screen_base_base();
                    trigger_menu_element(menus, menu_index);
                    menu_index   = 0;
                    screen_index = 0;
                }
                break;
            case LAYER_MENU:
                if (screen_index == 0) {
                    load_screen_base_menu();
                    screen_index = 1;
                }
                break;
            default:
                break;
        }
    }

    if (is_keyboard_left()) {
        for (int i = 0; i < sizeof(widgets) / sizeof(obj_update_dilemma_lcd_status_t); i++) {
            lv_obj_t *obj = widgets[i].obj;
            if (obj && widgets[i].update_function) widgets[i].update_function(obj, current_status, prev_status);
        }
    }

    last_layer = current_layer;
}

bool process_record_screen_base(uint16_t keycode, keyrecord_t *record) {
    // TODO index is hardcoded...
    if (screen_index == 1) {
        process_record_menu(keycode, record, menus, &menu_index, sizeof(menus) / sizeof(obj_update_dilemma_menu_t));
    }
    return true;
}
