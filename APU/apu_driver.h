#pragma once

#include "apu_communication.h"
#include "apu_components.h"
#include "apu_controller.h"
#include "apu_defs.h"

void reset_fault(
    Messages* msgs,
    Gas_generator* ggen,
    Rotor* rotor,
    Cooling_fan* fan,
    Generator* gen,
    Compressor* comp,
    Fuel_pump* pump,
    Pneumatic_system* psys);

void perform_manual_actions(
    Actions_manual* actm,
    Messages* msgs,
    State state,
    ECU* c0,
    ECU* c1,
    Starter* start,
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Cooling_fan* fan,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys);

void perform_actions(
    Actions* acts,
    Messages* msgs,
    Actions_manual* actm,
    Starter* start,
    Gas_generator* ggen,
    Rotor* rotor,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys);