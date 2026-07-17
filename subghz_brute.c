/*
 * SubGHz Brute App - Core App for Xanthorox PortaPack H2 Firmware
 * Comprehensive brute-force for sub-GHz remotes (garage, gates, cars, alarms).
 * Dictionary + incremental attacks, rolling code support, power scanning.
 */

#include "lvgl.h"
#include "rf_engine.h"
#include <stdio.h>

static signal_capture_t current_capture;

void subghz_capture_btn_cb(lv_event_t * e) {
    printf("[SubGHz Brute] Starting signal capture for brute force...\n");
    current_capture.frequency = 433920000;
    rf_engine_capture(&current_capture, 4000);
}

void subghz_attack_btn_cb(lv_event_t * e) {
    printf("[SubGHz Brute] ATTACK - Dictionary + incremental brute force initiated.\n");
    rf_engine_set_stealth_mode(true, 10);
    rf_engine_subghz_brute_attack();
    // TODO: Dictionary selection, incremental range UI, power scan results display
}

void create_subghz_brute_screen(void) {
    lv_obj_t * screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(screen, LV_HOR_RES, LV_VER_RES);

    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "SubGHz Brute - Remote & Gate Attacks");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t * spectrum_area = lv_obj_create(screen);
    lv_obj_set_size(spectrum_area, LV_HOR_RES - 20, 120);
    lv_obj_align(spectrum_area, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t * preset_label = lv_label_create(screen);
    lv_label_set_text(preset_label, "Presets: Garage | Gate | Car Alarm | Rolling Code");
    lv_obj_align(preset_label, LV_ALIGN_TOP_MID, 0, 170);

    lv_obj_t * capture_btn = lv_btn_create(screen);
    lv_obj_set_size(capture_btn, 140, 60);
    lv_obj_align(capture_btn, LV_ALIGN_LEFT_MID, 20, 50);
    lv_obj_add_event_cb(capture_btn, subghz_capture_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * cap_label = lv_label_create(capture_btn);
    lv_label_set_text(cap_label, "CAPTURE");
    lv_obj_center(cap_label);

    lv_obj_t * attack_btn = lv_btn_create(screen);
    lv_obj_set_size(attack_btn, 140, 60);
    lv_obj_align(attack_btn, LV_ALIGN_RIGHT_MID, -20, 50);
    lv_obj_add_event_cb(attack_btn, subghz_attack_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * atk_label = lv_label_create(attack_btn);
    lv_label_set_text(atk_label, "ATTACK");
    lv_obj_center(atk_label);

    lv_obj_t * stealth_switch = lv_switch_create(screen);
    lv_obj_align(stealth_switch, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t * status = lv_label_create(screen);
    lv_label_set_text(status, "Status: Ready | Stealth: ON | Attempts: 0");
    lv_obj_align(status, LV_ALIGN_BOTTOM_MID, 0, -60);
}