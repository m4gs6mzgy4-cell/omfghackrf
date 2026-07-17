#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>
#include <hackrf.h>
#include <math.h>
#include <time.h>

#define MAX_ATTACKS   16
#define SAMPLE_RATE   2000000
#define DEFAULT_FREQ  433920000
#define BUFFER_SIZE   262144

typedef struct {
    uint64_t freq;
    int duration;
    int mode;
    int8_t *data;
    size_t data_len;
} attack_config_t;

typedef int (*attack_func_t)(attack_config_t *config);

typedef struct {
    const char *name;
    const char *desc;
    attack_func_t func;
} attack_t;

int attack_replay(attack_config_t *config);
int attack_jam(attack_config_t *config);
int attack_sniff(attack_config_t *config);
int attack_inject(attack_config_t *config);
int attack_modulate(attack_config_t *config);
int attack_sweep(attack_config_t *config);
int attack_chirp(attack_config_t *config);
int attack_pulse(attack_config_t *config);
int attack_noise(attack_config_t *config);
int attack_decode(attack_config_t *config);
int attack_encode(attack_config_t *config);
int attack_scan(attack_config_t *config);
int attack_record(attack_config_t *config);
int attack_bruteforce(attack_config_t *config);
int attack_rolling_code(attack_config_t *config);
int attack_vehicle_can(attack_config_t *config);
int attack_locker(attack_config_t *config);
int attack_cctv(attack_config_t *config);

attack_t attacks[] = {
    {"replay",    "Replay a captured signal", attack_replay},
    {"jam",       "Jam a frequency with noise", attack_jam},
    {"sniff",     "Sniff and log RF signals", attack_sniff},
    {"inject",    "Inject custom signals", attack_inject},
    {"modulate",  "Modulate signals", attack_modulate},
    {"sweep",     "Sweep frequencies", attack_sweep},
    {"chirp",     "Send chirp signals", attack_chirp},
    {"pulse",     "Send pulse signals", attack_pulse},
    {"noise",     "Send noise signals", attack_noise},
    {"decode",    "Decode signals", attack_decode},
    {"encode",    "Encode signals", attack_encode},
    {"scan",      "Scan frequencies for activity", attack_scan},
    {"record",    "Record signals to a file", attack_record},
    {"bruteforce","Brute-force signals", attack_bruteforce},
    {"rolling_code", "Rolling code attack", attack_rolling_code},
    {"vehicle_can", "Vehicle CAN attack", attack_vehicle_can},
    {"locker", "Locker attack", attack_locker},
    {"cctv", "CCTV attack", attack_cctv},
};
int attack_count = sizeof(attacks)/sizeof(attack_t);

int attack_replay(attack_config_t *config) {
    hackrf_device *dev = NULL;
    FILE *record_fp;
    static int done;
    static int8_t *buf_mem;

    printf("[Replay] Recording %d seconds at %llu Hz...\n", config->duration, config->freq);
    record_fp = fopen("capture.iq", "wb");
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    done = 0;
    hackrf_start_rx(dev, (hackrf_sample_block_cb_fn)record_fp, NULL);
    sleep(config->duration);
    done = 1;
    hackrf_stop_rx(dev);
    hackrf_close(dev); hackrf_exit();
    fclose(record_fp);

    printf("[Replay] Transmitting at %llu Hz...\n", config->freq);
    FILE *f = fopen("capture.iq", "rb");
    buf_mem = malloc(BUFFER_SIZE);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    while(1) {
        size_t cnt = fread(buf_mem, 1, BUFFER_SIZE, f);
        if(cnt == 0) break;
        hackrf_start_tx(dev, NULL, NULL);
        usleep(250000);
        hackrf_stop_tx(dev);
    }
    hackrf_close(dev); hackrf_exit();
    fclose(f); free(buf_mem);
    return 0;
}

int attack_jam(attack_config_t *config) {
    hackrf_device *dev = NULL;

    printf("[Jam] Jamming %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_sniff(attack_config_t *config) {
    hackrf_device *dev = NULL;

    printf("[Sniff] Sniffing %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_rx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_rx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_inject(attack_config_t *config) {
    hackrf_device *dev = NULL;

    printf("[Inject] Injecting signal at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_modulate(attack_config_t *config) {
    hackrf_device *dev = NULL;
    static float phase = 0;

    printf("[Modulate] Modulating signal at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_sweep(attack_config_t *config) {
    hackrf_device *dev = NULL;
    static uint64_t current_freq = config->freq;

    printf("[Sweep] Sweeping from %llu to %llu Hz for %d sec\n", config->freq, config->freq + 1000000, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_chirp(attack_config_t *config) {
    hackrf_device *dev = NULL;
    static float phase = 0;
    static float freq = 1000;

    printf("[Chirp] Sending chirp at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_pulse(attack_config_t *config) {
    hackrf_device *dev = NULL;
    static int pulse = 1;

    printf("[Pulse] Sending pulse at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_noise(attack_config_t *config) {
    hackrf_device *dev = NULL;

    printf("[Noise] Sending noise at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_decode(attack_config_t *config) {
    hackrf_device *dev = NULL;
    static int bit = 0;

    printf("[Decode] Decoding signal at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_rx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_rx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_encode(attack_config_t *config) {
    hackrf_device *dev = NULL;
    static int bit = 0;

    printf("[Encode] Encoding signal at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_scan(attack_config_t *config) {
    hackrf_device *dev = NULL;
    static uint64_t current_freq = config->freq;

    printf("[Scan] Scanning from %llu to %llu Hz for %d sec\n", config->freq, config->freq + 1000000, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_rx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_rx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_record(attack_config_t *config) {
    hackrf_device *dev = NULL;

    printf("[Record] Recording signal at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_rx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_rx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_bruteforce(attack_config_t *config) {
    hackrf_device *dev = NULL;
    static int code = 0;

    printf("[BruteForce] Brute-forcing signal at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_rolling_code(attack_config_t *config) {
    hackrf_device *dev = NULL;
    static int code = 0;

    printf("[RollingCode] Attacking rolling code at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_vehicle_can(attack_config_t *config) {
    hackrf_device *dev = NULL;

    printf("[VehicleCAN] Attacking vehicle CAN at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_locker(attack_config_t *config) {
    hackrf_device *dev = NULL;

    printf("[Locker] Attacking locker at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int attack_cctv(attack_config_t *config) {
    hackrf_device *dev = NULL;

    printf("[CCTV] Attacking CCTV at %llu Hz for %d sec\n", config->freq, config->duration);
    hackrf_init();
    hackrf_open(&dev);
    hackrf_set_freq(dev, config->freq);
    hackrf_set_sample_rate(dev, SAMPLE_RATE);
    hackrf_start_tx(dev, (hackrf_sample_block_cb_fn)config->data, NULL);
    sleep(config->duration);
    hackrf_stop_tx(dev);
    hackrf_close(dev); hackrf_exit();
    return 0;
}

int run_attack(const char *name, attack_config_t *config) {
    for(int i=0; i<attack_count; ++i) {
        if(strcmp(attacks[i].name, name) == 0) {
            return attacks[i].func(config);
        }
    }
    return -1;
}