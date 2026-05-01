#include "apu_controller.h"
#include "apu_defs.h"
#include "apu_state_machine.h"
#include "apu_communtication.h"

void init_controller(ECU* contr, int id, bool main_channel)
{
    contr->id           = id;
    contr->power        = OFF;
    contr->main_channel = main_channel;
    contr->fault        = 0;
    contr->last_updated = 0;
    contr->tolerance    = 4;
}

void update_controller(
    ECU* c1,
    ECU* c2,
    uint32_t cur_time,
    State state,
    Event last_event,
    uint32_t last_event_time,
    Starter* start, 
    Gas_generator* ggen, 
    Rotor* rotor, 
    Compressor* comp, 
    Cooling_fan* fan, 
    Generator* gen, 
    Fuel_pump* pump, 
    Pneumatic_system* psys,
    Messages* msgs,
    Responses* rsps)
{
    // ≈сли контроллер работает на втором канале
    if (!c1->fault && !c1->main_channel)
    {
        // ≈сли по какой-то причине второй контроллер не главный канал, перехватываем управление
        if (!c2->main_channel)
            c1->main_channel = 1;

        // ѕроверка необходимости перехвата управлени€ по превышению tolerance
        if (cur_time - c2->last_updated > c1->tolerance)
            c1->main_channel = 1;

        if(c1->main_channel)
            printf("Controller %d at %7.2f : Intercepted control\n", c1->id, TIME(cur_time));
    }

    // ≈сли контроллер работает на главном канале
    if (!c1->fault && c1->main_channel)
    {
        switch (state)
        {
        case STATE_OFF:
            if (c1->power) 
            {
                printf("Controller %d at %7.2f : APU turned ON\n", c1->id, TIME(cur_time));
                last_event = EVENT_POWER_ON;
                update_starter(start, 1, rotor);

            }
            
            break;

        case STATE_IDLE:

        }
    }

    // ≈сли c1->fault, то контроллер завис, ничего не обновл€етс€
}
