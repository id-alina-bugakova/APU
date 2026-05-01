#pragma once

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
} ACS_message;

// МСУ
typedef struct {
    bool eng_start;
    bool eng_start_cmd;
    bool eng_start_cutoff;
} MPU_message;

typedef struct {
    bool start_inhibit_sig;
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

// СУВШ
typedef struct {
    bool weight_on_wheels;
} LGERS_message;

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
    bool start;
    double N1;
    double EGT;
    bool avail;
    bool auto_shutdown;
} RCS_response;

// САУ
typedef struct {
    bool auto_start;
} ACS_message;

// Общие
typedef struct {
    EPS_message eps;
    FS_message fs;
    ACS_message acs;
    MPU_message mpu;
    FPS_message fps;
    LGERS_message lgers;
    RCS_message rcs;
    ACS_message acs;
} Messages;

typedef struct {
    EPS_response eps;
    FS_response fs;
    MPU_response mpu;
    RCS_response rcs;
} Responses;