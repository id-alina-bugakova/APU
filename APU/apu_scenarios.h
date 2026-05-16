#pragma once

#define N_SCENARIOS 9

#include <stdbool.h>
#include "apu_defs.h"

typedef struct {
    char key1;
    char key2;
    char key3;
} Keys;

typedef struct {
    bool power;
    bool test;
    bool start_stop;
    bool bleed;
    bool gen;
    bool mpu;
    bool fire;
    unsigned int height;
    bool ignition;
    bool enough_pres;
    bool c0;
    bool c1;
    char faults;
} Sc_commands;

typedef struct {
    char path[N_SCENARIOS][STRING_LEN];
    long pos;
    bool finished;
} Scenarios;

void init_scenarios(Scenarios* scns);

void start_scenario(int n, Scenarios* scns, Keys* k);