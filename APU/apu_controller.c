#include "apu_controller.h"
#include "apu_defs.h"
#include "apu_state_machine.h"

void init_controller(ECU* contr, int id, bool main_channel)
{
    contr->id              = id;
    contr->power           = OFF;
    contr->main_channel    = main_channel;
    contr->fault           = 0;
    contr->last_updated    = 0;
    contr->tolerance       = 4;

    contr->problems_found  = 0;
    contr->fire            = 0;
}

void test_components(
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Cooling_fan* fan,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys,
    Messages* msgs,
    Data* data)
{
    /* Критические отказы: датчика пламени, двух датчиков оборотов, двух каналов EGT,
    *  датчиков температуры/давления за компрессором, клапана подачи топлива или его
    *  датчика, клапана забора атмосферного воздуха (если закрыт) или его датчика,
    *  клапана отбора воздуха в пневмосистему или его датчика, перекрестного клапана
    *  СКВ/МСУ (если открыт на СКВ - 1) или его датчика, перекрестного клапана МСУ или
    *  его датчика. Также учитывается пожар.
    */
    if (msgs->fps.fire_sig ||
        ggen->flame_sensor.fault ||
        (rotor->N1_0.fault && rotor->N1_1.fault) ||
        (rotor->EGT_A0.fault && rotor->EGT_A1.fault &&
            rotor->EGT_B0.fault && rotor->EGT_B1.fault) ||
        comp->P3.fault || comp->T3.fault ||
        pump->fuel_sov.fault || pump->fuel_sov.sensor.fault ||
        (psys->asv.fault && psys->asv.open == 0) || psys->asv.sensor.fault ||
        psys->bsv.fault || psys->bsv.sensor.fault ||
        (psys->fcv.fault && psys->fcv.open == 1) || psys->fcv.sensor.fault ||
        psys->mpu_xbleed.fault || psys->mpu_xbleed.sensor.fault)
        data->critical_fault = 1;
    /* Некритические отказы: одного из датчиков вращения ротора, одного или нескольких датчиков EGT
    *  (не всех), вентилятора отсека, датчика частоты вращения генератора, датчика давления 
    *  топлива, клапана забора атмосферного воздуха (если открыт), датчиков температуры/давления за
    *  бортом, перекрестного клапана СКВ/МСУ (если открыт на МСУ - 0), датчиков
    *  температуры/давления на входе в пневмосистему.
    *  Параметр apu_fault сигнализирует о всех отказах, включая критические.
    */
    if (data->critical_fault ||
        rotor->N1_0.fault || rotor->N1_1.fault ||
        rotor->EGT_A0.fault || rotor->EGT_A1.fault ||
        rotor->EGT_B0.fault || rotor->EGT_B1.fault ||
        fan->fault ||
        gen->NGC.fault ||
        pump->P_fuel.fault ||
        (psys->asv.fault && psys->asv.open == 1) ||
        psys->T2.fault || psys->P2.fault ||
        (psys->fcv.fault && psys->fcv.open == 0) ||
        psys->T_duct.fault || psys->P_duct.fault)
        data->fault = 1;
}

double calculate_EGT(Rotor* rotor)
{
    /* Вычисляем EGT : если один датчик в канале сломан, то берется значение другого,
    *  иначе среднее арифметическое. Если один канал неактивен, то берем показания
    *  другого, иначе среднее арифметическое.
    */
    double EGT_A, EGT_B;
    if (!rotor->EGT_A0.fault && !rotor->EGT_A1.fault)
        EGT_A = (rotor->EGT_A0.value + rotor->EGT_A1.value) / 2;
    else if (!rotor->EGT_A0.fault)
        EGT_A = rotor->EGT_A0.value;
    else if (!rotor->EGT_A1.fault)
        EGT_A = rotor->EGT_A1.value;
    else
        EGT_A = -1;
    if (!rotor->EGT_B0.fault && !rotor->EGT_B1.fault)
        EGT_B = (rotor->EGT_B0.value + rotor->EGT_B1.value) / 2;
    else if (!rotor->EGT_B0.fault)
        EGT_B = rotor->EGT_B0.value;
    else if (!rotor->EGT_B1.fault)
        EGT_B = rotor->EGT_B1.value;
    else
        EGT_B = -1;
    if (DOUBLE_EQUALS(EGT_A, -1, COMP_CONST) && DOUBLE_EQUALS(EGT_B, -1, COMP_CONST))
        return (EGT_A + EGT_B) / 2;
    else if (DOUBLE_EQUALS(EGT_A, -1, 1e-12))
        return EGT_A;
    else if (DOUBLE_EQUALS(EGT_B, -1, 1e-12))
        return EGT_B;
    else
        return -1;
}

double calculate_N(Rotor* rotor)
{
    if (!rotor->N1_0.fault && !rotor->N1_1.fault)
        return (rotor->N1_0.value + rotor->N1_1.value) / 2;
    else if (!rotor->N1_0.fault)
        return rotor->N1_0.value;
    else if (!rotor->N1_1.fault)
        return rotor->N1_1.value;
    else
        return -1;
}

void check_N_EGT(uint32_t cur_time, Rotor* rotor, Responses* rsps, Data* data)
{
    // Вычисляем показатели ротора
    data->EGT_cur = calculate_EGT(rotor);
    data->N_cur = calculate_N(rotor);
    // Передаем СДУ
    rsps->rcs.N1 = data->N_cur;
    rsps->rcs.EGT = data->EGT_cur;
    // Фиксируем перегрев или его отсутствие
    if (data->EGT_cur > EGT_LIMIT && !data->ovheat)
    {
        data->ovheat = 1;
        data->last_ovheat_detected = cur_time;
    }
    else if (!data->ovheat)
        data->ovheat = 0;
}

void check_for_emergencies(
    ECU* c1,
    uint32_t cur_time,
    Event* last_event,
    Rotor* rotor,
    Gas_generator* ggen,
    Responses* rsps,
    Data* data)
{
    // Обработка аварийных состояний
    if (data->EGT_cur > EGT_LIMIT &&
        TIME(cur_time) > TIME(data->last_ovheat_detected) + OVHEAT_TIME_LIMIT)
    {
        printf("Controller %d at %7.2f : Warning: EGT over limit.\n",
            c1->id, TIME(cur_time));
        last_event = EVENT_OVHEAT_DETECTED;
        rotor->N_target = 0; // Остановка с отсечкой топлива
        rsps->fs.fuel_demand = 0;
        data->demanded_fuel = 0;
        data->emergency_found = 1;
    }
    else if (data->N_cur > N_LIMIT)
    {
        printf("Controller %d at %7.2f : Warning: rotor overspeed.\n",
            c1->id, TIME(cur_time));
        last_event = EVENT_OVSPEED_DETECTED;
        rotor->N_target = 0; // Остановка с отсечкой топлива
        rsps->fs.fuel_demand = 0;
        data->demanded_fuel = 0;
        data->emergency_found = 1;
    }
    else if (data->ignited && !ggen->flame_sensor.value)
    {
        printf("Controller %d at %7.2f : Warning: flame went out.\n",
            c1->id, TIME(cur_time));
        last_event = EVENT_FLAME_WENT_OUT;
        rotor->N_target = 0; // Остановка с отсечкой топлива
        rsps->fs.fuel_demand = 0;
        data->demanded_fuel = 0;
        data->emergency_found = 1;
    }
}

void check_for_emergencies_emergent(
    ECU* c1,
    uint32_t cur_time,
    Event* last_event,
    Rotor* rotor,
    Gas_generator* ggen,
    Messages* msgs,
    Responses* rsps,
    Data* data)
{
    // Единственная значимая проблема это погасание факела 
    if (data->ignited && !ggen->flame_sensor.value)
    {
        printf("Controller %d at %7.2f : Warning: flame went out.\n",
            c1->id, TIME(cur_time));
        last_event = EVENT_FLAME_WENT_OUT;
        data->emergency_found = 1;
    }
    // О других проблемах предупреждаем
    // Возникновение пожара на данном такте
    if (msgs->fps.fire_sig && !data->fire)
        printf("Controller %d at %7.2f : Warning: fire detected.\n",
            c1->id, TIME(cur_time));
    if (data->EGT_cur > EGT_LIMIT &&
        TIME(cur_time) > TIME(data->last_ovheat_detected) + OVHEAT_TIME_LIMIT)
        printf("Controller %d at %7.2f : Warning: EGT over limit.\n",
            c1->id, TIME(cur_time));
    else if (data->N_cur > N_LIMIT)
        printf("Controller %d at %7.2f : Warning: rotor overspeed.\n",
            c1->id, TIME(cur_time));
}

void update_controller(
    ECU* c1,
    ECU* c2,
    uint32_t cur_time,
    State state,
    Event last_event,
    Starter* start, 
    Gas_generator* ggen, 
    Rotor* rotor, 
    Compressor* comp, 
    Cooling_fan* fan, 
    Generator* gen, 
    Fuel_pump* pump, 
    Pneumatic_system* psys,
    Messages* msgs,
    Responses* rsps,
    Actions* acts,
    Data* data,
    Physical* phys)
{

    // Если контроллер работает на втором канале
    if (!c1->fault && !c1->main_channel)
    {
        // Если по какой-то причине второй контроллер не главный канал, перехватываем управление
        if (!c2->main_channel)
            c1->main_channel = 1;

        // Проверка необходимости перехвата управления по превышению tolerance
        if (cur_time - c2->last_updated > c1->tolerance)
            c1->main_channel = 1;

        if(c1->main_channel)
            printf("Controller %d at %7.2f : Intercepted control.\n", c1->id, TIME(cur_time));
    }

    // Если контроллер работает на главном канале
    if (!c1->fault && c1->main_channel)
    {
        init_responses(rsps);
        init_actions(acts);
        if (state != STATE_OFF)
            rsps->rcs.power_on = 1;

        // Индикация пожара кроме аварийных состояний
        if (state != STATE_OFF && msgs->fps.fire_sig &&
            state != STATE_IDLE && // Рассматривается отдельно
            state != STATE_EMERGENCY_START && state != STATE_IDLE_RUN_LIMITED && // Аварийные
            state != STATE_GEN_LIMITED && state != STATE_MPU_START_LIMITED &&
            state != STATE_APU_FIRE && state != STATE_EMERGENCY_SHUTDOWN) 
        {
            printf("Controller %d at %7.2f : Warning: fire detected.\n", c1->id, TIME(cur_time));
            last_event = EVENT_FIRE_DETECTED;
            data->fire = 1;
        }
        else
        {
            switch (state)
            {
            case STATE_OFF:
                // Если подано питание и получено сообщение о подаче питания И не пожар
                if (c1->power && msgs->rcs.apu_power && !msgs->fps.fire_sig)
                {
                    printf("Controller %d at %7.2f : APU turned ON.\n", c1->id, TIME(cur_time));
                    last_event = EVENT_POWER_ON;
                }
                break;

            case STATE_IDLE:
                rsps->rcs.power_on = 1;
                if (!msgs->rcs.apu_power)
                {
                    printf("Controller %d at %7.2f : Turning OFF.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_POWER_OFF;
                }
                if (msgs->auto_cs.auto_start)
                {
                    if (c1->fire)
                    {
                        printf("Controller %d at %7.2f : Unable to auto start due to fire.\n",
                            c1->id, TIME(cur_time));
                        last_event = EVENT_NONE;
                    }
                    else if (phys->height >= CRITICAL_HEIGHT)
                    {
                        printf("Controller %d at %7.2f : Unable to auto start due to height.\n",
                            c1->id, TIME(cur_time));
                        last_event = EVENT_NONE;
                    }
                    else if (!msgs->fs.fuel_avail || msgs->fs.low_pres_warn)
                    {
                        printf("Controller %d at %7.2f : Unable to auto start due to fuel issues.\n",
                            c1->id, TIME(cur_time));
                        last_event = EVENT_NONE;
                    }
                    else if (data->critical_fault)
                    {
                        printf(
                            "Controller %d at %7.2f : Unable to auto start due to critical fault.\n",
                            c1->id, 
                            TIME(cur_time));
                        last_event = EVENT_NONE;
                    }
                    else
                    {
                        printf("Controller %d at %7.2f : Auto starting...\n", 
                            c1->id, TIME(cur_time));
                        last_event = EVENT_AUTO_START_CMD;
                        data->last_start_cmd = cur_time;
                    }
                }
                else if (msgs->rcs.start_cmd)
                {
                    // При пожаре, превышении допустимой высоты или отсутстствии топлива
                    // выполняется аварийный запуск
                    if (c1->fire || phys->height >= CRITICAL_HEIGHT ||
                        !msgs->fs.fuel_avail || msgs->fs.low_pres_warn ||
                        data->critical_fault)
                    {
                        printf("Controller %d at %7.2f : Emergency starting...\n",
                            c1->id, TIME(cur_time));
                        last_event = EVENT_EMERGENCY_START_CMD;
                    }
                    else
                    {
                        printf("Controller %d at %7.2f : Starting...\n", c1->id, TIME(cur_time));
                        last_event = EVENT_START_CMD;
                    }
                    data->last_start_cmd = cur_time;
                }
                else if (msgs->rcs.test)
                {
                    printf("Controller %d at %7.2f : Initiating testing...\n", 
                        c1->id, TIME(cur_time));
                    last_event = EVENT_TEST_START_CMD;
                }
                break;

            case STATE_TEST:
                rsps->rcs.power_on = 1;
                test_components(ggen, rotor, comp, fan, gen, pump, psys, msgs, data);
                rsps->rcs.critical_fault = data->critical_fault;
                rsps->rcs.apu_fault = data->fault;
                printf("Controller %d at %7.2f : Testing completed. Problems found: ",
                    c1->id, TIME(cur_time));
                if (rsps->rcs.apu_fault)
                    printf("YES");
                else
                    printf("NO");
                printf("; critical problems found: ");
                if (rsps->rcs.critical_fault)
                    printf("YES");
                else
                    printf("NO");
                printf(".\n");
                last_event = EVENT_TEST_COMPLETED;
                break;

            case STATE_START:              // Основной алгоритм
            case STATE_AUTO_START:         // Нет ручного прерывания старта
            case STATE_EMERGENCY_START:    // Только ручное прерывание старта
                rsps->rcs.power_on = 1;
                rsps->rcs.start = 1;
                check_N_EGT(cur_time, rotor, rsps, data);
                /* Варианты развития событий, приводящие к отмене запуска: запуск прерван, 
                *  превышено макс. время запуска, перегрев дольше OVHEAT_TIME_LIMIT с,
                *  разнос ротора, погасание факела в камере сгорания
                */
                if (state != STATE_AUTO_START && msgs->rcs.stop_cmd)
                {
                    printf("Controller %d at %7.2f : Start aborted.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_START_ABORTED;
                    data->starting = 0;
                    rsps->rcs.start = 0;
                    // В нормальном режиме остановка без отсечки, в аварийном - с
                    rotor->N_target = 0;
                    if (state == STATE_EMERGENCY_START)
                    {
                        rsps->fs.fuel_demand = 0;
                        data->demanded_fuel = 0;
                    }
                }
                else if (TIME(cur_time) > TIME(data->last_start_cmd) + MAX_START_TIME)
                {
                    if (state != STATE_EMERGENCY_START)
                    {
                        printf("Controller %d at %7.2f : Start failure: start time exceeded.\n",
                            c1->id, TIME(cur_time));
                        last_event = EVENT_START_FAILURE;
                        data->starting = 0;
                        rsps->rcs.start = 0;
                        rotor->N_target = 0; // Остановка без немедленной отсечки топлива
                    }
                    else
                        printf("Controller %d at %7.2f : Warning: start time exceeded.\n",
                            c1->id, TIME(cur_time));
                }
                else if (data->EGT_cur > EGT_LIMIT &&
                    TIME(cur_time) > TIME(data->last_ovheat_detected) + OVHEAT_TIME_LIMIT)
                {
                    if (state != STATE_EMERGENCY_START)
                    {
                        printf("Controller %d at %7.2f : Start failure: EGT over limit.\n",
                            c1->id, TIME(cur_time));
                        last_event = EVENT_OVHEAT_DETECTED;
                        data->starting = 0;
                        rsps->rcs.start = 0;
                        rotor->N_target = 0; // Остановка с отсечкой топлива
                        rsps->fs.fuel_demand = 0;
                        data->demanded_fuel = 0;
                    }
                    else
                        printf("Controller %d at %7.2f : Warning: EGT over limit.\n",
                            c1->id, TIME(cur_time));
                }
                else if (data->N_cur > N_LIMIT)
                {
                    if (state != STATE_EMERGENCY_START)
                    {
                        printf("Controller %d at %7.2f : Start failure: rotor overspeed.\n",
                            c1->id, TIME(cur_time));
                        last_event = EVENT_OVSPEED_DETECTED;
                        data->starting = 0;
                        rsps->rcs.start = 0;
                        rotor->N_target = 0; // Остановка с отсечкой топлива
                        rsps->fs.fuel_demand = 0;
                        data->demanded_fuel = 0;
                    }
                    else
                        printf("Controller %d at %7.2f : Warning: rotor overspeed.\n",
                            c1->id, TIME(cur_time));
                }
                else if (data->ignited && !ggen->flame_sensor.value) 
                {
                    if (state != STATE_EMERGENCY_START)
                    {
                        printf("Controller %d at %7.2f : Start failure: flame went out.\n",
                            c1->id, TIME(cur_time));
                        data->starting = 0;
                        rsps->rcs.start = 0;
                        rotor->N_target = 0; // Остановка с отсечкой топлива
                        rsps->fs.fuel_demand = 0;
                        data->demanded_fuel = 0;
                    }
                    else
                    {
                        // При аварийном запуске допустимо перезажигание факела
                        printf("Controller %d at %7.2f : Warning: flame went out.\n",
                            c1->id, TIME(cur_time));
                    }
                    last_event = EVENT_FLAME_WENT_OUT;
                }
                // Корректное завершение запуска: малые обороты набраны
                else if (data->N_cur >= rotor->N_idle - N_COMP_CONST)
                {
                    printf("Controller %d at %7.2f : Start success.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_START_SUCCESS;
                    data->starting = 0;
                    rsps->rcs.start = 0;      // Сообщаем о включении
                    rsps->fs.fuel_demand = 1; // Продолжаем потреблять топливо
                    // Сообщаем о готовности к нагрузке
                    rsps->eps.power_avail = 1;
                    rsps->mpu.apu_bleed_avail = 1;
                    rsps->air_cs.apu_bleed_avail = 1;
                }
                // Если в начале запуска
                else if (!data->starting)
                {
                    printf("Controller %d at %7.2f : Start initiated.\n",
                        c1->id, TIME(cur_time));
                    data->starting = 1;
                    data->last_start_cmd = cur_time;
                    data->demanded_fuel = 0;
                    acts->turn_on_cmd = 1;
                    acts->set_N = 1;
                    acts->N_target = rotor->N_idle;
                    acts->open_asv = 1;
                }
                // Если достигли оборотов начала подачи топлива
                else if (data->N_cur >= ggen->fuel_N - N_COMP_CONST)
                {
                    // Запрашиваем толпиво и ждем
                    rsps->fs.fuel_demand = 1;
                    if (!pump->on)
                    {
                        /* Если запрос топлива прошел давно или нет топлива / низкое давление, то 
                        * отказ в запуске (кроме аварийного запуска)
                        */ 
                        if (state != STATE_EMERGENCY_START && 
                            ((data->demanded_fuel &&
                            TIME(cur_time) > TIME(data->last_demanded_fuel) + FUEL_TIME_LIMIT) ||
                            !msgs->fs.fuel_avail || msgs->fs.low_pres_warn))
                        {
                            printf("Controller %d at %7.2f : Start failure: start time exceeded.\n",
                                c1->id, TIME(cur_time));
                            last_event = EVENT_START_FAILURE;
                            data->starting = 0;
                            rsps->rcs.start = 0;
                            rotor->N_target = 0; // Остановка без немедленной отсечки топлива
                        }
                        data->demanded_fuel = 1;
                        data->last_demanded_fuel = cur_time;
                    }
                    // Иначе газогенератор уже работает
                    else
                    {
                        data->demanded_fuel = 0;
                        // Включаем зажигание
                        acts->ignite = 1;
                        data->ignited = 1;
                    }
                }
                // Иначе ожидаем раскрутки ротора
                break;

            case STATE_IDLE_RUN:
                // Остаемся в STATE_IDLE_RUN до тех пор, пока не выйдем на рабочие обороты
                rsps->rcs.power_on = 1;
                rsps->fs.fuel_demand = 1;
                check_N_EGT(cur_time, rotor, rsps, data);
                data->emergency_found = 0;
                check_for_emergencies(c1, cur_time, &last_event, rotor, ggen, rsps, data);
                // Обработка команды выключения 
                if (!data->emergency_found && msgs->rcs.stop_cmd)
                {
                    printf("Controller %d at %7.2f : Shutdown command recieved.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_NORMAL_SHUTDOWN_CMD;
                    data->starting = 0;
                }
                // Высший приоритет имеет команда запуска МСУ, затем генератор и отбор воздуха
                // Команды выполняются, если не принята другая и если нет аварии
                // Поскольку остаемся в состоянии до выхода на обороты, событие EVENT_NONE
                else if (!data->starting && !data->emergency_found && msgs->mpu.eng_start)
                {
                    printf("Controller %d at %7.2f : MPU demanded air bleed.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_NONE;
                    data->starting = 1;
                    data->last_start_cmd = cur_time;
                    acts->set_N = 1;
                    acts->N_target = rotor->N_work;
                }
                // Если есть запрос на генератор и нет наземного питания
                else if (!data->starting && !data->emergency_found
                    && msgs->eps.gen_switch && !msgs->eps.ext_power_avail)
                {
                    printf("Controller %d at %7.2f : EPS demanded generator.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_NONE;
                    data->starting = 1;
                    data->last_start_cmd = cur_time;
                    acts->set_N = 1;
                    acts->N_target = rotor->N_work;
                }
                else if (!data->starting && !data->emergency_found 
                    && msgs->air_cs.bleed_air_demand)
                {
                    printf("Controller %d at %7.2f : ACS demanded air bleed.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_NONE;
                    data->starting = 1;
                    data->last_start_cmd = cur_time;
                    acts->set_N = 1;
                    acts->N_target = rotor->N_work;
                }
                // Если нет команд, сообщаем о готовности к нагрузке
                else if (!data->starting && !data->emergency_found)
                {
                    rsps->eps.power_avail = 1;
                    rsps->mpu.apu_bleed_avail = 1;
                    rsps->air_cs.apu_bleed_avail = 1;
                }
                // Вышли на рабочие обороты
                else if (data->starting && !data->emergency_found && 
                    data->N_cur >= rotor->N_work - N_COMP_CONST2)
                {
                    if (msgs->mpu.eng_start)
                    {
                        last_event = EVENT_MPU_ON_CMD;
                        acts->update_air_demand = 1;
                        acts->open_bsv = 1;
                        acts->switch_fcv_to_mpu = 1;
                    }
                    else if (msgs->eps.gen_switch)
                    {
                        last_event = EVENT_GEN_ON_CMD;
                        acts->update_gen_demand = 1;
                        acts->gen_demand = 1;
                    }
                    else if (msgs->air_cs.bleed_air_demand)
                    {
                        last_event = EVENT_BLEED_ON_CMD;
                        acts->update_air_demand = 1;
                        acts->open_bsv;
                        acts->switch_fcv_to_acs = 1;
                    }
                    printf("Controller %d at %7.2f : Full rotation speed reached.\n",
                        c1->id, TIME(cur_time));
                    data->starting = 0;
                }
                // Иначе ждем выхода ротора на обороты
                break;

            case STATE_BLEED:
                rsps->rcs.power_on = 1;
                rsps->fs.fuel_demand = 1;
                check_N_EGT(cur_time, rotor, rsps, data);
                data->emergency_found = 0;
                check_for_emergencies(c1, cur_time, &last_event, rotor, ggen, rsps, data);
                // Остановка - в начале, т.к. начатая приоритетнее других команд
                if (data->starting && !data->emergency_found && 
                    DOUBLE_EQUALS(data->N_cur, rotor->N_idle, N_COMP_CONST))
                {
                    printf("Controller %d at %7.2f : Returned to low speed.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_BLEED_OFF_CMD;
                    data->starting = 0;
                }
                // Вторая по приоритетности команда - включение МСУ
                else if (!data->starting && !data->emergency_found && msgs->mpu.eng_start)
                {
                    printf("Controller %d at %7.2f : Switched to MPU air bleed.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_MPU_ON_CMD;
                    // Обновляем требование воздуха
                    acts->update_air_demand = 1;
                    acts->switch_fcv_to_mpu = 1;
                }
                // Добавление генератора
                else if (!data->starting && !data->emergency_found &&
                    msgs->eps.gen_switch && !msgs->eps.ext_power_avail)
                {
                    printf("Controller %d at %7.2f : Generator turned ON.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_GEN_ON_CMD;
                    // Подключаем генератор
                    acts->update_gen_demand = 1;
                    acts->gen_demand = 1;
                }
                // Если больше не требуется ВСУ или высокое давление в пневмосети - в конце, т.к.
                // лучше не сбрасывать обороты.
                // Также остаемся в режиме до тех пор, пока не остановимся
                else if (!data->starting && !data->emergency_found &&
                    (!msgs->air_cs.bleed_air_demand || msgs->air_cs.duct_pres))
                {
                    printf("Controller %d at %7.2f : Returning to low speed...\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_NONE;
                    // Отключение подачи воздуха
                    acts->update_air_demand = 1;
                    acts->close_bsv = 1;
                    // Задача малых оборотов
                    data->starting = 1;
                    data->last_start_cmd = cur_time;
                    acts->set_N = 1;
                    acts->N_target = rotor->N_idle;
                }
                if (!data->starting && !data->emergency_found)
                {
                    rsps->eps.power_avail = 1;
                    rsps->mpu.apu_bleed_avail = 1;
                    rsps->air_cs.apu_bleed_avail = 1;
                }
                break;

            case STATE_GEN:
                rsps->rcs.power_on = 1;
                rsps->fs.fuel_demand = 1;
                check_N_EGT(cur_time, rotor, rsps, data);
                data->emergency_found = 0;
                check_for_emergencies(c1, cur_time, &last_event, rotor, ggen, rsps, data);
                // Остановка - в начале, т.к. начатая приоритетнее других команд
                if (data->starting && !data->emergency_found &&
                    DOUBLE_EQUALS(data->N_cur, rotor->N_idle, N_COMP_CONST))
                {
                    printf("Controller %d at %7.2f : Returned to low speed.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_GEN_OFF_CMD;
                    data->starting = 0;
                }
                // Вторая по приоритетности команда - включение МСУ
                else if (!data->starting && !data->emergency_found && msgs->mpu.eng_start)
                {
                    printf("Controller %d at %7.2f : Switched to MPU air bleed.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_MPU_ON_CMD;
                    // Требование воздуха + убираем генератор
                    acts->update_gen_demand = 1;
                    acts->gen_demand = 0;
                    acts->update_air_demand = 1;
                    acts->open_bsv;
                    acts->switch_fcv_to_mpu;
                }
                else if (!data->starting && !data->emergency_found
                    && msgs->air_cs.bleed_air_demand)
                {
                    printf("Controller %d at %7.2f : ACS bleed turned ON.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_BLEED_ON_CMD;
                    // Требование воздуха 
                    acts->update_air_demand = 1;
                    acts->open_bsv;
                    acts->switch_fcv_to_acs;
                }
                // Если больше не требуется ВСУ - в конце, т.к. лучше не сбрасывать обороты
                // Также остаемся в режиме до тех пор, пока не остановимся
                // Если задано выключить генератор или СЭС не получает питание от него, то отключаем
                else if (!data->starting && !data->emergency_found 
                    && (!msgs->eps.gen_switch || !msgs->eps.apb_stat))
                {
                    printf("Controller %d at %7.2f : Returning to low speed...\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_NONE;
                    // Отключение генератора
                    acts->update_gen_demand = 1;
                    acts->gen_demand = 0;
                    // Задача малых оборотов
                    data->starting = 1;
                    data->last_start_cmd = cur_time;
                    acts->set_N = 1;
                    acts->N_target = rotor->N_idle;
                }
                if (!data->starting && !data->emergency_found)
                {
                    rsps->eps.power_avail = 1;
                    rsps->mpu.apu_bleed_avail = 1;
                    rsps->air_cs.apu_bleed_avail = 1;
                }
                break;

            case STATE_LOAD:
                rsps->rcs.power_on = 1;
                rsps->fs.fuel_demand = 1;
                check_N_EGT(cur_time, rotor, rsps, data);
                data->emergency_found = 0;
                check_for_emergencies(c1, cur_time, &last_event, rotor, ggen, rsps, data);
                // Самая приоритетная задача - включение МСУ
                if (!data->emergency_found && msgs->mpu.eng_start)
                {
                    printf("Controller %d at %7.2f : Switched to MPU air bleed.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_MPU_ON_CMD;
                    // Убираем генератор, воздух обновляем
                    acts->update_gen_demand = 1;
                    acts->gen_demand = 0;
                    acts->update_air_demand = 1;
                    acts->switch_fcv_to_mpu = 1;
                }
                // Если задано выключить генератор или СЭС не получает питание от него, то отключаем
                else if (!data->emergency_found && (!msgs->eps.gen_switch || !msgs->eps.apb_stat))
                {
                    printf("Controller %d at %7.2f : Generator turned OFF.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_GEN_OFF_CMD;
                    // Убираем генератор
                    acts->update_gen_demand = 1;
                    acts->gen_demand = 0;
                }
                // Если задано выключить подачу воздуха в СКВ или высокое давление в пневмосети
                else if (!data->emergency_found && 
                    (msgs->air_cs.bleed_air_demand || msgs->air_cs.duct_pres))
                {
                    printf("Controller %d at %7.2f : ACS bleed turned OFF.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_BLEED_OFF_CMD;
                    // Убираем требование воздуха 
                    acts->update_air_demand = 1;
                    acts->close_bsv = 1;
                }
                if (!data->emergency_found)
                {
                    rsps->eps.power_avail = 1;
                    rsps->mpu.apu_bleed_avail = 1;
                    rsps->air_cs.apu_bleed_avail = 1;
                }
                break;

            case STATE_MPU_START:
            case STATE_MPU_START_LIMITED:
                rsps->rcs.power_on = 1;
                rsps->fs.fuel_demand = 1;
                check_N_EGT(cur_time, rotor, rsps, data);
                data->emergency_found = 0;
                if(STATE_MPU_START)
                    check_for_emergencies(c1, cur_time, &last_event, rotor, ggen, rsps, data);
                else
                    check_for_emergencies_emergent(
                        c1, cur_time, &last_event, rotor, ggen, msgs, rsps, data);
                rsps->mpu.apu_bleed_valve_stat = psys->bsv.sensor.value;
                rsps->mpu.crossbleed_valve_stat = psys->fcv.sensor.value;
                if (psys->bsv.sensor.value && !psys->fcv.sensor.value && phys->enough_pressure)
                    rsps->mpu.apu_bleed_pres = 1;
                else
                    rsps->mpu.apu_bleed_pres = 0;
                // Остановка
                if (data->starting && !data->emergency_found &&
                    DOUBLE_EQUALS(data->N_cur, rotor->N_idle, N_COMP_CONST))
                {
                    printf("Controller %d at %7.2f : Returned to low speed.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_MPU_OFF_CMD;
                    data->starting = 0;
                }
                // Работа на разные двигатели
                if (!data->starting && !data->emergency_found &&
                    psys->fcv.sensor.value != msgs->mpu.eng_switch)
                {
                    if (!msgs->mpu.eng_switch)
                    {
                        printf("Controller %d at %7.2f : Switched to left engine.\n",
                            c1->id, TIME(cur_time));
                        // Меняем на левый двигатель
                        acts->update_air_demand = 1;
                        acts->switch_xbleed_to_left = 1;
                    }
                    else
                    {
                        printf("Controller %d at %7.2f : Switched to right engine.\n",
                            c1->id, TIME(cur_time));
                        // Меняем на правый двигатель
                        acts->update_air_demand = 1;
                        acts->switch_xbleed_to_right = 1;
                    }
                }
                // Если больше не требуется ВСУ
                // Также остаемся в режиме до тех пор, пока не остановимся
                else if (!data->starting && !data->emergency_found
                    && msgs->mpu.eng_start_cutoff)
                {
                    printf("Controller %d at %7.2f : Returning to low speed...\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_NONE;
                    // Отключение подачи воздуха
                    acts->update_air_demand = 1;
                    acts->close_bsv = 1;
                    // Задача малых оборотов
                    data->starting = 1;
                    data->last_start_cmd = cur_time;
                    acts->set_N = 1;
                    acts->N_target = rotor->N_idle;
                }
                if (!data->starting && !data->emergency_found)
                    rsps->mpu.apu_bleed_avail = 1;
                break;

            case STATE_IDLE_RUN_LIMITED:
                // Остаемся в состоянии до тех пор, пока не выйдем на ограниченные обороты
                rsps->rcs.power_on = 1;
                rsps->fs.fuel_demand = 1;
                check_N_EGT(cur_time, rotor, rsps, data);
                data->emergency_found = 0;
                check_for_emergencies_emergent(
                    c1, cur_time, &last_event, rotor, ggen, msgs, rsps, data);
                // Обработка команды выключения 
                if (!data->emergency_found && msgs->rcs.stop_cmd)
                {
                    printf("Controller %d at %7.2f : Shutdown command recieved.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_NORMAL_SHUTDOWN_CMD;
                    data->starting = 0;
                }
                // Высший приоритет имеет команда запуска МСУ, затем генератор 
                // Команды выполняются, если не принята другая и если нет аварии
                // Поскольку остаемся в состоянии до выхода на обороты, событие EVENT_NONE
                else if (!data->starting && !data->emergency_found && msgs->mpu.eng_start)
                {
                    printf("Controller %d at %7.2f : MPU demanded air bleed.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_NONE;
                    data->starting = 1;
                    data->last_start_cmd = cur_time;
                    acts->set_N = 1;
                    acts->N_target = rotor->N_limited;
                }
                // Если есть запрос на генератор и нет наземного питания
                else if (!data->starting && !data->emergency_found && 
                    msgs->eps.gen_switch && !msgs->eps.ext_power_avail)
                {
                    printf("Controller %d at %7.2f : EPS demanded generator.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_NONE;
                    data->starting = 1;
                    data->last_start_cmd = cur_time;
                    acts->set_N = 1;
                    acts->N_target = rotor->N_limited;
                }
                // Если нет команд, сообщаем о готовности к нагрузке
                else if (!data->starting && !data->emergency_found)
                {
                    rsps->eps.power_avail = 1;
                    rsps->mpu.apu_bleed_avail = 1;
                }
                // Вышли на рабочие (ограниченные) обороты
                else if (data->starting && !data->emergency_found && 
                    data->N_cur >= rotor->N_limited - N_COMP_CONST2)
                {
                    if (msgs->mpu.eng_start)
                    {
                        last_event = EVENT_MPU_ON_CMD;
                        acts->update_air_demand = 1;
                        acts->open_bsv = 1;
                        acts->switch_fcv_to_mpu = 1;
                    }
                    else if (msgs->eps.gen_switch)
                    {
                        last_event = EVENT_GEN_ON_CMD;
                        acts->update_gen_demand = 1;
                        acts->gen_demand = 1;
                    }
                    printf("Controller %d at %7.2f : Limited rotation speed reached.\n",
                        c1->id, TIME(cur_time));
                    data->starting = 0;
                }
                // Иначе ждем выхода ротора на обороты
                break;

            case STATE_GEN_LIMITED:
                rsps->rcs.power_on = 1;
                rsps->fs.fuel_demand = 1;
                check_N_EGT(cur_time, rotor, rsps, data);
                data->emergency_found = 0;
                check_for_emergencies_emergent(
                    c1, cur_time, &last_event, rotor, ggen, msgs, rsps, data);
                // Остановка - в начале, т.к. начатая приоритетнее других команд
                if (data->starting && !data->emergency_found &&
                    DOUBLE_EQUALS(data->N_cur, rotor->N_idle, N_COMP_CONST))
                {
                    printf("Controller %d at %7.2f : Returned to low speed.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_GEN_OFF_CMD;
                    data->starting = 0;
                }
                // Вторая по приоритетности команда - включение МСУ
                else if (!data->starting && !data->emergency_found && msgs->mpu.eng_start)
                {
                    printf("Controller %d at %7.2f : Switched to MPU air bleed.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_MPU_ON_CMD;
                    // Требование воздуха + убираем генератор
                    acts->update_gen_demand = 1;
                    acts->gen_demand = 0;
                    acts->update_air_demand = 1;
                    acts->open_bsv = 1;
                    acts->switch_fcv_to_mpu = 1;
                }
                // Если больше не требуется ВСУ - в конце, т.к. лучше не сбрасывать обороты
                // Также остаемся в режиме до тех пор, пока не остановимся
                // Если задано выключить генератор или СЭС не получает питание от него, то отключаем
                else if (!data->starting && !data->emergency_found
                    && (!msgs->eps.gen_switch || !msgs->eps.apb_stat))
                {
                    printf("Controller %d at %7.2f : Returning to low speed...\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_NONE;
                    // Отключение генератора
                    acts->update_gen_demand = 1;
                    acts->gen_demand = 0;
                    // Задача малых оборотов
                    data->starting = 1;
                    data->last_start_cmd = cur_time;
                    acts->set_N = 1;
                    acts->N_target = rotor->N_idle;
                }
                if (!data->starting && !data->emergency_found)
                {
                    rsps->eps.power_avail = 1;
                    rsps->mpu.apu_bleed_avail = 1;
                }
                break;

            case STATE_COOLDOWN:
                rsps->rcs.power_on = 1;
                rsps->fs.fuel_demand = 1;
                check_N_EGT(cur_time, rotor, rsps, data);
                data->emergency_found = 0;
                check_for_emergencies(c1, cur_time, &last_event, rotor, ggen, rsps, data);
                printf("Controller %d at %7.2f : Starting cooldown.\n",
                    c1->id, TIME(cur_time));
                // Отмечаем начала охлаждения
                if (!data->cooling)
                {
                    data->last_cooldown_start = cur_time;
                    data->cooling = 1;
                }
                // По истечении таймера начинаем сбрасывать обороты и начинаем выключение
                if (!data->emergency_found &&
                    TIME(cur_time) > TIME(data->last_cooldown_start) + COOLDOWN_TIME)
                {
                    printf("Controller %d at %7.2f : Cooldown is over.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_COOLDOWN_TIMEOUT;
                }
                break;

            case STATE_NORMAL_SHUTDOWN:
                rsps->rcs.power_on = 1;
                rsps->fs.fuel_demand = 0;
                check_N_EGT(cur_time, rotor, rsps, data);
                data->emergency_found = 0;
                check_for_emergencies(c1, cur_time, &last_event, rotor, ggen, rsps, data);
                if (!data->emergency_found && !data->starting)
                {
                    // Начинаем остановку ротора
                    printf("Controller %d at %7.2f : Stopping APU...\n", c1->id, TIME(cur_time));
                    acts->set_N = 1;
                    acts->N_target = 0;
                }
                // Когда произошел выбег ротора до менее SHUTDOWN_N * 100% малых оборотов, 
                // отключаем горение и подачу топлива
                else if (!data->emergency_found && data->starting &&
                    data->N_cur < SHUTDOWN_N * rotor->N_idle)
                {
                    printf("Controller %d at %7.2f : APU stopped.\n", c1->id, TIME(cur_time));
                    last_event = EVENT_APU_STOPPED;
                    data->ignited = 0;
                    acts->turn_off_cmd = 1;
                }
                break;

            case STATE_APU_FIRE:
            case STATE_EGT_OVER_LIMIT:
            case STATE_OVSPEED:
                // Отключаем горение и подачу топлива
                data->ignited = 0;
                acts->turn_off_cmd = 1;
                printf("Controller %d at %7.2f : Initiating emergency shutdown.\n",
                    c1->id, TIME(cur_time));
                last_event = EVENT_EMERGENCY_SHUTDOWN_CMD;
                break;

            case STATE_FLAME_OUT:
                data->ignited = 0;
                // Пришли из аварийных режимов, доступно перезажигание, если не дана команда 
                // отключения
                if ((data->parent_state == STATE_EMERGENCY_START ||
                    data->parent_state == STATE_IDLE_RUN_LIMITED ||
                    data->parent_state == STATE_GEN_LIMITED ||
                    data->parent_state == STATE_MPU_START_LIMITED ||
                    data->parent_state == STATE_RELIGHT) &&
                    !msgs->rcs.stop_cmd)
                {
                    printf("Controller %d at %7.2f : Attempting to relight.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_RELIGHT_ATTEMPT_CMD;
                }
                // Иначе пришли из нормального состояния => аварийная остановка
                else
                {
                    acts->turn_off_cmd = 1;
                    printf("Controller %d at %7.2f : Initiating emergency shutdown.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_EMERGENCY_SHUTDOWN_CMD;
                }
                break;

            case STATE_RELIGHT:
                if (phys->ignited)
                {
                    printf("Controller %d at %7.2f : Flame restored.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_FLAME_RESTORED;
                    data->ignited = 1;
                }
                else
                {
                    printf("Controller %d at %7.2f : Failed to restore flame.\n",
                        c1->id, TIME(cur_time));
                    last_event = EVENT_FLAME_WENT_OUT; 
                }
                break;

            case STATE_EMERGENCY_SHUTDOWN:
                acts->set_N = 1;
                acts->N_target = 0;
                data->ignited = 0;
                acts->turn_off_cmd = 1;
                printf("Controller %d at %7.2f : Emergency shutdown initiated.\n",
                    c1->id, TIME(cur_time));
                last_event = EVENT_POWER_OFF;
            }

        }

        // Состояние     a  b  b  b  c
        // Прошлое       -  a  b  b  b
        // Родительское  -  a  a  a  b
        // Сохраняются только рабочие состояния: IDLE RUN, GEN, MPU (+LIMITED), BLEED, LOAD + RELIGHT
        if (state != data->prev_state)
            data->parent_state = data->prev_state;
        if (state == STATE_IDLE_RUN || state == STATE_IDLE_RUN_LIMITED ||
            state == STATE_BLEED || state == STATE_LOAD ||
            state == STATE_GEN || state == STATE_GEN_LIMITED ||
            state == STATE_MPU_START || state == STATE_MPU_START_LIMITED ||
            state == STATE_RELIGHT)
            data->prev_state = state;
            
        // Обработка события
        handle_event(state, last_event);
    }
    // Если c1->fault, то контроллер завис, ничего не обновляется
}
