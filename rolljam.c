/*
 * Rolljam App - Core App for Xanthorox PortaPack H2 Firmware
 * 
 * UI: Android-style screen with large touch targets.
 * Features: Real-time capture, selective jam, replay with timing, presets.
 * One-tap "ATTACK" button.
 * Integrates with unified rf_engine and live spectrum.
 */

#include "lvgl.h"
#include "rf_engine.h"
#include <stdio.h>

static signal_capture_t current_capture;

void rolljam_capture_btn_cb(lv_event_t * e) {
    printf("[Rolljam] Starting capture...\n");
    current_capture.frequency = 433920000; // Example common frequency - customize per preset
    rf_engine_capture(&current_capture, 5000); // 5 seconds
    // Update UI with captured signal info
}

void rolljam_attack_btn_cb(lv_event_t * e) {
    printf("[Rolljam] ATTACK button pressed - Executing Rolljam sequence.\n");
    rf_engine_set_stealth_mode(true, 5); // Low emission
    rf_engine_rolljam_attack();  // Hook to core engine
    // TODO: Add timing precision UI controls, jam duration slider
}

void create_rolljam_screen(void) {
    lv_obj_t * screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(screen, LV_HOR_RES, LV_VER_RES);

    // Title bar
    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "Rolljam - Rolling Code Attack");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    // Live spectrum preview area
    lv_obj_t * spectrum_area = lv_obj_create(screen);
    lv_obj_set_size(spectrum_area, LV_HOR_RES - 20, 120);
    lv_obj_align(spectrum_area, LV_ALIGN_TOP_MID, 0, 40);
    // Hook rf_engine_get_spectrum() to update this area periodically

    // Presets row
    lv_obj_t * preset_label = lv_label_create(screen);
    lv_label_set_text(preset_label, "Presets: Vehicle | Gate | Alarm");
    lv_obj_align(preset_label, LV_ALIGN_TOP_MID, 0, 170);

    // Big Capture button
    lv_obj_t * capture_btn = lv_btn_create(screen);
    lv_obj_set_size(capture_btn, 140, 60);
    lv_obj_align(capture_btn, LV_ALIGN_LEFT_MID, 20, 50);
    lv_obj_add_event_cb(capture_btn, rolljam_capture_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * cap_label = lv_label_create(capture_btn);
    lv_label_set_text(cap_label, "CAPTURE");
    lv_obj_center(cap_label);

    // Big Attack button (prominent, red accent for action)
    lv_obj_t * attack_btn = lv_btn_create(screen);
    lv_obj_set_size(attack_btn, 140, 60);
    lv_obj_align(attack_btn, LV_ALIGN_RIGHT_MID, -20, 50);
    lv_obj_add_event_cb(attack_btn, rolljam_attack_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * atk_label = lv_label_create(attack_btn);
    lv_label_set_text(atk_label, "ATTACK");
    lv_obj_center(atk_label);

    // Stealth toggle
    lv_obj_t * stealth_switch = lv_switch_create(screen);
    lv_obj_align(stealth_switch, LV_ALIGN_BOTTOM_MID, 0, -20);
    // Add event to call rf_engine_set_stealth_mode on change

    // Status / log area
    lv_obj_t * status = lv_label_create(screen);
    lv_label_set_text(status, "Status: Ready | Stealth: ON | Signals captured: 0");
    lv_obj_align(status, LV_ALIGN_BOTTOM_MID, 0, -60);
}