#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "apu_components.h"
#include "apu_controller.h"
#include "apu_interface.h"

typedef struct {
    uint32_t tolerance;
    bool initiated_shutdown;
} Watchdog;

void init_watchdog(Watchdog* wd);

// ¬озвращает 1, если хот€ бы один канал работает
void update_watchdog(
    Watchdog* wd,
    uint32_t cur_time,
    ECU* c0,
    ECU* c1,
    Actions* acts,
    Message_buffer* mb,
    File_output* fout);