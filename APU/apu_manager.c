#define _CRT_SECURE_NO_WARNINGS

#define T_MODELING 310
#define N_STEPS (1/H * T_MODELING)
#define SLEEP_TIME 2  //2 

#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <string.h>

#include "apu_defs.h"
#include "apu_math.h"
#include "apu_components.h"
#include "apu_controller.h"
#include "apu_outer.h"
#include "apu_driver.h"
#include "apu_interface.h"


void manager_cyclic()
{
    APU apu;
    init_APU(&apu);

    ECU c0, c1;
    init_controller(&c0, 0, true);
    init_controller(&c1, 1, false);

    State state = STATE_OFF;
    Event last_event = EVENT_NONE;

    Messages msgs;
    Responses rsps;
    Actions acts;
    Actions_manual actm;
    Data data;
    Physical phys;
    init_messages(&msgs);
    init_responses(&rsps);
    init_actions(&acts);
    init_actions_manual(&actm);
    init_data(&data);
    init_physical(&phys);

    Output out;
    File_output fout;
    Message_buffer mb;
    init_output(&out);
    init_file_output(&fout);
    init_buffer(&mb);

    // Предыдущие состояния структур для вывода
    APU prev_apu;
    init_APU(&prev_apu);
    State prev_state = STATE_OFF;
    Responses prev_rsps;
    init_responses(&prev_rsps);
    int prev_height = 0;

    printf("============================================================\n");
    printf("  - Press S - select scenario\n");
    printf("  - Press M - start in manual mode (on start only)\n");
    printf("============================================================\n");

    char key;

    key = _getch();
    while(key != 'S' && key != 'M')
        key = _getch();
    
    bool manual_mode = (key == 'M');

    if (manual_mode)
    {
        printf(" === MANUAL MODE ===\n");
        Sleep(250);
    }
    else
    {
        printf("  Select scenario:\n");
    }

    for (int i = 1; 1; i++)
    {
        fflush(stdout);
        printer(
            &out, &mb, &rsps, &prev_rsps, 
            i, phys.height, &prev_height, state, &prev_state, 
            &apu, &prev_apu);
        if (_kbhit())
        {
            key = _getch();
            handle_key_press(key, manual_mode, &out, &mb, state, (!c0.power && !c1.power), &actm);
        }

        // Собираем сообщения
        by_eps(&(msgs.eps), &(rsps.eps), &(apu.gen), (i == 1), 0, actm.gen);
        by_fs(&(msgs.fs), (i == 1), 0);
        by_air_cs(&(msgs.air_cs), &(rsps.air_cs), (i == 1), actm.bleed, 0);
        by_mpu(&(msgs.mpu), &(rsps.mpu), (i == 1), actm.mpu_start, 0);
        by_fps(&(msgs.fps), 0);
        by_rcs(&(msgs.rcs), &(rsps.rcs), state, (i == 1), manual_mode, actm.power, actm.test, 1);
        by_auto_cs(&(msgs.auto_cs), 0);
        // Получаем физику
        get_physical(&phys, 0, 1, 1);

        // Выполняем действия вручную, если ручной режим
        if(manual_mode)
            perform_manual_actions(
                &actm, &msgs, state,
                &c0, &c1,
                &(apu.start), &(apu.ggen), &(apu.rotor), &(apu.comp),
                &(apu.fan), &(apu.gen), &(apu.pump), &(apu.psys));

        // Обновляем контроллеры
        update_controller(
            &c0, &c1, i, &state, 
            &(apu.start), &(apu.ggen), &(apu.rotor), &(apu.comp),
            &(apu.fan), &(apu.gen), &(apu.pump), &(apu.psys),
            &msgs, &rsps, &acts, &data, &phys, &mb);
        update_controller(
            &c1, &c0, i, &state,
            &(apu.start), &(apu.ggen), &(apu.rotor), &(apu.comp),
            &(apu.fan), &(apu.gen), &(apu.pump), &(apu.psys),
            &msgs, &rsps, &acts, &data, &phys, &mb);

        // Передаем ответы
        
        // Выполняем действия
        perform_actions(
            &acts, &msgs, &actm, 
            &(apu.start), &(apu.ggen), &(apu.rotor), &(apu.gen), &(apu.pump), & (apu.psys));

        // Обновляем агрегаты
        update_APU(&apu, c0.power, phys.height);

        // Логируем состояния агрегатов ВСУ
        write_files(&fout, &apu);

        Sleep(SLEEP_TIME);
    }
}

void main()
{
    manager_cyclic();
}