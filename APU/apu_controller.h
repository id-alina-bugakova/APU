#pragma once

#include "apu_components.h"
#include "apu_communication.h"

#include "stdbool.h"

typedef struct {
    int id;                             ///< Идентификатор контроллера
    bool power;                         ///< Признак питания (0 - нет питания, 1 - есть)
    bool main_channel;                  ///< Признак работы на главном канале (0 - резерв)
    bool fault;                         ///< Признак неисправности (0 - исправен, 1 - неисправен)
    unsigned long long last_updated;    ///< Время последнего обновления (в тактах)
    unsigned long long tolerance;       ///< Время, после которого контроллер считается "упавшим"

    bool problems_found;                ///< В ходе теста обнаружены неполадки
    bool fire;                          ///< Обнаружен пожар в отсеке ВСУ
} ECU;

void init_controller(ECU* contr, int id, bool main_channel);

double calculate_EGT(Rotor* rotor);

double calculate_N(Rotor* rotor);

void check_N_EGT(uint32_t cur_time, Rotor* rotor, Responses* rsps, Data* data);

void check_for_emergencies(
    ECU* c1, 
    uint32_t cur_time, 
    Event* last_event, 
    Rotor* rotor,
    Gas_generator* ggen,
    Responses* rsps, 
    Data* data);

void check_for_emergencies_emergent(
    ECU* c1,
    uint32_t cur_time,
    Event* last_event,
    Rotor* rotor,
    Gas_generator* ggen,
    Messages* msgs,
    Responses* rsps,
    Data* data);

void update_controller(
    ECU* c1,
    ECU* c2,
    uint32_t cur_time,
    State state,
    Event last_event,
    Starter* start,
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Cooling_fan* fan,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys,
    Messages* msgs,
    Responses* rsps,
    Actions* acts,
    Data* data,
    Physical* phys);