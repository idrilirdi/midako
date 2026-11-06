#ifndef KEYCODE_LABEL_H
#define KEYCODE_LABEL_H

#include <stddef.h>
#include <stdint.h>

/**
 * Short label (at most 3 glyphs, about 32 px in montserrat 14) for a keycode, sized for one key cell on the
 * LCD layer map. The result may contain LVGL symbol glyphs (LV_SYMBOL_*), so it
 * must be drawn with a font that includes them (the built-in montserrat fonts).
 * Writes at least " " (never an empty string, which would end a btnmatrix map).
 */
void keycode_label(uint16_t keycode, char *buf, size_t size);

/**
 * Label for keymap-specific keycodes (macros, custom keycodes). Weak: return NULL for
 * keycodes it does not handle. Checked before the built-in labels.
 */
const char *keycode_label_user(uint16_t keycode);

/**
 * Display name of a layer. Weak: override it in the keymap to name your layers.
 */
const char *lcd_layer_name(uint8_t layer);

#endif
