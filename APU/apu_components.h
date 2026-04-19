#pragma once

#define GGEN_BUFFER_SIZE 5

#include <stdbool.h>

#include "apu_sensors.h"


/* @brief  Электростартер
* 
*  Инерционное звено 1-го порядка с разностным уравнением y[n+1] = k_yn * y[n] + k_u * u[n], 
*  где k_yn = 0.9934, k_u = 0.3987. 
*  Раскручивает ротор до target_N (6000) оборотов, после чего отключается.
*  Имеет ограничение момента M_max (60) Н*м. 
*  Питается от электросети постоянного тока.
*/
typedef struct
{
    bool power;                         ///< Признак питания (0 - нет, 1 - есть)
    double k_yn;                        ///< Коэффициент y[n] в разностном уравнении
    double k_u;                         ///< Коэффициент u[n] в разностном уравнении
    double M;                           ///< Момент, создаваемый стартером, Н*м
    double M_max;                       ///< Максимальный момент, который может создать стартер
    double turnoff_N;                   ///< N, при котором стартер выключается
} Starter;

/* @brief Газогенератор
* 
*  Газогенератор не вырабатывает собственный момент до достижения start_moment_N оборотов.
*  По достижении этого значения момент увеличивается линейно с ростом оборотов, пока не достигнет
*  M_max Н*м при full_moment_N оборотах.  
*  При погасании факела в камере сгорания выход падает на 80%, после чего происходит выбег.
*  У газогенератора присутствует транспортная задержка 0.1 с (5 тактов) на доставку топлива 
*  в камеру сгорания.
*  Имеет ограничение момента M_max (250) Н*м. 
*/
typedef struct
{
    bool ignition;                      ///< Признак горения топлива (0 - нет, 1 - есть)
    Discrete_sensor flame_sensor;       ///< Датчик факела в камере сгорания
    double buffer[GGEN_BUFFER_SIZE];    ///< Буфер транспортной задержки
    double M;                           ///< Момент, создаваемый стартером, Н*м
    double M_max;                       ///< Максимальный момент, который может создать стартер
    double start_moment_N;              ///< N начала выработки собственного момента
    double full_moment_N;               ///< N достижения полного момента
} Gas_generator;

typedef struct
{
    double k;                           ///< Коэффициент усиления интегратора
    double N;                           ///< Обороты ротора в об./мин.
    double N_work;                      ///< Обороты, приравниваемые к 100% (рабочий режим)
    Digital_sensor N1_0;                ///< Датчик частоты вращения ротора (основной)
    Digital_sensor N1_1;                ///< Датчик частоты вращения ротора (резервный)
    Digital_sensor EGT_A0;              ///< Датчик температуры выхлопных газов (1, канал 1)
    Digital_sensor EGT_A1;              ///< Датчик температуры выхлопных газов (2, канал 1)
    Digital_sensor EGT_B0;              ///< Датчик температуры выхлопных газов (1, канал 2)
    Digital_sensor EGT_B1;              ///< Датчик температуры выхлопных газов (2, канал 2)
} Rotor;


/* @brief  Клапан
*
*  Хранит признаки питмания, положения, заклинивания и связанный датчик положения
*/
typedef struct
{
    bool power;                ///< Признак питания (0 - нет, 1 - есть)
    bool open;                 ///< Признак положения (0 - закрыт, 1 - открыт)
    bool stuck;                ///< Признак заклинивания (0 - норма, 1 - заклинил)
    Discrete_sensor sensor;    ///< Датчик положения клапана
} Valve;

typedef struct
{
    bool power;
    Valve fuel_sov;
    bool active;
    Digital_sensor P_fuel;
} Fuel_pump;

void init_starter(Starter* starter);
void init_gas_generator(Gas_generator* ggen);
void init_rotor(Rotor* rotor);

void update_starter(Starter* starter, bool turn_on_cmd, double current_N);
void update_gas_generator(Gas_generator* ggen, bool ignition, double current_N);
void update_rotor(Rotor* rotor, double M_start, double M_ggen, double M_load);