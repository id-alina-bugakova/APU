#define _CRT_SECURE_NO_WARNINGS

#define INFINITE_RUN 0
#define T_MODELING 310
#define N_STEPS (1/H * T_MODELING)
#define SLEEP_TIME 10 

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>

#include "apu_defs.h"
#include "apu_math.h"
#include "apu_components.h"
#include "apu_controller.h"
#include "apu_outer.h"

void manager_cyclic()
{
    Starter start;
    PID pid;
    Gas_generator ggen;
    Rotor rotor;
    Compressor comp;
    Cooling_fan fan;
    Generator gen;
    Fuel_pump pump;
    Pneumatic_system psys;

    init_starter(&start);
    init_PID(&pid);
    init_gas_generator(&ggen);
    init_rotor(&rotor);
    init_compressor(&comp);
    init_cooling_fan(&fan);
    init_generator(&gen);
    init_fuel_pump(&pump);
    init_pneumatic_system(&psys);

    ECU c0, c1;

    init_controller(&c0, 0, true);
    init_controller(&c1, 1, false);
    c0.power = ON;
    c1.power = ON;

    State state = STATE_OFF;
    Event last_event = EVENT_NONE;

    Messages msgs;
    Responses rsps;
    Actions acts;
    Data data;
    Physical phys;

    

    printf("==================================================\n");
    printf("  Press c - input command\n");
    printf("  Press s - start/stop simulation\n");
    printf("==================================================\n\n");

    char key;

    key = _getch();
    while(key != 's')
        key = _getch();

    for (int i = 1; (INFINITE_RUN? (i > 0) : (i <= N_STEPS)); i++)
    {
        fflush(stdout);
        if (_kbhit())
        {
            key = _getch();
            if (key == 's')
                system("pause");
        }

        // Собираем сообщения
        by_eps(&(msgs.eps));
        by_fs(&(msgs.fs));
        by_air_cs(&(msgs.air_cs));
        by_mpu(&(msgs.mpu));
        by_fps(&(msgs.fps));
        by_rcs(&(msgs.rcs));
        by_auto_cs(&(msgs.auto_cs));
        // Получаем физику
        get_physical(&phys);

        // Обновляем контроллеры
        update_controller(
            &c0, &c1, i, state, &last_event, 
            &start, &ggen, &rotor, &comp, &fan, &gen, &pump, &psys, 
            &msgs, &rsps, &acts, &data, &phys);
        update_controller(
            &c1, &c0, i, state, &last_event,
            &start, &ggen, &rotor, &comp, &fan, &gen, &pump, &psys,
            &msgs, &rsps, &acts, &data, &phys);

        // Передаем ответы
        
        // Выполняем действия

        Sleep(SLEEP_TIME);
    }
}

void main()
{
    manager_cyclic();
}