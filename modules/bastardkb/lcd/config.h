#pragma once

// TODO move this to a module definition, move the dilemma sync definition to the sync module
#define SPLIT_TRANSACTION_IDS_KB RPC_ID_KEYCODE_SYNC, RPC_ID_DILEMMA_SYNC

// QP stuff
#define SPI_SCK_PIN GP22  // as per vik connector
#define SPI_MOSI_PIN GP23 // as per vik connector
#define SPI_MISO_PIN GP18 // Unused
#define LCD_RST_PIN GP16  // Unused, TODO connect to Vcc for now, test with SCL later
#define LCD_DC_PIN GP12   // vik_gp1
#define LCD_CS_PIN GP11   // vik_gp2
#define LCD_BLK_PIN GP4   // not used
#ifdef BACKLIGHT_LEVELS
#    undef BACKLIGHT_LEVELS
#endif
#define BACKLIGHT_LEVELS 16
#define BACKLIGHT_PIN GP20 // backlight pin for lcd
#define BACKLIGHT_PWM_DRIVER PWMD2
#define BACKLIGHT_PWM_CHANNEL RP2040_PWM_CHANNEL_A

#define SPI_DRIVER SPID0
#define LCD_SPI_DIVISOR 2
#define LCD_WAIT_TIME 150
#define LCD_ROTATION QP_ROTATION_0
#define LCD_OFFSET_X 0
#define LCD_OFFSET_Y 15
#define LCD_WIDTH 240  // Set according to your display specs
#define LCD_HEIGHT 280 // Set according to your display specs
// Inner padding of every screen. Measured on the keyboard: the case opening over the
// screen hides the leftmost ~6-8 px and the right edge past x ~ 236, and has rounded
// corners. Left/right are uneven so content (e.g. the WPM bar) looks centered.
#ifndef LCD_SAFE_PAD_LEFT
#    define LCD_SAFE_PAD_LEFT 14
#endif
#ifndef LCD_SAFE_PAD_RIGHT
#    define LCD_SAFE_PAD_RIGHT 8
#endif
#ifndef LCD_SAFE_PAD_TOP
#    define LCD_SAFE_PAD_TOP 16
#endif
#ifndef LCD_SAFE_PAD_BOTTOM
#    define LCD_SAFE_PAD_BOTTOM 5
#endif
#define SPI_MODE 3     // Set according to your display specs
#define ST7789         // Set according to your display specs GC_9A01 or ST7789
#ifdef QUANTUM_PAINTER_SUPPORTS_NATIVE_COLORS
#    undef QUANTUM_PAINTER_SUPPORTS_NATIVE_COLORS
#endif
#define QUANTUM_PAINTER_SUPPORTS_NATIVE_COLORS TRUE
// Timeout configuration, default 30000 (30 sek). 0 = No timeout. Beware of image retention.
#ifdef QUANTUM_PAINTER_DISPLAY_TIMEOUT
#    undef QUANTUM_PAINTER_DISPLAY_TIMEOUT
#endif
#define QUANTUM_PAINTER_DISPLAY_TIMEOUT 0
// #define QUANTUM_PAINTER_PIXDATA_BUFFER_SIZE 4096
#ifdef QUANTUM_PAINTER_TASK_THROTTLE
#    undef QUANTUM_PAINTER_TASK_THROTTLE
#endif
#define QUANTUM_PAINTER_TASK_THROTTLE 32 // 30 FPS ish
#define RGB_MATRIX_MODE_NAME_ENABLE
#ifdef QP_LVGL_TASK_PERIOD
#    undef QP_LVGL_TASK_PERIOD
#endif
#define QP_LVGL_TASK_PERIOD 66 // throttle lvgl for keyboard responsiveness

// #define QUANTUM_PAINTER_DISPLAY_TIMEOUT 5000

// Custom display stuff
#define BK_PALETTE LV_PALETTE_CYAN
// Layer that opens the LCD menu. Module config is read before the keymap config.h, so
// override it there with #undef LAYER_MENU + #define LAYER_MENU <n>.
#ifndef LAYER_MENU
#    define LAYER_MENU 4
#endif

#define SPLIT_TRANSPORT_MIRROR
#define SPLIT_MODS_ENABLE
#define SPLIT_POINTING_ENABLE
// WPM is shown on the left half even when the right half is master.
#define SPLIT_WPM_ENABLE