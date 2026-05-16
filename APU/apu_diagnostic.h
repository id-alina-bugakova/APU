#pragma once
#include "apu_communication.h"
#include "apu_components.h"
#include "apu_interface.h"

typedef struct {
    bool c_flame_sensor;
    bool c_2_N1;
    bool c_4_EGT;
    bool c_P3;
    bool c_T3;
    bool c_pump;
    bool c_fuel_sov;
    bool c_fuel_sov_sensor;
    bool c_asv;
    bool c_asv_sensor;
    bool c_bsv;
    bool c_bsv_sensor;
    bool c_fcv;
    bool c_fcv_sensor;
    bool c_xbleed;
    bool c_xbleed_sensor;
    bool N1_0;
    bool N1_1;
    bool EGT_A0;
    bool EGT_A1;
    bool EGT_B0;
    bool EGT_B1;
    bool fan;
    bool NGC;
    bool P_fuel;
    bool asv;
    bool P2;
    bool T2;
    bool fcv;
    bool P_duct;
    bool T_duct;
} Problem_notifications;

void init_problem_notifications(Problem_notifications* ntfs);

void wellness_check(
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Cooling_fan* fan,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys,
    Messages* msgs,
    Data* data);

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
    File_output* fout);

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
    Pneumatic_system* psys);