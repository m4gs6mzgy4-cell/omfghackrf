/*
 * File Manager App - Core component for Xanthorox PortaPack H2 Firmware
 * Phase 2: Polished intuitive Android-style file browser for captures, signals, presets.
 * Unified signal library browser with tagging, search, export, replay from file.
 */

#include "lvgl.h"
#include "rf_engine.h"
#include <stdio.h>
#include <string.h>

void file_manager_refresh_list(lv_obj_t * list) {
    // TODO: Scan /sdcard/signals/ directory, populate list with .iq .wav files
    // Example static entries for skeleton
    lv_obj_t * item1 = lv_list_add_btn(list, LV_SYMBOL_FILE, "capture_433_vehicle.iq  [ProtoPirate]");
    lv_obj_t * item2 = lv_list_add_btn(list, LV_SYMBOL_FILE, "rolljam_315_gate.iq     [Rolljam]");
    lv_obj_t * item3 = lv_list_add_btn(list, LV_SYMBOL_FILE, "subghz_brute_garage.iq  [SubGHz]");
    // Add click handlers to load/replay selected file via rf_engine_replay
}

void create_file_manager_screen(void) {
    lv_obj_t * screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(screen, LV_HOR_RES, LV_VER_RES);

    lv_obj_t * title = lv_label_create(screen);
    lv_label_set_text(title, "File Manager - Captures & Signals");
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);

    lv_obj_t * list = lv_list_create(screen);
    lv_obj_set_size(list, LV_HOR_RES - 20, 280);
    lv_obj_align(list, LV_ALIGN_TOP_MID, 0, 45);

    file_manager_refresh_list(list);

    // Bottom action bar - Export, Delete, Replay selected
    lv_obj_t * action_bar = lv_obj_create(screen);
    lv_obj_set_size(action_bar, LV_HOR_RES, 50);
    lv_obj_align(action_bar, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t * export_btn = lv_btn_create(action_bar);
    lv_obj_set_size(export_btn, 80, 40);
    lv_obj_align(export_btn, LV_ALIGN_LEFT_MID, 10, 0);
    lv_obj_t * exp_label = lv_label_create(export_btn);
    lv_label_set_text(exp_label, "Export");
    lv_obj_center(exp_label);

    lv_obj_t * replay_btn = lv_btn_create(action_bar);
    lv_obj_set_size(replay_btn, 80, 40);
    lv_obj_align(replay_btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_t * rep_label = lv_label_create(replay_btn);
    lv_label_set_text(rep_label, "Replay");
    lv_obj_center(rep_label);

    lv_obj_t * delete_btn = lv_btn_create(action_bar);
    lv_obj_set_size(delete_btn, 80, 40);
    lv_obj_align(delete_btn, LV_ALIGN_RIGHT_MID, -10, 0);
    lv_obj_t * del_label = lv_label_create(delete_btn);
    lv_label_set_text(del_label, "Delete");
    lv_obj_center(del_label);

    // Search / filter bar at top or integrated
    lv_obj_t * search_hint = lv_label_create(screen);
    lv_label_set_text(search_hint, "Search: tag, frequency, app...");
    lv_obj_align(search_hint, LV_ALIGN_TOP_MID, 0, 35);
}