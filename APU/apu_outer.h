#pragma once

#include "apu_communication.h"

void get_physical(Physical* phys);

void by_eps(EPS_message* msg);
void by_fs(FS_message* msg);
void by_air_cs(AirCS_message* msg);
void by_mpu(MPU_message* msg);
void by_fps(FPS_message* msg);
void by_rcs(RCS_message* msg);
void by_auto_cs(AutoCS_message* msg);