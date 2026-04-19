#include "apu_defs.h"
#include "apu_math.h"
#include "apu_components.h"

void init_starter(Starter* starter)
{
    starter->power       = OFF;
    starter->k_yn        = 0.9934;
    starter->k_u         = 0.3987;
    starter->M           = 0;
    starter->M_max       = 60;
    starter->turnoff_N   = 15;
}

void init_gas_generator(Gas_generator* ggen)
{
    ggen->ignition       = OFF;
    ggen->M              = 0;
    ggen->M_max          = 250;
    ggen->start_moment_N = 6000;
    ggen->full_moment_N  = 24000;

    for (int i = 0; i < GGEN_BUFFER_SIZE; i++)
        ggen->buffer[i] = 0;

    init_discrete_sensor(&(ggen->flame_sensor), SENSOR_TYPE_FLAME, 0);
}

void init_rotor(Rotor* rotor)
{
    rotor->k = 15;
    rotor->N = 0;
    rotor->N_work = 40000;
    
    init_digital_sensor(&(rotor->N1_0),   SENSOR_TYPE_N1,  0);
    init_digital_sensor(&(rotor->N1_1),   SENSOR_TYPE_N1,  1);
    init_digital_sensor(&(rotor->EGT_A0), SENSOR_TYPE_EGT, 0);
    init_digital_sensor(&(rotor->EGT_A1), SENSOR_TYPE_EGT, 1);
    init_digital_sensor(&(rotor->EGT_B0), SENSOR_TYPE_EGT, 2);
    init_digital_sensor(&(rotor->EGT_B1), SENSOR_TYPE_EGT, 3);
}


void update_starter(Starter* starter, bool turn_on_cmd, double current_N)
{
    // Логика включения-выключения
    if (current_N >= starter->turnoff_N)
        starter->power = OFF;
    if (turn_on_cmd)
        starter->power = ON;

    // y[n+1] = k_yn * y[n] + k_u * u[n]  + ограничение момента
    starter->M = MIN(starter->M_max, starter->k_yn * starter->M + starter->k_u * starter->power);
}

void update_gas_generator(Gas_generator* ggen, bool ignition, double current_N)
{
    // Обновление параметра и датчика пламени в камере сгорания
    bool flame_went_out = (ggen->ignition && !ignition);
    ggen->ignition = ignition;
    update_discrete_sensor(&(ggen->flame_sensor), ignition);
    
    // Реализация транспортной задержки
    ggen->M = ggen->buffer[0];
    for (int i = 1; i < GGEN_BUFFER_SIZE; i++)
        ggen->buffer[i - 1] = ggen->buffer[i];

    // Линейное нарастание момента после достижения start_moment_N
    double M_new;
    if (current_N <= ggen->start_moment_N)
        M_new = 0;
    else if (current_N <= ggen->full_moment_N)
        M_new = ggen->M_max * (current_N - ggen->start_moment_N)
                / (ggen->full_moment_N - ggen->start_moment_N);
    else
        M_new = ggen->M_max;
    // При погасании пламени газогенератор теряет 80% момента, далее выбег
    if (flame_went_out)
        M_new = 0.2 * M_new;
    ggen->buffer[GGEN_BUFFER_SIZE - 1] = M_new;
}

void update_rotor(Rotor* rotor, double M_start, double M_ggen, double M_load)
{
    double M = M_start + M_ggen - M_load;

}