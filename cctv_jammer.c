/*
 * CCTV Jammer App - Core App for Xanthorox PortaPack H2 Firmware
 * Multi-band video surveillance disruption tool.
 * Continuous or burst jamming on 2.4GHz, 5.8GHz, analog, WiFi cams.
 * Selective camera targeting, deauth integration, recording disruption with power control.
 */

#include "lvgl.h"
#include "rf_engine.h"
#include <stdio.h>

void cctv_jam_btn_cb(lv_event_t * e) {
    printf("[CCTV Jammer] ATTACK - Multi-band jamming sequence started.\n");
    rf_engine_set_stealth_mode(false, 15); // Higher power for jamming effectiveness
    rf_engine_cctv_jammer_start();
    // TODO: Band selection checkboxes (2.4/5.8/analog/WiFi), burst vs continuous, selective targeting UI, power slider
}

void create_cctv_jammer_screen(void) {
    lv_obj_t * screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(screen, LV_HOR_RES, LV_VER_RES);

    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "CCTV Jammer - Surveillance Disruption");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t * spectrum_area = lv_obj_create(screen);
    lv_obj_set_size(spectrum_area, LV_HOR_RES - 20, 120);
    lv_obj_align(spectrum_area, LV_ALIGN_TOP_MID, 0, 40);

    lv_obj_t * band_label = lv_label_create(screen);
    lv_label_set_text(band_label, "Bands: 2.4 GHz | 5.8 GHz | Analog | WiFi Deauth");
    lv_obj_align(band_label, LV_ALIGN_TOP_MID, 0, 170);

    // Prominent Jam button (no separate capture needed for pure jammer)
    lv_obj_t * jam_btn = lv_btn_create(screen);
    lv_obj_set_size(jam_btn, 200, 70);
    lv_obj_align(jam_btn, LV_ALIGN_CENTER, 0, 30);
    lv_obj_add_event_cb(jam_btn, cctv_jam_btn_cb, LV_EVENT_CLICKED, NULL);
    lv_obj_t * jam_label = lv_label_create(jam_btn);
    lv_label_set_text(jam_label, "START JAMMING");
    lv_obj_center(jam_label);

    lv_obj_t * stealth_switch = lv_switch_create(screen);
    lv_obj_align(stealth_switch, LV_ALIGN_BOTTOM_MID, 0, -20);

    lv_obj_t * status = lv_label_create(screen);
    lv_label_set_text(status, "Status: Ready | Power: High | Cameras disrupted: 0");
    lv_obj_align(status, LV_ALIGN_BOTTOM_MID, 0, -60);
}