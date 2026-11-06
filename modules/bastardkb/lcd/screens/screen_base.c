#include QMK_KEYBOARD_H
#include "layer_map.h"
#include "introspection.h"
#include "lvgl.h"
#include <stdio.h>

#include "config.h"
#include "screen_base.h"
#include "screen_pomodoro.h"
#include "lcd.h"
#include "menu.h"
#include "ui_elements.h"
#include "dilemma_sync.h"
#include "keycode_label.h"
#include "bongocat_img.h"

/*
 * Base screen, shown on the left half's LCD.
 *
 *   ┌──────────────────────────┐
 *   │ LAYER NAME      S C A G  │  header: layer name + modifier pills
 *   ├──────────────────────────┤
 *   │ typing view  (on the default/base layer, whichever it is):
 *   │    bongo cat, WPM, avg/max/keys, WPM bar
 *   │ layer view   (any other layer):
 *   │    left half key map on top, right half below
 *   └──────────────────────────┘
 *
 * Both views live on the same LVGL screen and are toggled with LV_OBJ_FLAG_HIDDEN.
 * The menu (LAYER_MENU) is a separate screen, as before.
 */

#ifndef LCD_WPM_BAR_MAX
#    define LCD_WPM_BAR_MAX 150
#endif
// Bongo cat timing: how long a paw stays down after a key press, and how long the
// "ready" (paws up) pose is held after the last key press before idling.
#ifndef BONGOCAT_TAP_HOLD_MS
#    define BONGOCAT_TAP_HOLD_MS 110
#endif
#ifndef BONGOCAT_PREP_HOLD_MS
#    define BONGOCAT_PREP_HOLD_MS 1200
#endif
#ifndef BONGOCAT_IDLE_FRAME_MS
#    define BONGOCAT_IDLE_FRAME_MS 300
#endif
// How often the WPM number, stats line and bar are redrawn. Each LVGL refresh blocks
// this half's key scan, so these are not updated on every key press.
#ifndef LCD_STATS_UPDATE_MS
#    define LCD_STATS_UPDATE_MS 500
#endif
// Seconds after the last key press during which WPM samples count toward the average.
#ifndef LCD_WPM_AVG_ACTIVE_MS
#    define LCD_WPM_AVG_ACTIVE_MS 3000
#endif

#define HALF_ROWS 5
#define HALF_COLS 6
#define HALF_KEYS (HALF_ROWS * HALF_COLS)
// Each half is drawn as two button matrices: the 4 main rows, and the thumb row.
#define MAIN_ROWS (HALF_ROWS - 1)
#define MAIN_KEYS (MAIN_ROWS * HALF_COLS)
// buttons + one "\n" per row break + terminating ""
#define MAIN_MAP_LEN (MAIN_KEYS + MAIN_ROWS)
#define THUMB_MAP_LEN (HALF_COLS + 1)
// Divider between the halves, and between each half's main block and thumb row.
#define HALF_DIVIDER_HEIGHT 2
#define THUMB_DIVIDER_HEIGHT 1
#define LAYER_VIEW_GAP 3
// Width of one half's key block. The rest of the row is the offset that pushes the
// left half left and the right half right.
#define HALF_BLOCK_WIDTH 204
// Index-finger home keys (T and N on Colemak-DH), as main-matrix button ids:
// home row (row 2), column 4 on the left half and column 1 on the right half.
#define HOMING_KEY_LEFT (2 * HALF_COLS + 4)
#define HOMING_KEY_RIGHT (2 * HALF_COLS + 1)
#define HEADER_HEIGHT 28
#define LAYER_PILL_WIDTH 86
#define MOD_PILLS_SHIFT_LEFT 3
#define SCREEN_GAP 1
#define VIEW_HEIGHT (LCD_HEIGHT - LCD_SAFE_PAD_TOP - LCD_SAFE_PAD_BOTTOM - HEADER_HEIGHT - SCREEN_GAP)

_Static_assert(LAYER_MAP_ROWS == 2 * HALF_ROWS && LAYER_MAP_COLS == HALF_COLS,
               "The LCD layer view expects LAYER_MAP_ROWS 10 / LAYER_MAP_COLS 6: rows 0-4 left half, rows 5-9 right half, in visual order");

LV_FONT_DECLARE(jetbrainsmonoextrabold60);

// Not exported by the layer_map module header, but not static either.
extern void populate_layer_map(void);

static void menu_base_go_pomodoro(void);
static void menu_base_change_theme(void);
static void load_screen_base_menu(void);
static void load_screen_base_base(void);
static void refresh_screen_base(void);

static lv_obj_t *ui_screen_base;
static lv_obj_t *ui_screen_base_menu;

static obj_update_dilemma_menu_t menus[3];
static uint8_t                   screen_index = 0;
static uint8_t                   menu_index   = 0;

static dilemma_status_t dilemma_lcd_status_prev = {0};
static dilemma_status_t dilemma_lcd_status      = {0};

/* ----- header ----- */
static lv_obj_t *layer_label;
static lv_obj_t *mod_pills[4];
static const uint8_t mod_masks[4] = {MOD_MASK_SHIFT, MOD_MASK_CTRL, MOD_MASK_ALT, MOD_MASK_GUI};

/* ----- typing view ----- */
static lv_obj_t *typing_view;
static lv_obj_t *bongo_img;
static lv_obj_t *wpm_label;
static lv_obj_t *stats_label;
static lv_obj_t *wpm_bar;
#ifdef LCD_DEBUG_RENDER_TIME
static lv_obj_t *render_label;
#endif

/* ----- layer view ----- */
static lv_obj_t   *layer_view;
static lv_obj_t   *keymap_main[2];
static lv_obj_t   *keymap_thumb[2];
static char        key_labels[2][HALF_KEYS][16];
static const char *main_maps[2][MAIN_MAP_LEN];
static const char *thumb_maps[2][THUMB_MAP_LEN];
static uint32_t    pressed_bits[2];
static uint32_t    disabled_bits[2];

// Thumb row slots that hold no key: the two outer slots of the left half, the two
// outer slots of the right half. Must match layer_remap in the keymap.
static bool is_empty_slot(uint8_t half, uint8_t row, uint8_t col) {
    if (row != HALF_ROWS - 1) return false;
    return half == 0 ? col < 2 : col >= 4;
}

// Key index i = row * HALF_COLS + col within a half -> its button matrix and button id.
static void set_key_ctrl(uint8_t half, uint8_t i, lv_btnmatrix_ctrl_t ctrl, bool on) {
    lv_obj_t *m  = i < MAIN_KEYS ? keymap_main[half] : keymap_thumb[half];
    uint16_t  id = i < MAIN_KEYS ? i : i - MAIN_KEYS;
    if (on) {
        lv_btnmatrix_set_btn_ctrl(m, id, ctrl);
    } else {
        lv_btnmatrix_clear_btn_ctrl(m, id, ctrl);
    }
}

/* ----- typing stats & bongo cat state (left half only) ----- */
static uint32_t last_press_time = 0;
static uint32_t keys_pressed    = 0;
static uint16_t wpm_max         = 0;
static uint32_t wpm_sum         = 0;
static uint32_t wpm_samples     = 0;
static uint8_t  tap_frame       = 0;
static bool     tap_pending     = false;

/* ======================================================================== */

static lv_color_t theme_value_color(void) {
    lv_style_value_t v;
    if (lv_style_get_prop(&get_current_ui_styles()->value_labels, LV_STYLE_TEXT_COLOR, &v) == LV_RES_OK) {
        return v.color;
    }
    return lv_color_white();
}

static lv_obj_t *create_view(lv_obj_t *parent) {
    lv_obj_t *view = lv_obj_create(parent);
    lv_obj_remove_style_all(view);
    lv_obj_set_size(view, LV_PCT(100), VIEW_HEIGHT);
    lv_obj_add_flag(view, LV_OBJ_FLAG_FLEX_IN_NEW_TRACK);
    lv_obj_clear_flag(view, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_flex_flow(view, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(view, LV_FLEX_ALIGN_SPACE_EVENLY, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    return view;
}

// Radius of the outer top corner of the outermost header pills, so they follow the
// rounded corners of the case opening over the screen instead of being cut by it.
#ifndef LCD_HEADER_OUTER_RADIUS
#    define LCD_HEADER_OUTER_RADIUS 16
#endif

/*
 * LVGL rounds all four corners equally, so the outer corner is clipped with a radius
 * mask over an area grown away from that corner: only the one corner gets the big
 * radius. The mask covers the object's own background/border drawing (MAIN); children
 * (the centered label) are drawn afterwards and are not affected.
 * User data: 0 = round the top-left corner, 1 = top-right.
 */
static void outer_corner_draw_cb(lv_event_t *e) {
    static lv_draw_mask_radius_param_t param;
    static int16_t                     mask_id = LV_MASK_ID_INV;

    if (lv_event_get_code(e) == LV_EVENT_DRAW_MAIN_BEGIN) {
        lv_obj_t *obj = lv_event_get_target(e);
        lv_area_t a;
        lv_obj_get_coords(obj, &a);
        const lv_coord_t r = LCD_HEADER_OUTER_RADIUS;
        if ((uintptr_t)lv_event_get_user_data(e) == 0) {
            a.x2 += r; // keep only the top-left corner of the mask inside the object
        } else {
            a.x1 -= r; // keep only the top-right corner
        }
        a.y2 += r;
        lv_draw_mask_radius_init(&param, &a, r, false);
        mask_id = lv_draw_mask_add(&param, NULL);
    } else if (mask_id != LV_MASK_ID_INV) {
        lv_draw_mask_free_param(&param);
        lv_draw_mask_remove_id(mask_id);
        mask_id = LV_MASK_ID_INV;
    }
}

static void round_outer_top_corner(lv_obj_t *obj, bool right) {
    void *side = (void *)(uintptr_t)(right ? 1 : 0);
    lv_obj_add_event_cb(obj, outer_corner_draw_cb, LV_EVENT_DRAW_MAIN_BEGIN, side);
    lv_obj_add_event_cb(obj, outer_corner_draw_cb, LV_EVENT_DRAW_MAIN_END, side);
}

// Invisible flex item that takes an equal share of the free space in the header row.
static lv_obj_t *header_spacer(lv_obj_t *cont) {
    lv_obj_t *spacer = lv_obj_create(cont);
    lv_obj_remove_style_all(spacer);
    lv_obj_set_size(spacer, 0, HEADER_HEIGHT);
    lv_obj_set_flex_grow(spacer, 1);
    return spacer;
}

static void create_header(lv_obj_t *cont) {
    header_spacer(cont);
    layer_label           = ui_create_layer_label(cont);
    lv_obj_t *layer_box   = lv_obj_get_parent(layer_label);
    // Fixed width instead of filling the row, centered in the free space by a spacer on each side.
    lv_obj_set_flex_grow(layer_box, 0);
    lv_obj_set_size(layer_box, LAYER_PILL_WIDTH, HEADER_HEIGHT);
    lv_obj_set_style_pad_hor(layer_box, 0, 0);
    header_spacer(cont);
    lv_obj_set_style_pad_ver(layer_box, 0, 0);
    lv_obj_set_style_shadow_width(layer_box, 0, 0);

    static const char *const names[4] = {"S", "C", "A", "G"};
    for (int i = 0; i < 4; i++) {
        mod_pills[i] = ui_create_mod_button(cont, names[i], false, mod_masks[i]);
        lv_obj_set_flex_grow(mod_pills[i], 0);
        lv_obj_set_size(mod_pills[i], 26, HEADER_HEIGHT);
        lv_obj_set_style_pad_ver(mod_pills[i], 0, 0);
        lv_obj_set_style_pad_ver(mod_pills[i], 0, LV_STATE_PRESSED);
        lv_obj_set_style_shadow_width(mod_pills[i], 0, 0);
        lv_obj_set_style_translate_x(mod_pills[i], -MOD_PILLS_SHIFT_LEFT, 0);
    }

    round_outer_top_corner(mod_pills[3], true);
}

static void create_typing_view(lv_obj_t *cont) {
    typing_view = create_view(cont);

    bongo_img = lv_img_create(typing_view);
    lv_img_set_src(bongo_img, bongocat_idle[0]);
    lv_obj_set_size(bongo_img, BONGOCAT_IMG_WIDTH, BONGOCAT_IMG_HEIGHT);
    lv_obj_set_style_img_recolor(bongo_img, theme_value_color(), 0);
    lv_obj_set_style_img_recolor_opa(bongo_img, LV_OPA_COVER, 0);

    lv_obj_t *wpm_row = lv_obj_create(typing_view);
    lv_obj_remove_style_all(wpm_row);
    lv_obj_set_size(wpm_row, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_flex_flow(wpm_row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(wpm_row, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_END, LV_FLEX_ALIGN_END);
    lv_obj_set_style_pad_column(wpm_row, 8, 0);

    wpm_label = lv_label_create(wpm_row);
    lv_obj_add_style(wpm_label, &get_current_ui_styles()->value_labels, 0);
    lv_obj_set_style_text_font(wpm_label, &jetbrainsmonoextrabold60, 0);
    lv_label_set_text(wpm_label, "0");

    lv_obj_t *wpm_unit = ui_create_secondary_text(wpm_row, "WPM", false, 0);
    lv_obj_set_style_pad_bottom(wpm_unit, 14, 0);

    stats_label = ui_create_secondary_text(typing_view, "AVG 0   MAX 0   KEYS 0", false, 0);

    wpm_bar = ui_create_progress_bar(typing_view, 0);
    lv_obj_set_width(wpm_bar, LV_PCT(100));
    lv_bar_set_range(wpm_bar, 0, LCD_WPM_BAR_MAX);
    lv_bar_set_value(wpm_bar, 0, LV_ANIM_OFF);

#ifdef LCD_DEBUG_RENDER_TIME
    render_label = ui_create_secondary_text(typing_view, "SCAN 0 MS   PEAK 0 MS", false, 0);
#endif
}

// Horizontal divider in the theme accent color (bar style). Local size overrides the
// bar style's fixed height.
static lv_obj_t *create_divider(lv_obj_t *parent, lv_coord_t height, lv_opa_t opa) {
    lv_obj_t *line = lv_obj_create(parent);
    lv_obj_remove_style_all(line);
    lv_obj_add_style(line, &get_current_ui_styles()->bar, 0);
    lv_obj_set_size(line, LV_PCT(100), height);
    lv_obj_set_style_radius(line, 0, 0);
    lv_obj_set_style_shadow_width(line, 0, 0);
    lv_obj_set_style_opa(line, opa, 0);
    lv_obj_clear_flag(line, LV_OBJ_FLAG_SCROLLABLE);
    return line;
}

// Mark the homing key with a border in the theme text color. (A keycap-style bump bar under the label
// does not fit: rows are ~19 px, it overlaps the text.) The homing button id is passed
// as the event user data.
static void homing_key_draw_cb(lv_event_t *e) {
    lv_obj_draw_part_dsc_t *dsc = lv_event_get_draw_part_dsc(e);
    if (dsc->class_p != &lv_btnmatrix_class || dsc->type != LV_BTNMATRIX_DRAW_PART_BTN) return;
    if (dsc->id != (uint32_t)(uintptr_t)lv_event_get_user_data(e)) return;

    // Text color, not the accent: held keys are accent-filled and must stay distinct.
    dsc->rect_dsc->border_color = theme_value_color();
    dsc->rect_dsc->border_width = 2;
    dsc->rect_dsc->border_opa   = LV_OPA_COVER;
}

static lv_obj_t *create_key_matrix(lv_obj_t *parent, const char **map, lv_coord_t height) {
    ui_styles_t *sty = get_current_ui_styles();
    lv_obj_t    *m   = lv_btnmatrix_create(parent);
    lv_btnmatrix_set_map(m, map);
    lv_obj_set_size(m, LV_PCT(100), height);
    lv_obj_clear_flag(m, LV_OBJ_FLAG_CLICKABLE);

    lv_obj_set_style_bg_opa(m, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_border_width(m, 0, LV_PART_MAIN);
    lv_obj_set_style_shadow_width(m, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_all(m, 0, LV_PART_MAIN);
    lv_obj_set_style_pad_gap(m, 2, LV_PART_MAIN);

    lv_obj_add_style(m, &sty->mod_btn, LV_PART_ITEMS);
    // Flat keys: gradients and shadows are expensive to software-render for 60 keys.
    lv_obj_set_style_bg_grad_dir(m, LV_GRAD_DIR_NONE, LV_PART_ITEMS);
    lv_obj_set_style_bg_grad_dir(m, LV_GRAD_DIR_NONE, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_add_style(m, &sty->mod_btn_pressed, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_text_font(m, &lv_font_montserrat_14, LV_PART_ITEMS);
    lv_obj_set_style_text_font(m, &lv_font_montserrat_14, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_shadow_width(m, 0, LV_PART_ITEMS);
    lv_obj_set_style_shadow_width(m, 0, LV_PART_ITEMS | LV_STATE_CHECKED);
    lv_obj_set_style_radius(m, 4, LV_PART_ITEMS);
    // KC_NO: outline only
    lv_obj_set_style_bg_opa(m, LV_OPA_TRANSP, LV_PART_ITEMS | LV_STATE_DISABLED);
    lv_obj_set_style_border_opa(m, LV_OPA_50, LV_PART_ITEMS | LV_STATE_DISABLED);
    return m;
}

// Same as create_divider, but vertical.
static lv_obj_t *create_vdivider(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, lv_coord_t width, lv_coord_t height, lv_opa_t opa) {
    lv_obj_t *line = create_divider(parent, height, opa);
    lv_obj_set_size(line, width, height);
    lv_obj_set_pos(line, x, y);
    return line;
}

/*
 * Layer view, top to bottom: left half / thick divider / right half.
 *
 * Each half is narrower than the screen and pushed to its own side, with a vertical
 * divider on its inner edge, so the two halves read like the split keyboard:
 *
 *   [ main rows    ]|
 *   [--------------]|
 *   [     |thumbs  ]|
 *   =================
 *  |[ main rows    ]
 *  |[--------------]
 *  |[ thumbs|      ]
 *
 * The thin vertical line marks the outer edge of each thumb cluster.
 */
static void create_layer_view(lv_obj_t *cont) {
    layer_view = create_view(cont);
    lv_obj_set_flex_align(layer_view, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_row(layer_view, LAYER_VIEW_GAP, 0);

    // View: half, gap, divider, gap, half. Half: main, gap, thin line, gap, thumbs.
    const lv_coord_t half_h  = (VIEW_HEIGHT - 2 * LAYER_VIEW_GAP - HALF_DIVIDER_HEIGHT) / 2;
    const lv_coord_t keys_h  = half_h - 2 * LAYER_VIEW_GAP - THUMB_DIVIDER_HEIGHT;
    const lv_coord_t main_h  = keys_h * MAIN_ROWS / HALF_ROWS;
    const lv_coord_t thumb_h = keys_h - main_h;
    const lv_coord_t thumb_y = main_h + 2 * LAYER_VIEW_GAP + THUMB_DIVIDER_HEIGHT;
    // Left edge of key column c inside a block of width HALF_BLOCK_WIDTH (2 px gaps).
#define KEY_COL_X(c) ((c) * (HALF_BLOCK_WIDTH + 2) / HALF_COLS)

    for (uint8_t h = 0; h < 2; h++) {
        uint8_t n = 0;
        for (uint8_t r = 0; r < MAIN_ROWS; r++) {
            for (uint8_t c = 0; c < HALF_COLS; c++) {
                snprintf(key_labels[h][r * HALF_COLS + c], sizeof(key_labels[h][0]), " ");
                main_maps[h][n++] = key_labels[h][r * HALF_COLS + c];
            }
            main_maps[h][n++] = (r == MAIN_ROWS - 1) ? "" : "\n";
        }
        for (uint8_t c = 0; c < HALF_COLS; c++) {
            snprintf(key_labels[h][MAIN_KEYS + c], sizeof(key_labels[h][0]), " ");
            thumb_maps[h][c] = key_labels[h][MAIN_KEYS + c];
        }
        thumb_maps[h][HALF_COLS] = "";

        if (h == 1) create_divider(layer_view, HALF_DIVIDER_HEIGHT, LV_OPA_COVER);

        lv_obj_t *half = lv_obj_create(layer_view);
        lv_obj_remove_style_all(half);
        lv_obj_set_size(half, LV_PCT(100), half_h);
        lv_obj_clear_flag(half, LV_OBJ_FLAG_SCROLLABLE);

        const lv_coord_t content_w = LCD_WIDTH - LCD_SAFE_PAD_LEFT - LCD_SAFE_PAD_RIGHT;
        const lv_coord_t block_x   = h == 0 ? 0 : content_w - HALF_BLOCK_WIDTH;
        const lv_coord_t inner_x   = h == 0 ? HALF_BLOCK_WIDTH + 3 : block_x - 3 - HALF_DIVIDER_HEIGHT;

        keymap_main[h] = create_key_matrix(half, main_maps[h], main_h);
        void *homing   = (void *)(uintptr_t)(h == 0 ? HOMING_KEY_LEFT : HOMING_KEY_RIGHT);
        lv_obj_add_event_cb(keymap_main[h], homing_key_draw_cb, LV_EVENT_DRAW_PART_BEGIN, homing);
        lv_obj_set_size(keymap_main[h], HALF_BLOCK_WIDTH, main_h);
        lv_obj_set_pos(keymap_main[h], block_x, 0);

        lv_obj_t *thin = create_divider(half, THUMB_DIVIDER_HEIGHT, LV_OPA_60);
        lv_obj_set_size(thin, HALF_BLOCK_WIDTH, THUMB_DIVIDER_HEIGHT);
        lv_obj_set_pos(thin, block_x, main_h + LAYER_VIEW_GAP);

        keymap_thumb[h] = create_key_matrix(half, thumb_maps[h], thumb_h);
        lv_obj_set_size(keymap_thumb[h], HALF_BLOCK_WIDTH, thumb_h);
        lv_obj_set_pos(keymap_thumb[h], block_x, thumb_y);

        // inner edge of the half
        create_vdivider(half, inner_x, 0, HALF_DIVIDER_HEIGHT, half_h, LV_OPA_COVER);
        // outer edge of the thumb cluster: left thumbs are columns 2-5, right thumbs 0-3
        const lv_coord_t cluster_x = block_x + (h == 0 ? KEY_COL_X(2) - 2 : KEY_COL_X(4) - 2);
        create_vdivider(half, cluster_x, thumb_y, THUMB_DIVIDER_HEIGHT, thumb_h, LV_OPA_60);

        for (uint8_t c = 0; c < HALF_COLS; c++) {
            if (is_empty_slot(h, HALF_ROWS - 1, c)) {
                lv_btnmatrix_set_btn_ctrl(keymap_thumb[h], c, LV_BTNMATRIX_CTRL_HIDDEN);
            }
        }
        pressed_bits[h]  = 0;
        disabled_bits[h] = 0;
    }
}

void load_module_base(void) {
    load_screen_base_base();
}

void init_screen_base(void) {
    ui_screen_base = lv_obj_create(NULL);
    lv_obj_t *cont = ui_create_container(ui_screen_base);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_row(cont, SCREEN_GAP, 0);
    lv_obj_set_style_pad_column(cont, 4, 0);

    create_header(cont);
    create_typing_view(cont);
    create_layer_view(cont);
    lv_obj_add_flag(layer_view, LV_OBJ_FLAG_HIDDEN);

    ui_screen_base_menu = lv_obj_create(NULL);
    lv_obj_t *cont_menu = ui_create_container(ui_screen_base_menu);

    menus[0] = (obj_update_dilemma_menu_t){
        ui_create_menu_line(cont_menu, "Back to main"),
        NULL,
    };
    menus[1] = (obj_update_dilemma_menu_t){
        ui_create_menu_line(cont_menu, "Pomodoro"),
        &menu_base_go_pomodoro,
    };
    menus[2] = (obj_update_dilemma_menu_t){
        ui_create_menu_line(cont_menu, "Change theme"),
        &menu_base_change_theme,
    };
}

static void menu_base_go_pomodoro(void) {
    set_current_module(MODULE_POMODORO);
}

static void menu_base_change_theme(void) {
    cycle_theme_and_save_in_eeprom();
    update_styles_from_current_theme();
    lv_obj_set_style_img_recolor(bongo_img, theme_value_color(), 0);
}

/* ----- header ----- */

static void update_header(uint8_t layer, bool force) {
    static uint8_t last_layer = 0xFF;
    if (layer != last_layer || force) {
        lv_label_set_text(layer_label, lcd_layer_name(layer));
        last_layer = layer;
    }
    for (int i = 0; i < 4; i++) {
        bool on  = dilemma_lcd_status.mods & mod_masks[i];
        bool was = dilemma_lcd_status_prev.mods & mod_masks[i];
        if (on != was || force) {
            if (on) {
                lv_obj_add_state(mod_pills[i], LV_STATE_PRESSED);
            } else {
                lv_obj_clear_state(mod_pills[i], LV_STATE_PRESSED);
            }
        }
    }
}

/* ----- layer view ----- */

// Rebuild key labels when the effective keymap changes (any layer or default layer
// change, since transparent keys fall through). We populate the layer map ourselves:
// on the slave half, layer_state is written by the split transport without running
// the layer_state_set callbacks the layer_map module relies on.
static void update_layer_map_labels(bool force) {
    static layer_state_t last_layer_state = 0, last_default_layer_state = 0;
    if (!force && layer_state == last_layer_state && default_layer_state == last_default_layer_state) {
        return;
    }
    last_layer_state         = layer_state;
    last_default_layer_state = default_layer_state;
    populate_layer_map();

    for (uint8_t h = 0; h < 2; h++) {
        uint32_t disabled = 0;
        for (uint8_t r = 0; r < HALF_ROWS; r++) {
            for (uint8_t c = 0; c < HALF_COLS; c++) {
                uint8_t i = r * HALF_COLS + c;
                if (is_empty_slot(h, r, c)) continue;
                uint16_t kc = layer_map[h * HALF_ROWS + r][c];
                keycode_label(kc, key_labels[h][i], sizeof(key_labels[h][i]));
                if (kc == KC_NO) disabled |= 1UL << i;
            }
        }
        uint32_t changed = disabled ^ disabled_bits[h];
        for (uint8_t i = 0; i < HALF_KEYS; i++) {
            if (changed & (1UL << i)) set_key_ctrl(h, i, LV_BTNMATRIX_CTRL_DISABLED, disabled & (1UL << i));
        }
        disabled_bits[h] = disabled;
        // labels were changed in place, the map pointers are unchanged
        lv_obj_invalidate(keymap_main[h]);
        lv_obj_invalidate(keymap_thumb[h]);
    }
}

// Highlight keys that are currently held. The left half sees the full matrix
// (SPLIT_TRANSPORT_MIRROR), so this works whichever half is master.
static void update_pressed_keys(void) {
    for (uint8_t h = 0; h < 2; h++) {
        uint32_t pressed = 0;
        for (uint8_t r = 0; r < HALF_ROWS; r++) {
            for (uint8_t c = 0; c < HALF_COLS; c++) {
                if (is_empty_slot(h, r, c)) continue;
                if (peek_matrix_layer_map(h * HALF_ROWS + r, c)) pressed |= 1UL << (r * HALF_COLS + c);
            }
        }
        uint32_t changed = pressed ^ pressed_bits[h];
        for (uint8_t i = 0; i < HALF_KEYS; i++) {
            if (changed & (1UL << i)) set_key_ctrl(h, i, LV_BTNMATRIX_CTRL_CHECKED, pressed & (1UL << i));
        }
        pressed_bits[h] = pressed;
    }
}

/* ----- typing view ----- */

// Runs on every layer, so typing on other layers still counts toward the stats.
static void sample_wpm_stats(void) {
    static uint32_t last_sample = 0;
    if (timer_elapsed32(last_sample) < 1000) return;
    last_sample = timer_read32();

    uint16_t wpm = get_current_wpm();
    if (wpm > wpm_max) wpm_max = wpm;
    // Only average while actually typing, so pauses do not drag the average down.
    if (wpm > 0 && timer_elapsed32(last_press_time) < LCD_WPM_AVG_ACTIVE_MS) {
        wpm_sum += wpm;
        wpm_samples++;
    }
}

static void update_wpm_stats(bool force) {
    static uint32_t last_update = 0;
    static uint16_t shown_wpm   = 0xFFFF;
    static uint32_t shown_keys  = 0xFFFFFFFF;
    static uint16_t shown_avg = 0xFFFF, shown_max = 0xFFFF;

    if (!force && timer_elapsed32(last_update) < LCD_STATS_UPDATE_MS) return;
    last_update = timer_read32();

    uint16_t wpm = get_current_wpm();
    uint16_t avg = wpm_samples ? wpm_sum / wpm_samples : 0;

    if (wpm != shown_wpm) {
        lv_label_set_text_fmt(wpm_label, "%u", wpm);
        lv_bar_set_value(wpm_bar, MIN(wpm, LCD_WPM_BAR_MAX), LV_ANIM_OFF);
        shown_wpm = wpm;
    }
    if (avg != shown_avg || wpm_max != shown_max || keys_pressed != shown_keys) {
        lv_label_set_text_fmt(stats_label, "AVG %u   MAX %u   KEYS %lu", avg, wpm_max, (unsigned long)keys_pressed);
        shown_avg  = avg;
        shown_max  = wpm_max;
        shown_keys = keys_pressed;
    }
#ifdef LCD_DEBUG_RENDER_TIME
    // Longest gap between key scans on this half: last update window, and since boot.
    lv_label_set_text_fmt(render_label, "SCAN %lu MS   PEAK %lu MS", (unsigned long)lcd_scan_gap_take_window(), (unsigned long)lcd_scan_gap_peak());
#endif
}

// The cat slaps a paw on every key press (alternating paws), holds "ready" for a
// moment after the last press, then falls back to the idle loop.
static void update_bongocat(void) {
    static const lv_img_dsc_t *shown      = NULL;
    static uint32_t            idle_timer = 0;
    static uint8_t             idle_frame = 0;
    const lv_img_dsc_t        *next;

    uint32_t since = timer_elapsed32(last_press_time);
    if (tap_pending) {
        tap_pending = false;
        tap_frame   = (tap_frame + 1) % BONGOCAT_TAP_FRAMES;
        next        = bongocat_tap[tap_frame];
    } else if (last_press_time && since < BONGOCAT_TAP_HOLD_MS) {
        next = shown;
    } else if (last_press_time && since < BONGOCAT_PREP_HOLD_MS) {
        next = bongocat_prep[0];
    } else {
        if (timer_elapsed32(idle_timer) >= BONGOCAT_IDLE_FRAME_MS) {
            idle_timer = timer_read32();
            idle_frame = (idle_frame + 1) % BONGOCAT_IDLE_FRAMES;
        }
        next = bongocat_idle[BONGOCAT_IDLE_FRAMES - 1 - idle_frame];
    }

    if (next && next != shown) {
        lv_img_set_src(bongo_img, next);
        shown = next;
    }
}

/* ======================================================================== */

void housekeeping_task_screen_base(void) {
    dilemma_lcd_status = get_dilemma_status();
    refresh_screen_base();
    dilemma_lcd_status_prev = dilemma_lcd_status;
}

static void load_screen_base_base(void) {
    load_screen_xx_base(menus, sizeof(menus) / sizeof(obj_update_dilemma_menu_t), ui_screen_base);
}

static void load_screen_base_menu(void) {
    load_screen_xx_menu(menus, sizeof(menus) / sizeof(obj_update_dilemma_menu_t), ui_screen_base_menu);
}

static void refresh_screen_base(void) {
    static uint8_t last_layer   = 0xFF;
    static bool    first_run    = true;
    static bool    typing_shown = true;
    uint8_t        layer        = get_highest_layer(layer_state | default_layer_state);

    if (layer != last_layer) {
        if (layer == LAYER_MENU) {
            if (screen_index == 0) {
                load_screen_base_menu();
                screen_index = 1;
            }
        } else if (screen_index == 1) {
            // Leaving the menu layer triggers the selected menu entry.
            load_screen_base_base();
            trigger_menu_element(menus, menu_index);
            menu_index   = 0;
            screen_index = 0;
        }
    }
    last_layer = layer;

    sample_wpm_stats();

    // The menu entry may have switched to another module: stop drawing this one.
    if (screen_index == 1 || lv_scr_act() != ui_screen_base) {
        return;
    }

    update_header(layer, first_run);

    // Typing view whenever no layer is active above the default one (Colemak or QWERTY).
    bool typing = (layer == get_highest_layer(default_layer_state));
    bool view_switched = (typing != typing_shown || first_run);
    if (view_switched) {
        if (typing) {
            lv_obj_add_flag(layer_view, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(typing_view, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(typing_view, LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(layer_view, LV_OBJ_FLAG_HIDDEN);
        }
        typing_shown = typing;
    }

    if (typing) {
        update_wpm_stats(view_switched);
        update_bongocat();
    } else {
        update_layer_map_labels(first_run);
        update_pressed_keys();
    }
    first_run = false;
}

bool process_record_screen_base(uint16_t keycode, keyrecord_t *record) {
    if (screen_index == 1) {
        process_record_menu(keycode, record, menus, &menu_index, sizeof(menus) / sizeof(obj_update_dilemma_menu_t));
    }
    if (record->event.pressed) {
        last_press_time = timer_read32();
        if (last_press_time == 0) last_press_time = 1; // 0 means "never pressed"
        keys_pressed++;
        tap_pending = true;
    }
    return true;
}
