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

#include "../../../../common/common_config.h"

#define ENCODER_RESOLUTION 4
// LCD layer map: rows 0-4 are the left half, rows 5-9 the right half, in visual order
// (see layer_remap in keymap.c).
#define LAYER_MAP_REMAPPING
#define LAYER_MAP_ROWS 10
#define LAYER_MAP_COLS 6

// LCD menu layer, above all layers of common_keymap.h (the module default, 4, is SYMBOLS here). Toggled by a combo.
#undef LAYER_MENU
#define LAYER_MENU 9

// Show the longest gap (ms) between key scans on the LCD typing view.
#define LCD_DEBUG_RENDER_TIME
