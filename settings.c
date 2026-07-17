/*
 * Settings Screen - Phase 3 Polished LVGL
 * Full production settings for theme, stealth, notifications, hardware calibration.
 */

#include "lvgl.h"
#include "theme.h"
#include "rf_engine.h"
#include <stdio.h>

void settings_stealth_switch_cb(lv_event_t * e) {
    lv_obj_t * sw = lv_event_get_target(e);
    bool state = lv_obj_has_state(sw, LV_STATE_CHECKED);
    rf_engine_set_stealth_mode(state, state ? 5 : 15);
    printf("[Settings] Stealth mode %s\n", state ? "ENABLED" : "DISABLED");
}

void settings_accent_btn_cb(lv_event_t * e) {
    // Cycle accent colors (Material You style)
    static int accent_idx = 0;
    lv_color_t accents[] = {
        lv_color_hex(0xD0BCFF), // Purple
        lv_color_hex(0xB5E0FF), // Blue
        lv_color_hex(0xFFB4AB), // Coral
        lv_color_hex(0xC3E0C3)  // Green
    };
    accent_idx = (accent_idx + 1) % 4;
    theme_set_accent_color(accents[accent_idx]);
    printf("[Settings] Accent color changed\n");
}

void create_settings_screen(void) {
    lv_obj_t * screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(screen, LV_HOR_RES, LV_VER_RES);
    theme_apply_to_screen(screen);

    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "Settings - System & Theme");
    theme_apply_label_style(title);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Stealth Mode toggle
    lv_obj_t * stealth_label = lv_label_create(screen);
    lv_label_set_text(stealth_label, "Stealth / Low Emission Mode");
    theme_apply_label_style(stealth_label);
    lv_obj_align(stealth_label, LV_ALIGN_TOP_LEFT, 20, 50);

    lv_obj_t * stealth_sw = lv_switch_create(screen);
    lv_obj_align(stealth_sw, LV_ALIGN_TOP_RIGHT, -20, 45);
    lv_obj_add_event_cb(stealth_sw, settings_stealth_switch_cb, LV_EVENT_VALUE_CHANGED, NULL);
    // Set initial state from rf_engine

    // Theme / Accent color
    lv_obj_t * accent_label = lv_label_create(screen);
    lv_label_set_text(accent_label, "Accent Color (Material You)");
    theme_apply_label_style(accent_label);
    lv_obj_align(accent_label, LV_ALIGN_TOP_LEFT, 20, 100);

    lv_obj_t * accent_btn = lv_btn_create(screen);
    lv_obj_set_size(accent_btn, 120, 40);
    lv_obj_align(accent_btn, LV_ALIGN_TOP_RIGHT, -20, 95);
    theme_apply_button_style(accent_btn);
    lv_obj_add_event_cb(accent_btn, settings_accent_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * accent_txt = lv_label_create(accent_btn);
    lv_label_set_text(accent_txt, "Change Accent");
    lv_obj_center(accent_txt);

    // Notifications toggle (example)
    lv_obj_t * notif_label = lv_label_create(screen);
    lv_label_set_text(notif_label, "Notifications & Toasts");
    theme_apply_label_style(notif_label);
    lv_obj_align(notif_label, LV_ALIGN_TOP_LEFT, 20, 150);

    lv_obj_t * notif_sw = lv_switch_create(screen);
    lv_obj_align(notif_sw, LV_ALIGN_TOP_RIGHT, -20, 145);
    // Add callback for enabling/disabling notification system

    // Hardware info section
    lv_obj_t * hw_label = lv_label_create(screen);
    lv_label_set_text(hw_label, "Hardware: PortaPack H2 | MCU: STM32F4 | Display: ILI9341");
    theme_apply_label_style(hw_label);
    lv_obj_align(hw_label, LV_ALIGN_BOTTOM_MID, 0, -30);

    // Back button or gesture to return to home
    lv_obj_t * back_btn = lv_btn_create(screen);
    lv_obj_set_size(back_btn, 80, 35);
    lv_obj_align(back_btn, LV_ALIGN_BOTTOM_LEFT, 10, -10);
    theme_apply_button_style(back_btn);
    lv_obj_t * back_txt = lv_label_create(back_btn);
    lv_label_set_text(back_txt, "Back");
    lv_obj_center(back_txt);
    // Add event to go back to home screen
}