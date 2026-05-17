/* @file   apu_components.c
*  @brief  Инициализация и обновление агрегатов
*
*  @detail Файл содержит код функций, задающих параметры агрегатов и описывающих их работу за 1 
*  такт.
*
*  @author Бугакова А.А.
*/


#include "apu_components.h"
#include "apu_defs.h"
#include "apu_math.h"


void init_valve(Valve* valve)
{
    valve->power        = OFF;
    valve->open_cmd     = 0;
    valve->close_cmd    = 0;
    valve->open         = 0;
    valve->fault        = 0;
    // Датчик инициализируется отдельно в зависисмости от его типа
}

void init_starter(Starter* starter)
{
    starter->power      = OFF; 
    starter->k_yn       = 0.9934;
    starter->k_u        = 0.3987;
    starter->M          = 0;
    starter->M_max      = 60;
    starter->turnoff_N  = 6000;
}

void init_PID(PID* pid)
{
    pid->k_p[0]         = 11.999549; 
    pid->k_p[1]         = 12.675221;
    pid->k_i[0]         = 0.20018217;
    pid->k_i[1]         = 0.020742308;
    pid->k_d[0]         = 0.2998349;
    pid->k_d[1]         = 0.27940389;
    pid->k_pid_un       = 4.7619;
    pid->k_pid_un1      = 4.7619;
    pid->k_pid_yn       = 0.9048;
    pid->N_diff_prev    = 0;
    pid->i_prev         = 0;
    pid->d_prev         = 0;
    pid->k_fuel         = 1.0 / 40000.0;
    pid->fuel_feed      = 0;
}

void init_gas_generator(Gas_generator* ggen)
{
    ggen->ignition       = OFF;
    ggen->ignited        = OFF;
    ggen->fuel_N         = 2400;
    ggen->fuel_cmd       = 0;
    ggen->k_yn           = 0.9934;
    ggen->k_u            = 0.0797;
    ggen->fuel           = 0;
    ggen->fuel_max       = 12;
    ggen->k_M            = 250.0 / 12.0;
    ggen->M              = 0;
    ggen->start_moment_N = 6000;
    ggen->full_moment_N  = 24000;

    for (int i = 0; i < GGEN_BUFFER_SIZE; i++)
        ggen->buffer[i] = 0;

    init_discrete_sensor(&(ggen->flame_sensor), SENSOR_TYPE_FLAME, 0);
}

void init_rotor(Rotor* rotor)
{
    rotor->k_N        = 15;
    rotor->M_diff     = 0;
    rotor->k_friction = 6;
    rotor->N          = 0;
    rotor->N_target   = 0;
    rotor->N_idle     = 10000;
    rotor->N_work     = 39000;
    rotor->N_limited  = 32000;
    rotor->k_EGT      = 0.2;
    rotor->EGT        = ATMOSPHERIC_TEMP;
    rotor->delta_EGT  = 0;
    rotor->N1_0_err   = 1.02;
    rotor->N1_1_err   = 0.98;
    rotor->EGT_A0_err = 1.03;
    rotor->EGT_A1_err = 1.02;
    rotor->EGT_B0_err = 0.98;
    rotor->EGT_B1_err = 0.97;
    
    init_digital_sensor(&(rotor->N1_0),   SENSOR_TYPE_N1,  0);
    init_digital_sensor(&(rotor->N1_1),   SENSOR_TYPE_N1,  1);
    init_digital_sensor(&(rotor->EGT_A0), SENSOR_TYPE_EGT, 0);
    init_digital_sensor(&(rotor->EGT_A1), SENSOR_TYPE_EGT, 1);
    init_digital_sensor(&(rotor->EGT_B0), SENSOR_TYPE_EGT, 2);
    init_digital_sensor(&(rotor->EGT_B1), SENSOR_TYPE_EGT, 3);
}

void init_compressor(Compressor* comp)
{
    comp->k_NM  = 1.1e-7;
    comp->k_inc = 1.3;
    comp->bleed = 0;
    comp->M     = 0;
    comp->k_NP  = 2.8e-8;
    comp->P     = ATMOSPHERIC_PRES;
    comp->k_T1  = 0.2;
    comp->k_T2  = 0.9;
    comp->T     = ATMOSPHERIC_TEMP;

    init_digital_sensor(&(comp->P3), SENSOR_TYPE_P3, 0);
    init_digital_sensor(&(comp->T3), SENSOR_TYPE_T3, 0);
}

void init_cooling_fan(Cooling_fan* fan)
{
    fan->fault    = 0;
    fan->k_reduct = 0.2;
    fan->N        = 0;
    fan->k_M      = 1.1e-7;
    fan->M        = 0;
    fan->k_cool   = 0.002;
}

void init_generator(Generator* gen)
{
    gen->on       = 0;
    gen->N_turnon = 30000;
    gen->N_own    = 12000;
    gen->M        = 0;
    gen->M_const  = 15;

    init_digital_sensor(&(gen->NGC), SENSOR_TYPE_NGC, 0);
}

void init_fuel_pump(Fuel_pump* pump)
{
    pump->power         = OFF;
    pump->fault         = 0;
    pump->turn_on_cmd   = 0;
    pump->turn_off_cmd  = 0;
    pump->on            = 0;
    pump->M             = 0;
    pump->M_const       = 1;

    init_valve(&(pump->fuel_sov));
    init_discrete_sensor(&(pump->fuel_sov.sensor), SENSOR_TYPE_FUEL_SOV, 0);
    init_discrete_sensor(&(pump->P_fuel), SENSOR_TYPE_P_FUEL, 0);
}

void init_pneumatic_system(Pneumatic_system* psys)
{
    psys->P_loss = 0.7;
    psys->k_T    = 0.4;

    init_valve(&(psys->asv));
    init_discrete_sensor(&(psys->asv.sensor), SENSOR_TYPE_ASV, 0);
    init_digital_sensor(&(psys->T2), SENSOR_TYPE_T2, 0);
    init_digital_sensor(&(psys->P2), SENSOR_TYPE_P2, 0);
    init_valve(&(psys->bsv));
    init_discrete_sensor(&(psys->bsv.sensor), SENSOR_TYPE_BSV, 0);
    init_digital_sensor(&(psys->T_duct), SENSOR_TYPE_T_DUCT, 0);
    init_digital_sensor(&(psys->P_duct), SENSOR_TYPE_P_DUCT, 0);
    init_valve(&(psys->fcv));
    init_discrete_sensor(&(psys->fcv.sensor), SENSOR_TYPE_FCV, 0);
    init_valve(&(psys->mpu_xbleed));
    init_discrete_sensor(&(psys->mpu_xbleed.sensor), SENSOR_TYPE_MPU_XBLEED, 0);
}

void init_APU(APU* apu)
{
    init_starter(&(apu->start));
    init_PID(&(apu->pid));
    init_gas_generator(&(apu->ggen));
    init_rotor(&(apu->rotor));
    init_compressor(&(apu->comp));
    init_cooling_fan(&(apu->fan));
    init_generator(&(apu->gen));
    init_fuel_pump(&(apu->pump));
    init_pneumatic_system(&(apu->psys));
}


void update_starter(Starter* starter, Rotor* rotor)
{
    // Логика включения-выключения
    if (rotor->N >= starter->turnoff_N || starter->turn_off_cmd)
    {
        starter->power = OFF;
        starter->turn_off_cmd = 0;
    }

    // Вычисление момента
    // y[n+1] = k_yn * y[n] + k_u * u[n]  + ограничение момента
    starter->M = MIN(starter->M_max, starter->k_yn * starter->M + starter->k_u * starter->power);
}

void update_PID(PID* pid, Rotor* rotor)
{
    double N_diff = rotor->N_target - rotor->N;
    if (rotor->N <= PID_N)
        pid->p = pid->k_p[0] * N_diff;
    else
        pid->p = pid->k_p[1] * N_diff;
    // Численное интегрирование методом трапеций
    // y[n+1] = y[n] + k * T * (u[n] + u[n-1]) / 2
    if (rotor->N <= PID_N)
        pid->i = pid->i_prev + pid->k_i[0] * H * (N_diff + pid->N_diff_prev) / 2;
    else
        pid->i = pid->i_prev + pid->k_i[1] * H * (N_diff + pid->N_diff_prev) / 2;
    // Получено методом билинейного преобразования Тастина
    // y[n+1] = k_pid_yn * y[n] + k_d * (k_pid_un * u[n] + k_pid_un1 * u[n-1])
    if (rotor->N <= PID_N)
        pid->d = pid->k_pid_yn * pid->d_prev
            + pid->k_d[0] * (pid->k_pid_un * N_diff + pid->k_pid_un1 * pid->N_diff_prev);
    else
        pid->d = pid->k_pid_yn * pid->d_prev
            + pid->k_d[1] * (pid->k_pid_un * N_diff + pid->k_pid_un1 * pid->N_diff_prev);
    // Перевод в признак топлива и ограничение 
    pid->fuel_feed = MAX(0, MIN(1, pid->k_fuel * (pid->p + pid->i + pid->d)));
    // Обновление значений для следующего шага
    pid->i_prev = pid->i;
    pid->d_prev = pid->d;
}

void update_gas_generator(
    Gas_generator* ggen, 
    PID* pid, 
    Rotor* rotor, 
    Fuel_pump* pump)
{
    // Обновление параметра и датчика пламени в камере сгорания
    bool flame_went_out = (ggen->ignited && !ggen->ignition);
    ggen->ignited = ggen->ignition;
    if (ggen->flame_sensor.power && !ggen->flame_sensor.fault)
        update_discrete_sensor(&(ggen->flame_sensor), ggen->ignition);
    
    // Реализация транспортной задержки
    ggen->M = ggen->buffer[0];
    for (int i = 1; i < GGEN_BUFFER_SIZE; i++)
        ggen->buffer[i - 1] = ggen->buffer[i];

    // Определение подачи топлива
    if (rotor->N < ggen->fuel_N)
        ggen->fuel_cmd = 0;
    else
        ggen->fuel_cmd = pid->fuel_feed;
    // Вычисление скорости подачи топлива, если включен насос
    // y[n+1] = k_yn * y[n] + k_u * u[n]  + ограничение момента
    if (pump->on)
        ggen->fuel = MIN(ggen->fuel_max, ggen->k_yn * ggen->fuel + ggen->k_u * ggen->fuel_cmd);
    else
        ggen->fuel = 0;

    // Линейное нарастание момента после достижения start_moment_N
    double M_new;
    if (rotor->N <= ggen->start_moment_N)
        M_new = 0;
    else if (rotor->N <= ggen->full_moment_N)
        M_new = ggen->k_M * ggen->fuel * (rotor->N - ggen->start_moment_N)
                / (ggen->full_moment_N - ggen->start_moment_N);
    else
        M_new = ggen->k_M * ggen->fuel;
    // При погасании пламени газогенератор теряет 80% момента, далее выбег
    if (flame_went_out)
        M_new = 0.2 * M_new;
    ggen->buffer[GGEN_BUFFER_SIZE - 1] = M_new;
}

void update_rotor(
    Rotor* rotor, 
    Starter* start, 
    Gas_generator* ggen, 
    Compressor* comp, 
    Cooling_fan* fan,
    Generator* gen,
    Fuel_pump* pump,
    Digital_sensor* T2)
{
    // Вычисление входного и дифференциального моментов ротора
    double M          = start->M + ggen->M;
    double M_friction = rotor->k_friction * sign(rotor->N);
    double M_diff     = start->M + ggen->M - comp->M - fan->M - gen->M - pump->M - M_friction;

    // Вычисление скорости вращения: численное интегрирование методом трапеций
    // y[n+1] = y[n] + k * T * (u[n] + u[n-1]) / 2
    rotor->N = MAX(0, rotor->N + rotor->k_N * H * (rotor->M_diff + M_diff) / 2);
    // Сохранение значения момента для следующего шага
    rotor->M_diff = M_diff;

    // Вычисление мощности ротора
    double P = M * rotor->N;
    // Вычисление целевой температуры и сохранение старой
    double EGT_old = rotor->EGT;
    double EGT_expected = T2->value;
    if (rotor->N != 0)
        EGT_expected = T2->value + 7e-5 * P / (rotor->N / rotor->N_work);
    // else целевая температура не изменяется
    // Вычисление охлаждения от вентилятора
    double cool = fan->k_cool * fan->N * fan->M;
    // Вычисление изменения температуры выхлопных газов
    double delta_EGT = EGT_expected - EGT_old - cool;

    // Вычисление температуры выхлопных газов: численное интегрирование методом трапеций
    // y[n+1] = y[n] + k * T * (u[n] + u[n-1]) / 2
    rotor->EGT = rotor->EGT + rotor->k_EGT * H * (rotor->delta_EGT + delta_EGT) / 2;
    // Сохранение значения изменения температуры для следующего шага
    rotor->delta_EGT = delta_EGT;

    // Запись показаний на датчики (с постоянной ошибкой и учетом отказов)
    if (rotor->N1_0.power && !rotor->N1_0.fault)
        update_digital_sensor(&(rotor->N1_0), rotor->N * rotor->N1_0_err);
    else
        update_digital_sensor(&(rotor->N1_0), rotor->N1_0.max_value);
    if (rotor->N1_1.power && !rotor->N1_1.fault)
        update_digital_sensor(&(rotor->N1_1), rotor->N * rotor->N1_1_err);
    else
        update_digital_sensor(&(rotor->N1_1), rotor->N1_1.max_value);
    if (rotor->EGT_A0.power && !rotor->EGT_A0.fault)
        update_digital_sensor(&(rotor->EGT_A0), rotor->EGT * rotor->EGT_A0_err);
    else
        update_digital_sensor(&(rotor->EGT_A0), rotor->EGT_A0.max_value);
    if (rotor->EGT_A1.power && !rotor->EGT_A1.fault)
        update_digital_sensor(&(rotor->EGT_A1), rotor->EGT * rotor->EGT_A1_err);
    else
        update_digital_sensor(&(rotor->EGT_A1), rotor->EGT_A1.max_value);
    if (rotor->EGT_B0.power && !rotor->EGT_B0.fault)
        update_digital_sensor(&(rotor->EGT_B0), rotor->EGT * rotor->EGT_B0_err);
    else
        update_digital_sensor(&(rotor->EGT_B0), rotor->EGT_B0.max_value);
    if (rotor->EGT_B1.power && !rotor->EGT_B1.fault)
        update_digital_sensor(&(rotor->EGT_B1), rotor->EGT * rotor->EGT_B1_err);
    else
        update_digital_sensor(&(rotor->EGT_B1), rotor->EGT_B1.max_value);
}

void update_compressor(Compressor* comp, Rotor* rotor, Pneumatic_system* psys)
{
    // Расчет выходных параметров
    comp->M = comp->k_NM * binpow(rotor->N, 2);
    // Если есть подача воздуха в пневмосистему
    if (comp->bleed)
        comp->M = comp->M * comp->k_inc;
    comp->P = psys->P2.value + comp->k_NP * binpow(rotor->N, 2);
    if(!DOUBLE_EQUALS(psys->P2.value, 0.0, COMP_CONST))
        comp->T = (KELVIN + psys->T2.value) 
            * (comp->k_T1 * comp->P / psys->P2.value + comp->k_T2) - KELVIN;
    else
        comp->T = (KELVIN + psys->T2.value)
        * (comp->k_T1 * 1 + comp->k_T2) - KELVIN;

    // Запись показаний на датчики
    if(comp->P3.power && !comp->P3.fault)
        update_digital_sensor(&(comp->P3), comp->P);
    else
        update_digital_sensor(&(comp->P3), comp->P3.max_value);
    if (comp->T3.power && !comp->T3.fault)
        update_digital_sensor(&(comp->T3), comp->T);
    else
        update_digital_sensor(&(comp->T3), comp->T3.max_value);
}

void update_cooling_fan(Cooling_fan* fan, Rotor* rotor)
{
    // Правильная работа вентилятора
    if (!fan->fault)
    {
        // Редукция и сохранение оборотов
        double fan_N = rotor->N * fan->k_reduct;
        fan->N = fan_N;

        // Расчет момента
        fan->M = fan->k_M * fan_N * fan_N;
    }
    // Отказ
    else
    {
        fan->N = 0;
        fan->M = 0;
    }
}

void update_generator(Generator* gen, Rotor* rotor)
{
    // При падении оборотов генератор отключается
    if (rotor->N < gen->N_turnon)
        gen->on = 0;

    if (gen->on)
        gen->M = gen->M_const;
    else
        gen->M = 0;

    if(gen->NGC.power && !gen->NGC.fault)
        update_digital_sensor(&(gen->NGC), gen->on * gen->N_own);
}

void update_fuel_pump(
    Fuel_pump* pump, 
    bool power)
{
    // Включаем насос, клапан и датчик
    pump->power = power;
    pump->fuel_sov.power = power;
    pump->fuel_sov.sensor.power = power;
    // Если нет питания или была команда выключения, насос отключается, клапан подачи топлива 
    // закрывается (если есть питание и не заклинил)
    if (!pump->power || pump->turn_off_cmd)
    {
        if (pump->fuel_sov.power && !pump->fuel_sov.fault)
            pump->fuel_sov.open = 0;
        pump->on = 0;
        pump->M  = 0;
    }
    // Иначе, если пришла команда включения, насос включается
    else if (pump->turn_on_cmd)
    {
        // Пытаемся открыть клапан подачи топлива
        if (pump->fuel_sov.power && !pump->fuel_sov.fault)
            pump->fuel_sov.open = 1;
        // Если смогли, то включаем насос
        if (pump->fuel_sov.open)
        {
            pump->on = 1;
            pump->M = pump->M_const;
        }
        // Иначе не включаем
        else
        {
            pump->on = 0;
            pump->M = 0;
        }
    }
    pump->turn_on_cmd  = 0;
    pump->turn_off_cmd = 0;

    // Обновляем значение датчика положения
    update_discrete_sensor(&(pump->fuel_sov.sensor), pump->fuel_sov.open);
    // Обновляем значение датчика давления топлива (если неполадка, то давления нет)
    if (pump->P_fuel.power && !pump->P_fuel.fault)
        if (!pump->fault && !(pump->fuel_sov.fault && !pump->fuel_sov.open) && pump->on)
            update_discrete_sensor(&(pump->P_fuel), 1);
        else
            update_discrete_sensor(&(pump->P_fuel), 0);
}

void update_pneumatic_system(
    Pneumatic_system* psys,
    int height,
    Compressor* comp)
{
    // Обновляем датчики атмосферной температуры и давления
    if(psys->T2.power && !psys->T2.fault)
        update_T2(&(psys->T2), height);
    if (psys->P2.power && !psys->P2.fault)
        update_P2(&(psys->P2), height);

    // Если есть питание и клапан забора атмосферного воздуха не заклинил, его можно переместить
    if (psys->asv.power && !psys->asv.fault)
        if(psys->asv.open_cmd)
            psys->asv.open = 1;
        else if(psys->asv.close_cmd)
            psys->asv.open = 0;
    if(psys->asv.sensor.power && !psys->asv.sensor.fault)
        update_discrete_sensor(&(psys->asv.sensor), psys->asv.open);

    // Аналогично с входным клапаном пневмосистемы
    if (psys->bsv.power && !psys->bsv.fault)
        if (psys->bsv.open_cmd)
            psys->bsv.open = 1;
        else if (psys->bsv.close_cmd)
            psys->bsv.open = 0;
    if (psys->bsv.sensor.power && !psys->bsv.sensor.fault)
        update_discrete_sensor(&(psys->bsv.sensor), psys->bsv.open);

    // Обновляем параметры воздуха на входе в пневмосистему
    // Если один из клапанов закрыт, то давление подаваемого воздуха 0
    if(psys->P_duct.power && !psys->P_duct.fault)
        if (!psys->asv.open || !psys->bsv.open)
            update_digital_sensor(&(psys->P_duct), 0);
        else
            update_digital_sensor(&(psys->P_duct), psys->P2.value * psys->P_loss);
    if (psys->T_duct.power && !psys->T_duct.fault)
        update_digital_sensor(&(psys->T_duct),
            psys->T2.value + (comp->T3.value - psys->T2.value) * psys->k_T);

    // Аналогичное управление перекрестными клапанами
    if (psys->fcv.power && !psys->fcv.fault)
        if (psys->fcv.open_cmd)
            psys->fcv.open = 1;
        else if (psys->fcv.close_cmd)
            psys->fcv.open = 0;
    if (psys->fcv.sensor.power && !psys->fcv.sensor.fault)
        update_discrete_sensor(&(psys->fcv.sensor), psys->fcv.open);
    if (psys->mpu_xbleed.power && !psys->mpu_xbleed.fault)
        if (psys->mpu_xbleed.open_cmd)
            psys->mpu_xbleed.open = 1;
        else if (psys->mpu_xbleed.close_cmd)
            psys->mpu_xbleed.open = 0;
    if (psys->mpu_xbleed.sensor.power && !psys->mpu_xbleed.sensor.fault)
        update_discrete_sensor(&(psys->mpu_xbleed.sensor), psys->mpu_xbleed.open);

    // Сбрасываем команды
    psys->asv.open_cmd = 0;
    psys->asv.close_cmd = 0;
    psys->bsv.open_cmd = 0;
    psys->bsv.close_cmd = 0;
    psys->fcv.open_cmd = 0;
    psys->fcv.close_cmd = 0;
    psys->mpu_xbleed.open_cmd = 0;
    psys->mpu_xbleed.close_cmd = 0;
}

void update_APU(APU* apu, bool power, int height)
{
    update_starter(&(apu->start), &(apu->rotor));
    update_PID(&(apu->pid), &(apu->rotor));
    update_gas_generator(&(apu->ggen), &(apu->pid), &(apu->rotor), &(apu->pump));
    update_rotor(
        &(apu->rotor), &(apu->start), &(apu->ggen), 
        &(apu->comp), &(apu->fan), &(apu->gen), &(apu->pump), &(apu->psys.T2));
    update_compressor(&(apu->comp), &(apu->rotor), &(apu->psys));
    update_cooling_fan(&(apu->fan), &(apu->rotor));
    update_generator(&(apu->gen), &(apu->rotor));
    update_fuel_pump(&(apu->pump), power);
    update_pneumatic_system(&(apu->psys), height, &(apu->comp));
}


void copy_valve(Valve* original, Valve* copy)
{
    copy->power = original->power;
    copy->fault = original->fault;
    copy->open = original->open;
    copy->open_cmd = original->open_cmd;
    copy->close_cmd = original->close_cmd;
    copy->sensor.power = original->sensor.power;
    copy->sensor.fault = original->sensor.fault;
    copy->sensor.value = original->sensor.value;
}

void copy_APU(APU* original, APU* copy)
{
    copy->start.power = original->start.power;
    copy->start.M = original->start.M;

    copy->pid.fuel_feed = original->pid.fuel_feed;

    copy->ggen.ignition = original->ggen.ignition;
    copy->ggen.ignited = original->ggen.ignited;
    copy->ggen.flame_sensor.power = original->ggen.flame_sensor.power;
    copy->ggen.flame_sensor.fault = original->ggen.flame_sensor.fault;
    copy->ggen.flame_sensor.value = original->ggen.flame_sensor.value;
    copy->ggen.fuel_cmd = original->ggen.fuel_cmd;
    copy->ggen.fuel = original->ggen.fuel;
    copy->ggen.M = original->ggen.M;

    copy->rotor.N = original->rotor.N;
    copy->rotor.N_target = original->rotor.N_target;
    copy->rotor.N1_0.power = original->rotor.N1_0.power;
    copy->rotor.N1_0.fault = original->rotor.N1_0.fault;
    copy->rotor.N1_0.value = original->rotor.N1_0.value;
    copy->rotor.N1_1.power = original->rotor.N1_1.power;
    copy->rotor.N1_1.fault = original->rotor.N1_1.fault;
    copy->rotor.N1_1.value = original->rotor.N1_1.value;
    copy->rotor.EGT = original->rotor.EGT;
    copy->rotor.EGT_A0.power = original->rotor.EGT_A0.power;
    copy->rotor.EGT_A0.fault = original->rotor.EGT_A0.fault;
    copy->rotor.EGT_A0.value = original->rotor.EGT_A0.value;
    copy->rotor.EGT_A1.power = original->rotor.EGT_A1.power;
    copy->rotor.EGT_A1.fault = original->rotor.EGT_A1.fault;
    copy->rotor.EGT_A1.value = original->rotor.EGT_A1.value;
    copy->rotor.EGT_B0.power = original->rotor.EGT_B0.power;
    copy->rotor.EGT_B0.fault = original->rotor.EGT_B0.fault;
    copy->rotor.EGT_B0.value = original->rotor.EGT_B0.value;
    copy->rotor.EGT_B1.power = original->rotor.EGT_B1.power;
    copy->rotor.EGT_B1.fault = original->rotor.EGT_B1.fault;
    copy->rotor.EGT_B1.value = original->rotor.EGT_B1.value;

    copy->comp.bleed = original->comp.bleed;
    copy->comp.M = original->comp.M;
    copy->comp.P = original->comp.P;
    copy->comp.T = original->comp.T;
    copy->comp.P3.power = original->comp.P3.power;
    copy->comp.P3.fault = original->comp.P3.fault;
    copy->comp.P3.value = original->comp.P3.value;
    copy->comp.T3.power = original->comp.T3.power;
    copy->comp.T3.fault = original->comp.T3.fault;
    copy->comp.T3.value = original->comp.T3.value;

    copy->fan.fault = original->fan.fault;
    copy->fan.N = original->fan.N;
    copy->fan.M = original->fan.M;

    copy->gen.on = original->gen.on;
    copy->gen.M = original->gen.M;
    copy->gen.NGC.power = original->gen.NGC.power;
    copy->gen.NGC.fault = original->gen.NGC.fault;
    copy->gen.NGC.value = original->gen.NGC.value;

    copy->pump.power = original->pump.power;
    copy->pump.turn_on_cmd = original->pump.turn_on_cmd;
    copy->pump.turn_off_cmd = original->pump.turn_off_cmd;
    copy->pump.on = original->pump.on;
    copy->pump.M = original->pump.M;
    copy_valve(&(original->pump.fuel_sov), &(copy->pump.fuel_sov));
    copy->pump.P_fuel.power = original->pump.P_fuel.power;
    copy->pump.P_fuel.fault = original->pump.P_fuel.fault;
    copy->pump.P_fuel.value = original->pump.P_fuel.value;

    copy_valve(&(original->psys.asv), &(copy->psys.asv));
    copy_valve(&(original->psys.bsv), &(copy->psys.bsv));
    copy_valve(&(original->psys.fcv), &(copy->psys.fcv));
    copy_valve(&(original->psys.mpu_xbleed), &(copy->psys.mpu_xbleed));
    copy->psys.P2.power = original->psys.P2.power;
    copy->psys.P2.fault = original->psys.P2.fault;
    copy->psys.P2.value = original->psys.P2.value;
    copy->psys.T2.power = original->psys.T2.power;
    copy->psys.T2.fault = original->psys.T2.fault;
    copy->psys.T2.value = original->psys.T2.value;
    copy->psys.P_duct.power = original->psys.P_duct.power;
    copy->psys.P_duct.fault = original->psys.P_duct.fault;
    copy->psys.P_duct.value = original->psys.P_duct.value;
    copy->psys.T_duct.power = original->psys.T_duct.power;
    copy->psys.T_duct.fault = original->psys.T_duct.fault;
    copy->psys.T_duct.value = original->psys.T_duct.value;
}