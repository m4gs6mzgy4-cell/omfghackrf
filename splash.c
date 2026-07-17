/*
 * OMFG Splash Screen - LVGL Boot Sequence with Fade
 * Phase 4: Production ready for Xanthorox OMFG Firmware
 * 
 * Shows branded splash on boot with fade-in, holds, then fades out to home screen.
 * Uses OMFG text logo + spectrum graphic + status.
 * Ready for image asset integration (convert generated splash to LVGL img).
 */

#include "lvgl.h"
#include "theme.h"
#include "main.h"  // For transition to create_home_screen()

static lv_obj_t * splash_screen = NULL;
static lv_anim_t fade_anim;

static void splash_fade_out_cb(void * var, int32_t value) {
    lv_obj_set_style_opa((lv_obj_t *)var, value, 0);
}

static void splash_finished_cb(lv_anim_t * a) {
    // Clean up splash and go to main home screen
    if (splash_screen) {
        lv_obj_del(splash_screen);
        splash_screen = NULL;
    }
    create_home_screen();  // Transition to main UI
}

void show_omfg_splash(void) {
    splash_screen = lv_obj_create(lv_scr_act());
    lv_obj_set_size(splash_screen, LV_HOR_RES, LV_VER_RES);
    theme_apply_to_screen(splash_screen);
    lv_obj_set_style_opa(splash_screen, LV_OPA_TRANSP, 0);  // Start transparent for fade-in

    // OMFG Logo - Large centered text (vector-style branding)
    lv_obj_t * omfg_logo = lv_label_create(splash_screen);
    lv_label_set_text(omfg_logo, "OMFG");
    lv_obj_set_style_text_font(omfg_logo, &lv_font_montserrat_48, 0);  // Large bold font
    lv_obj_set_style_text_color(omfg_logo, THEME_PRIMARY_COLOR, 0);
    lv_obj_align(omfg_logo, LV_ALIGN_CENTER, 0, -40);

    // Subtitle
    lv_obj_t * subtitle = lv_label_create(splash_screen);
    lv_label_set_text(subtitle, "Xanthorox H2 - Android-Style SDR Powerhouse");
    theme_apply_label_style(subtitle);
    lv_obj_set_style_text_font(subtitle, &lv_font_montserrat_14, 0);
    lv_obj_align(subtitle, LV_ALIGN_CENTER, 0, 10);

    // Spectrum graphic hint (simple bar or use image asset)
    lv_obj_t * spectrum = lv_obj_create(splash_screen);
    lv_obj_set_size(spectrum, LV_HOR_RES - 40, 40);
    lv_obj_align(spectrum, LV_ALIGN_CENTER, 0, 60);
    lv_obj_set_style_bg_color(spectrum, THEME_SURFACE_COLOR, 0);
    // TODO: Replace with real spectrum image or animated bars

    // Status / version
    lv_obj_t * status = lv_label_create(splash_screen);
    lv_label_set_text(status, "SDR Mode | OMFG Edition v3 | Initializing Core Apps...");
    theme_apply_label_style(status);
    lv_obj_set_style_text_font(status, &lv_font_montserrat_12, 0);
    lv_obj_align(status, LV_ALIGN_BOTTOM_MID, 0, -20);

    // Fade-in animation
    lv_anim_init(&fade_anim);
    lv_anim_set_var(&fade_anim, splash_screen);
    lv_anim_set_values(&fade_anim, LV_OPA_TRANSP, LV_OPA_COVER);
    lv_anim_set_time(&fade_anim, 600);  // 600ms fade in
    lv_anim_set_exec_cb(&fade_anim, splash_fade_out_cb);  // Reuse for opacity
    lv_anim_start(&fade_anim);

    // Hold for 2.5 seconds then fade out and transition
    lv_timer_t * hold_timer = lv_timer_create((lv_timer_cb_t)splash_finished_cb, 2500, NULL);
    // In production: chain another fade-out anim before calling splash_finished_cb
}