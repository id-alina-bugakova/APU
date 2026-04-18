#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>

#include "apu_defs.h"
#include "apu_math.h"
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

double func(double t) 
{
    return PI * binpow(t, 4) + PI * PI * binpow(t, 3) + t;
}
void math_test()
{
    printf("Answer:  %f\n", binpow(1.6, 19));
    printf("Correct: 7555.786372591447\n\n");

    printf("Answer:  %f\n", parab_integral(func, 0, 10));
    printf("Correct: 87555.86449339830\n\n");
}

void main()
{
    math_test();
}