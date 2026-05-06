#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "apu_state_machine.h"

// Получение данных от системы электроснабжения
typedef struct {
    bool ext_power_avail;
    bool gen_switch;
    bool apb_stat;
} EPS_message;

typedef struct {
    bool power_avail;
} EPS_response;

// Топливная система
typedef struct {
    bool fuel_avail;
    bool low_pres_warn;
} FS_message;

typedef struct {
    bool fuel_demand;
} FS_response;

// СКВ
typedef struct {
    bool bleed_air_demand;
    bool duct_pres;
} AirCS_message;

typedef struct {
    bool apu_bleed_avail;
} AirCS_response;

// МСУ
typedef struct {
    bool eng_start;
    bool eng_switch; // 0l 1r
    bool eng_start_cutoff;
} MPU_message;

typedef struct {
    bool apu_bleed_avail;
    bool apu_bleed_valve_stat;
    bool crossbleed_valve_stat;
    bool eng_bleed_valve_stat;
    double apu_bleed_pres;
} MPU_response;

// СПЗ
typedef struct {
    bool fire_sig;
} FPS_message;

// СДУ
typedef struct {
    bool apu_power;
    bool test;
    bool start_cmd;
    bool stop_cmd;
} RCS_message;

typedef struct {
    bool power_on;
    bool apu_fault;
    bool critical_fault;
    bool start;
    double N1;
    double EGT;
    bool avail;
    bool auto_shutdown;
} RCS_response;

// САУ
typedef struct {
    bool auto_start;
} AutoCS_message;

// Общие
typedef struct {
    EPS_message eps;
    FS_message fs;
    AirCS_message air_cs;
    MPU_message mpu;
    FPS_message fps;
    RCS_message rcs;
    AutoCS_message auto_cs;
} Messages;

typedef struct {
    EPS_response eps;
    FS_response fs;
    AirCS_response air_cs;
    MPU_response mpu;
    RCS_response rcs;
} Responses;

typedef struct {
    bool turn_on_cmd;
    bool ignite;
    bool set_N;
    double N_target;
    bool update_air_demand;
    bool open_asv;
    bool open_bsv;
    bool close_bsv;
    bool switch_fcv_to_acs;
    bool switch_fcv_to_mpu;
    bool switch_xbleed_to_left;
    bool switch_xbleed_to_right;
    bool update_gen_demand;
    bool gen_demand;
    bool turn_off_cmd;
} Actions;

typedef struct {
    State prev_state;
    State parent_state;
    bool fault;
    bool critical_fault;
    bool emergency_found;
    bool fire;
    bool starting;
    uint32_t last_start_cmd;
    double EGT_cur;
    double N_cur;
    bool ovheat;
    uint32_t last_ovheat_detected;
    bool demanded_fuel;
    uint32_t last_demanded_fuel;
    bool ignited;
    bool cooling;
    uint32_t last_cooldown_start;
} Data;

typedef struct {
    int height;
    bool ignited;
    bool enough_pressure;
} Physical;

void init_messages(Messages* msgs);

void init_responses(Responses* rsps);

void init_actions(Actions* acts);

void init_data(Data* data);

void init_physical(Physical* phys);