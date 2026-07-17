/*
 * Xanthorox RF Core - Unified Signal Library & Engine Stub
 * For PortaPack H2 Firmware
 *
 * This provides the shared backend for all core apps:
 * - Capture / Replay database (SD card backed)
 * - Spectrum / Waterfall hooks
 * - Stealth / Low-emission modes
 * - Protocol detection hooks
 *
 * IMPORTANT: Attack-specific logic (brute force algorithms, rolling code bypass,
 * vehicle CAN/keyless, jamming patterns, locker frequencies, etc.) must be
 * integrated here from your trusted sources or existing Mayhem/Havoc code.
 * This stub ensures the UI launches cleanly and RF hardware is initialized.
 */

#ifndef RF_ENGINE_H
#define RF_ENGINE_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t frequency;
    uint32_t sample_rate;
    char protocol_tag[32];
    char capture_name[64];
    // IQ data pointer or file path
} signal_capture_t;

// Initialize HackRF + PortaPack RF frontend
void rf_engine_init(void);

// Unified capture function (called by all apps)
int rf_engine_capture(signal_capture_t *capture, uint32_t duration_ms);

// Replay function
int rf_engine_replay(signal_capture_t *capture, bool stealth_mode);

// Spectrum / waterfall live view (for GUI integration)
void rf_engine_get_spectrum(uint8_t *buffer, int size);

// Stealth controls
void rf_engine_set_stealth_mode(bool enable, uint8_t power_level);
bool rf_engine_is_stealth_active(void);

// App-specific hooks (stubs - implement in rf_engine.c or separate modules)
void rf_engine_proto_pirate_attack(void);
void rf_engine_subghz_brute_attack(void);
void rf_engine_rolljam_attack(void);
void rf_engine_kaiju_analyze(void);
void rf_engine_pandwarf_attack(void);
void rf_engine_amazon_locker_attack(void);
void rf_engine_cctv_jammer_start(void);

#endif // RF_ENGINE_H