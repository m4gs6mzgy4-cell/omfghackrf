#ifndef RF_ENGINE_H
#define RF_ENGINE_H

#include <stdint.h>

typedef struct {
    uint64_t freq;
    int duration;
    int mode;
    int8_t *data;
    size_t data_len;
} attack_config_t;

int run_attack(const char *name, attack_config_t *config);

#endif