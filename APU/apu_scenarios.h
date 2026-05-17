#pragma once

#define N_SCENARIOS 9

#include <stdbool.h>
#include "apu_defs.h"
#include "apu_communication.h"
#include "apu_controller.h"

typedef struct {
    char path[N_SCENARIOS][STRING_LEN];
    int n;
    long pos;
} Fin_scenario;

typedef struct
{
    bool first_record;
    char path[STRING_LEN];
} Fout_scenario;

void init_fin_scenario(Fin_scenario* fis);

void init_fout_scenario(Fout_scenario* fos);

void write_scenario(
    Fout_scenario* fos, 
    Physical* phys, 
    Actions_manual* actm, 
    bool test,
    bool c0_fault, 
    bool c1_fault, 
    char fault);

void start_scenario(
    Fin_scenario* scns,
    APU* apu,
    Actions_manual* actm,
    Physical* phys,
    ECU* c0,
    ECU* c1);