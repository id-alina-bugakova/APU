#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>

#include "apu_defs.h"
#include "apu_diagnostic.h"

void init_problem_notifications(Problem_notifications* ntfs)
{
    ntfs->c_flame_sensor = 0;
    ntfs->c_2_N1 = 0;
    ntfs->c_4_EGT = 0;
    ntfs->c_P3 = 0;
    ntfs->c_T3 = 0;
    ntfs->c_pump = 0;
    ntfs->c_fuel_sov = 0;
    ntfs->c_fuel_sov_sensor = 0;
    ntfs->c_asv = 0;
    ntfs->c_asv_sensor = 0;
    ntfs->c_bsv = 0;
    ntfs->c_bsv_sensor = 0;
    ntfs->c_fcv = 0;
    ntfs->c_fcv_sensor = 0;
    ntfs->c_xbleed = 0;
    ntfs->c_xbleed_sensor = 0;
    ntfs->N1_0 = 0;
    ntfs->N1_1 = 0;
    ntfs->EGT_A0 = 0;
    ntfs->EGT_A1 = 0;
    ntfs->EGT_B0 = 0;
    ntfs->EGT_B1 = 0;
    ntfs->fan = 0;
    ntfs->NGC = 0;
    ntfs->P_fuel = 0;
    ntfs->asv = 0;
    ntfs->P2 = 0;
    ntfs->T2 = 0;
    ntfs->fcv = 0;
    ntfs->P_duct = 0;
    ntfs->T_duct = 0;
}

void wellness_check(
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
        pump->fault ||
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

void wellness_check_verbose(
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Cooling_fan* fan,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys,
    int controller_id, 
    uint32_t cur_time,
    Data* data,
    Problem_notifications* ntfs,
    Message_buffer* mb, 
    File_output* fout)
{
    char temp_str[STRING_LEN];
    // Критические
    if (!ntfs->c_flame_sensor && ggen->flame_sensor.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - flame sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_flame_sensor = 1;
    }
    if (!ntfs->c_2_N1 && rotor->N1_0.fault && rotor->N1_1.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - both N1 sensors out.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_2_N1 = 1;
    }
    if (!ntfs->c_4_EGT && rotor->EGT_A0.fault && rotor->EGT_A1.fault &&
        rotor->EGT_B0.fault && rotor->EGT_B1.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - all 4 EGT sensors out.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_4_EGT = 1;
    }
    if (!ntfs->c_P3 && comp->P3.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - P3 sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_P3 = 1;
    }
    if (!ntfs->c_T3 && comp->T3.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - T3 sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_T3 = 1;
    }
    if(!ntfs->c_pump && pump->fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - fuel pump fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_pump = 1;
    }
    if(!ntfs->c_fuel_sov && pump->fuel_sov.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - fuel SOV fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_fuel_sov = 1;
    }
    if (!ntfs->c_fuel_sov_sensor && pump->fuel_sov.sensor.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - fuel SOV position sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_fuel_sov_sensor = 1;
    }
    if (!ntfs->c_asv && psys->asv.fault && psys->asv.open == 0)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - ASV jam (closed).\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_asv = 1;
    }
    if (!ntfs->c_asv_sensor && psys->asv.sensor.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - ASV position sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_asv_sensor = 1;
    }
    if (!ntfs->c_bsv && psys->bsv.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - BSV fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_bsv = 1;
    }
    if (!ntfs->c_bsv_sensor && psys->bsv.sensor.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - BSV position sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_bsv_sensor = 1;
    }
    if (!ntfs->c_fcv && psys->fcv.fault && psys->fcv.open == 1)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - FCV jam (switched to ACS).\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_fcv = 1;
    }
    if (!ntfs->c_fcv_sensor && psys->fcv.sensor.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - FCV position sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_fcv_sensor = 1;
    }
    if (!ntfs->c_xbleed && psys->mpu_xbleed.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - MPU Xbleed valve jam.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_xbleed = 1;
    }
    if (!ntfs->c_xbleed_sensor && psys->mpu_xbleed.sensor.fault)
    {
        data->critical_fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Critical problem detected - MPU Xbleed valve position sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->c_xbleed_sensor = 1;
    }

    // Некритические
    if(data->critical_fault)
        data->fault = 1;
    if (ntfs->N1_0 && !ntfs->c_2_N1 && rotor->N1_0.fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - N1 sensor 0 fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->N1_0 = 1;
    }
    if (ntfs->N1_1 && !ntfs->c_2_N1 && rotor->N1_1.fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - N1 sensor 1 fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->N1_1 = 1;
    }
    if (!ntfs->EGT_A0 && !ntfs->c_4_EGT && rotor->EGT_A0.fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - EGT sensor A0 fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->EGT_A0 = 1;
    }
    if (!ntfs->EGT_A1 && !ntfs->c_4_EGT && rotor->EGT_A1.fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - EGT sensor A1 fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->EGT_A1 = 1;
    }
    if (!ntfs->EGT_B0 && !ntfs->c_4_EGT && rotor->EGT_B0.fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - EGT sensor B0 fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->EGT_B0 = 1;
    }
    if (!ntfs->EGT_B1 && !ntfs->c_4_EGT && rotor->EGT_B1.fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - EGT sensor B1 fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->EGT_B1 = 1;
    }
    if (!ntfs->fan && fan->fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - cooling fan fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->fan = 1;
    }
    if (!ntfs->NGC && gen->NGC.fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - generator NGC sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->NGC = 1;
    }
    if (!ntfs->P_fuel && pump->P_fuel.fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - fuel pressure sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->P_fuel = 1;
    }
    if (!ntfs->asv && psys->asv.fault && psys->asv.open == 1)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - ASV jam (open).\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->asv = 1;
    }
    if (!ntfs->P2 && psys->P2.fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - P2 sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->P2 = 1;
    }
    if (!ntfs->T2 && psys->T2.fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - T2 sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->T2 = 1;
    }
    if (!ntfs->fcv && psys->fcv.fault && psys->fcv.open == 0)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - FCV jam (switched to MPU).\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->fcv = 1;
    }
    if (!ntfs->P_duct && psys->P_duct.fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - duct pressure sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->P_duct = 1;
    }
    if (!ntfs->T_duct && psys->T_duct.fault)
    {
        data->fault = 1;
        sprintf(
            &temp_str,
            "Controller %d at %7.2f : Problem detected - duct temperature sensor fault.\n",
            controller_id, TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        ntfs->T_duct = 1;
    }
}

void reset_fault(
    Messages* msgs,
    Data* data,
    Problem_notifications* ntfs,
    Gas_generator* ggen,
    Rotor* rotor,
    Cooling_fan* fan,
    Generator* gen,
    Compressor* comp,
    Fuel_pump* pump,
    Pneumatic_system* psys)
{
    // Сброс отказов
    msgs->fps.fire_sig = 0;         // + сброс пожарной сигнализации
    ggen->flame_sensor.fault = 0;
    rotor->N1_0.fault = 0;          rotor->N1_1.fault = 0;
    rotor->EGT_A0.fault = 0;        rotor->EGT_A1.fault = 0;
    rotor->EGT_B0.fault = 0;        rotor->EGT_B1.fault = 0;
    fan->fault = 0;
    gen->NGC.fault = 0;
    comp->P3.fault = 0;             comp->T3.fault = 0;
    pump->fault = 0;
    pump->fuel_sov.fault = 0;       pump->fuel_sov.sensor.fault = 0;
    pump->P_fuel.fault = 0;
    psys->asv.fault = 0;            psys->asv.sensor.fault = 0;
    psys->P2.fault = 0;             psys->T2.fault = 0;
    psys->bsv.fault = 0;            psys->bsv.sensor.fault = 0;
    psys->P_duct.fault = 0;         psys->T_duct.fault = 0;
    psys->fcv.fault = 0;            psys->fcv.sensor.fault = 0;
    psys->mpu_xbleed.fault = 0;     psys->mpu_xbleed.sensor.fault = 0;

    // Сброс данных об отказах
    data->fault = 0;
    data->critical_fault = 0;

    // Сброс уведомлений об отказах
    init_problem_notifications(ntfs);
}