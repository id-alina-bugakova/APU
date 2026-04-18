#pragma once

#include <stdbool.h>

#include "apu_sensors.h"

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