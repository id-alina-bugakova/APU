#pragma once

#include "apu_components.h"

typedef struct {
    int id;                             ///< Идентификатор контроллера
    bool power;                         ///< Признак питания (0 - нет питания, 1 - есть)
    bool main_channel;                  ///< Признак работы на главном канале (0 - резерв)
    bool fault;                         ///< Признак неисправности (0 - исправен, 1 - неисправен)
    unsigned long long last_updated;    ///< Время последнего обновления (в тактах)
    unsigned long long tolerance;       ///< Время, после которого контроллер считается "упавшим"
} ECU;

void init_controller(ECU* contr, int id, bool main_channel);

void update_controller(
    Starter* start,
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Cooling_fan* fan,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys);