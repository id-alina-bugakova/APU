#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "apu_state_machine.h"

void state_machine_test() 
{
    const char* apu_states[] = APU_STATES;
    const char* apu_events[] = APU_EVENTS;
    int state_machine = STATE_OFF;
    int event = EVENT_POWER_OFF;
    while (1)
    {
        printf("State: %s\n", apu_states[state_machine]);
        scanf("%d", &event);
        state_machine = handle_event(state_machine, event);
        printf("Event: %s\n", apu_events[event]);
    }
}

void main()
{
    state_machine_test();
}