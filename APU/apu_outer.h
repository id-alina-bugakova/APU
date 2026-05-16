#pragma once

#include "apu_communication.h"
#include "apu_components.h"

void get_physical(Physical* phys, bool auto_start, int height, bool ignited, bool enough_pressure);


void by_eps(
    EPS_message* msg,
    EPS_response* rsp,
    Generator* gen,
    bool first_interaction,
    bool ext_power,
    bool gen_demand);

void by_fs(FS_message* msg, bool first_interaction, bool low_pressure);

void by_air_cs(
    AirCS_message* msg,
    AirCS_response* rsp,
    bool first_interaction,
    bool bleed_demand,
    bool high_pressure);

void by_mpu(
    MPU_message* msg,
    MPU_response* rsp,
    bool first_interaction,
    bool bleed_demand,
    bool engine);

void by_fps(FPS_message* msg, bool fire);

void by_rcs(
    RCS_message* msg,
    RCS_response* rsp,
    State state,
    bool first_interaction,
    bool manual,
    bool power,
    bool test,
    bool apu_demand);

void by_auto_cs(AutoCS_message* msg, bool auto_start);