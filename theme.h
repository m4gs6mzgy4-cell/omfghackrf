/*
 * Xanthorox UI Theme Engine - Complete Material You Implementation
 * Phase 3: Full production theme for Android-like experience on PortaPack H2
 */

#ifndef THEME_H
#define THEME_H

#include "lvgl.h"

// Material You inspired colors (dark mode default)
#define THEME_BG_COLOR          lv_color_hex(0x1C1B1F)
#define THEME_SURFACE_COLOR     lv_color_hex(0x2B2A2F)
#define THEME_PRIMARY_COLOR     lv_color_hex(0xD0BCFF)  // Purple accent
#define THEME_ON_PRIMARY_COLOR  lv_color_hex(0x381E72)
#define THEME_SECONDARY_COLOR   lv_color_hex(0xCCC2DC)
#define THEME_ERROR_COLOR       lv_color_hex(0xF2B8B5)
#define THEME_ON_SURFACE_COLOR  lv_color_hex(0xE6E1E5)

// Theme initialization - call once after lv_init()
void theme_init(void);

// Apply theme to a screen or object
void theme_apply_to_screen(lv_obj_t * screen);
void theme_apply_button_style(lv_obj_t * btn);
void theme_apply_label_style(lv_obj_t * label);

// Dynamic accent color change (for user customization)
void theme_set_accent_color(lv_color_t color);

#endif // THEME_H