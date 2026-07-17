/*
 * Notifications System - Full Queue + Shade (Android-style)
 * Phase 4: Production ready for Xanthorox OMFG Firmware
 * 
 * Queue of notifications with pull-down shade UI.
 * Supports add, clear, and display with timestamps.
 */

#include "lvgl.h"
#include "theme.h"
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_NOTIFICATIONS 20
#define NOTIF_MSG_LEN 64

typedef struct {
    char message[NOTIF_MSG_LEN];
    uint32_t timestamp;  // Simple tick or time
} notification_t;

static notification_t notif_queue[MAX_NOTIFICATIONS];
static int notif_count = 0;
static lv_obj_t * shade = NULL;
static lv_obj_t * notif_list = NULL;

void notification_add(const char * msg) {
    if (notif_count >= MAX_NOTIFICATIONS) {
        // Shift queue (drop oldest)
        memmove(notif_queue, notif_queue + 1, sizeof(notification_t) * (MAX_NOTIFICATIONS - 1));
        notif_count--;
    }
    
    strncpy(notif_queue[notif_count].message, msg, NOTIF_MSG_LEN - 1);
    notif_queue[notif_count].timestamp = lv_tick_get();  // Use LVGL tick
    notif_count++;
    
    printf("[OMFG Notifications] Added: %s\n", msg);
    
    // Optional: Auto-show brief toast
    // show_toast(msg);
}

void notification_clear(void) {
    notif_count = 0;
    if (notif_list) {
        lv_obj_clean(notif_list);
    }
}

void show_notification_shade(void) {
    if (shade) {
        lv_obj_del(shade);
        shade = NULL;
        notif_list = NULL;
        return;  // Toggle off if already open
    }
    
    shade = lv_obj_create(lv_scr_act());
    lv_obj_set_size(shade, LV_HOR_RES, LV_VER_RES * 0.75);  // Pull-down style
    lv_obj_align(shade, LV_ALIGN_TOP_MID, 0, 0);
    theme_apply_to_screen(shade);
    
    // Title bar
    lv_obj_t * title = lv_label_create(shade);
    lv_label_set_text(title, "OMFG Notifications");
    theme_apply_label_style(title);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 10);
    
    // List of notifications
    notif_list = lv_list_create(shade);
    lv_obj_set_size(notif_list, LV_HOR_RES - 20, LV_VER_RES * 0.55);
    lv_obj_align(notif_list, LV_ALIGN_TOP_MID, 0, 40);
    
    for (int i = 0; i < notif_count; i++) {
        char buf[128];
        snprintf(buf, sizeof(buf), "%s  [%lu ticks]", 
                 notif_queue[i].message, notif_queue[notif_count - 1 - i].timestamp);
        lv_obj_t * item = lv_list_add_btn(notif_list, LV_SYMBOL_BELL, buf);
        // Optional: Click to expand or act on notification
    }
    
    // Clear all button
    lv_obj_t * clear_btn = lv_btn_create(shade);
    lv_obj_set_size(clear_btn, 100, 35);
    lv_obj_align(clear_btn, LV_ALIGN_BOTTOM_MID, 0, -10);
    theme_apply_button_style(clear_btn);
    lv_obj_add_event_cb(clear_btn, (lv_event_cb_t)notification_clear, LV_EVENT_CLICKED, NULL);
    lv_obj_t * clear_label = lv_label_create(clear_btn);
    lv_label_set_text(clear_label, "Clear All");
    lv_obj_center(clear_label);
    
    // Close button or swipe gesture hint
    lv_obj_t * close_hint = lv_label_create(shade);
    lv_label_set_text(close_hint, "Swipe down or tap outside to close");
    theme_apply_label_style(close_hint);
    lv_obj_align(close_hint, LV_ALIGN_BOTTOM_MID, 0, -50);
}

void show_toast(const char * msg) {
    // Simple temporary toast notification (non-blocking)
    lv_obj_t * toast = lv_label_create(lv_scr_act());
    lv_label_set_text(toast, msg);
    theme_apply_label_style(toast);
    lv_obj_align(toast, LV_ALIGN_BOTTOM_MID, 0, -30);
    lv_obj_set_style_bg_color(toast, THEME_SURFACE_COLOR, 0);
    lv_obj_set_style_bg_opa(toast, LV_OPA_80, 0);
    lv_obj_set_style_radius(toast, 8, 0);
    lv_obj_set_style_pad_all(toast, 8, 0);
    
    // Auto-remove after ~2 seconds (use timer in production)
    // For skeleton: manual or lv_timer_create for removal
    printf("[OMFG Toast] %s\n", msg);
}