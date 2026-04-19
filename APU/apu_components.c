#include "apu_defs.h"
#include "apu_math.h"
#include "apu_components.h"

void init_starter(Starter* starter)
{
    starter->power     = OFF;
    starter->k_yn      = 0.9934;
    starter->k_u       = 0.3987;
    starter->M         = 0;
    starter->M_max     = 60;
    starter->turnoff_N = 15;
}

void init_gas_generator(Gas_generator* ggen)
{
    ggen->ignition       = OFF;
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
    rotor->N          = 0;
    rotor->N_target   = 0;
    rotor->N_work     = 40000;
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
    comp->k_NM = 1.4e-7;
    comp->M    = 0;
    comp->k_NP = 5e-7;
    comp->P    = ATMOSPHERIC_PRES;
    comp->k_T1 = 0.06;
    comp->k_T2 = 0.9;
    comp->T    = ATMOSPHERIC_TEMP;

    init_digital_sensor(&(comp->P3), SENSOR_TYPE_P3, 0);
    init_digital_sensor(&(comp->T3), SENSOR_TYPE_T3, 0);
}

void init_cooling_fan(Cooling_fan* fan)
{
    fan->fault    = 0;
    fan->k_reduct = 0.2;
    fan->N        = 0;
    fan->k_M      = 1.4e-7;
    fan->M        = 0;
    fan->k_cool   = 0.002;
}

void init_generator(Generator* gen)
{
    gen->on = 0;
    gen->N_turnon = 8000;
    gen->M = 0;
    gen->M_const = 15;

    init_digital_sensor(&(gen->NGC), SENSOR_TYPE_NGC, 0);
}


void update_starter(Starter* starter, bool turn_on_cmd, Rotor* rotor)
{
    // Логика включения-выключения
    if (rotor->N >= starter->turnoff_N)
        starter->power = OFF;
    if (turn_on_cmd)
        starter->power = ON;

    // Вычисление момента
    // y[n+1] = k_yn * y[n] + k_u * u[n]  + ограничение момента
    starter->M = MIN(starter->M_max, starter->k_yn * starter->M + starter->k_u * starter->power);
}

void update_gas_generator(Gas_generator* ggen, bool ignition, Rotor* rotor)
{
    // Обновление параметра и датчика пламени в камере сгорания
    bool flame_went_out = (ggen->ignition && !ignition);
    ggen->ignition = ignition;
    if(!ggen->flame_sensor.fault)
        update_discrete_sensor(&(ggen->flame_sensor), ignition);
    
    // Реализация транспортной задержки
    ggen->M = ggen->buffer[0];
    for (int i = 1; i < GGEN_BUFFER_SIZE; i++)
        ggen->buffer[i - 1] = ggen->buffer[i];

    // Вычисление скорости подачи топлива
    // y[n+1] = k_yn * y[n] + k_u * u[n]  + ограничение момента
    ggen->fuel = MAX(ggen->fuel_max, ggen->k_yn * ggen->fuel + ggen->k_u * ggen->ignition);

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
    double M      = start->M + gen->M;
    double M_diff = start->M + gen->M - comp->M - fan->M - gen->M - pump->M;

    // Вычисление скорости вращения: численное интегрирование методом трапеций
    // y[n+1] = y[n] + k * T * (u[n] + u[n-1]) / 2  + ограничение N_target оборотами
    rotor->N = MIN(rotor->N_target, rotor->N + rotor->k_N * H * (rotor->M_diff + M_diff) / 2);
    // Сохранение значения момента для следующего шага
    rotor->M_diff = M_diff;

    // Вычисление мощности ротора
    double P = M * rotor->N;
    // Вычисление целевой температуры и сохранение старой
    double EGT_old = rotor->EGT;
    if (rotor->N != 0)
        rotor->EGT = T2->value + 7e-5 * P / (rotor->N / rotor->N_work);
    // else целевая температура не изменяется
    // Вычисление охлаждения от вентилятора
    double cool = fan->k_cool * fan->N * fan->M;
    // Вычисление изменения температуры выхлопных газов
    double delta_EGT = rotor->EGT - EGT_old - cool;

    // Вычисление температуры выхлопных газов: численное интегрирование методом трапеций
    // y[n+1] = y[n] + k * T * (u[n] + u[n-1]) / 2
    rotor->EGT = rotor->EGT + H * (rotor->delta_EGT + delta_EGT) / 2;
    // Сохранение значения изменения температуры для следующего шага
    rotor->delta_EGT = delta_EGT;

    // Запись показаний на датчики (с постоянной ошибкой и учетом отказов)
    if (!rotor->N1_0.fault)
        update_digital_sensor(&(rotor->N1_0), rotor->N * rotor->N1_0_err);
    else
        update_digital_sensor(&(rotor->N1_0), rotor->N1_0.max_value);
    if (!rotor->N1_1.fault)
        update_digital_sensor(&(rotor->N1_1), rotor->N * rotor->N1_1_err);
    else
        update_digital_sensor(&(rotor->N1_1), rotor->N1_1.max_value);
    if (!rotor->EGT_A0.fault)
        update_digital_sensor(&(rotor->EGT_A0), rotor->EGT * rotor->EGT_A0_err);
    else
        update_digital_sensor(&(rotor->EGT_A0), rotor->EGT_A0.max_value);
    if (!rotor->EGT_A1.fault)
        update_digital_sensor(&(rotor->EGT_A1), rotor->EGT * rotor->EGT_A1_err);
    else
        update_digital_sensor(&(rotor->EGT_A1), rotor->EGT_A1.max_value);
    if (!rotor->EGT_B0.fault)
        update_digital_sensor(&(rotor->EGT_B0), rotor->EGT * rotor->EGT_B0_err);
    else
        update_digital_sensor(&(rotor->EGT_B0), rotor->EGT_B0.max_value);
    if (!rotor->EGT_B1.fault)
        update_digital_sensor(&(rotor->EGT_B1), rotor->EGT * rotor->EGT_B1_err);
    else
        update_digital_sensor(&(rotor->EGT_B1), rotor->EGT_B1.max_value);
}

void update_compressor(Compressor* comp, Rotor* rotor, Digital_sensor* P2, Digital_sensor* T2)
{
    // Расчет выходных параметров
    comp->M = comp->k_NM * binpow(rotor->N, 2);
    comp->P = P2->value + comp->k_NP * binpow(rotor->N, 2);
    comp->T = (KELVIN + T2->value) * (comp->k_T1 * comp->P3.value / P2->value + comp->k_T2)
        - KELVIN;

    // Запись показаний на датчики
    if(!comp->P3.fault)
        update_digital_sensor(&(comp->P3), comp->P);
    else
        update_digital_sensor(&(comp->P3), comp->P3.max_value);
    if (!comp->T3.fault)
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
        fan->M = fan->k_M * binpow(fan_N, 2);
    }
    // Отказ
    else
    {
        fan->N = 0;
        fan->M = 0;
    }
}

void update_generator(Generator* gen, bool turn_on_cmd, bool turn_off_cmd, Rotor* rotor)
{
    // При падении оборотов или команде выключения генератор отключается
    if (rotor->N < gen->N_turnon || turn_off_cmd)
    {
        gen->on = 0;
        gen->M  = 0;
    }
    // При команде включения генератор включается, если достаточные обороты
    else if (turn_on_cmd)
    {
        gen->on = 1;
        gen->M  = gen->M_const;
    }  
}


void rotor_set_target_N(Rotor* rotor, double N_target)
{
    rotor->N_target = N_target;
}