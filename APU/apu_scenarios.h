#pragma once

#define N_SCENARIOS 8

#include "apu_interface.h"
#include "apu_controller.h"

typedef struct {
    char path[N_SCENARIOS][STRING_LEN];
    void (*func[N_SCENARIOS])(
        Actions_manual* actm,
        Messages* msgs,
        State state,
        ECU* c0,
        ECU* c1,
        APU* apu);
} Scenarios;