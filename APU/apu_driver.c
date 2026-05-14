#include "apu_driver.h"

void reset_fault(
    Messages* msgs,
    Gas_generator* ggen,
    Rotor* rotor,
    Cooling_fan* fan,
    Generator* gen,
    Compressor* comp,
    Fuel_pump* pump,
    Pneumatic_system* psys)
{
    msgs->fps.fire_sig = 0;
    ggen->flame_sensor.fault = 0;
    rotor->N1_0.fault = 0;          rotor->N1_1.fault = 0;
    rotor->EGT_A0.fault = 0;        rotor->EGT_A1.fault = 0;
    rotor->EGT_B0.fault = 0;        rotor->EGT_B1.fault = 0;
    fan->fault = 0;
    gen->NGC.fault = 0;
    comp->P3.fault = 0;             comp->T3.fault = 0;
    pump->fuel_sov.fault = 0;       pump->fuel_sov.sensor.fault = 0;
    pump->P_fuel.fault = 0;
    psys->asv.fault = 0;            psys->asv.sensor.fault = 0;
    psys->P2.fault = 0;             psys->T2.fault = 0;
    psys->bsv.fault = 0;            psys->bsv.sensor.fault = 0;
    psys->P_duct.fault = 0;         psys->T_duct.fault = 0;
    psys->fcv.fault = 0;            psys->fcv.sensor.fault = 0;
    psys->mpu_xbleed.fault = 0;     psys->mpu_xbleed.sensor.fault = 0;
}

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
    Pneumatic_system* psys)
{
    msgs->rcs.apu_power = actm->power;
    c0->power = actm->power;
    c1->power = actm->power;

    msgs->rcs.test = actm->test;
    actm->test = 0;

    if (actm->reset)
    {
        reset_fault(msgs, ggen, rotor, fan, gen, comp, pump, psys);
        actm->reset = 0;
    }

    if (actm->start && state == STATE_IDLE)
    {
        msgs->rcs.start_cmd = 1;
        msgs->rcs.stop_cmd = 0;
    }
    else if (!actm->start && (state == STATE_START ||
        state == STATE_IDLE_RUN || state == STATE_IDLE_RUN_LIMITED))
    {
        msgs->rcs.start_cmd = 0;
        msgs->rcs.stop_cmd = 1;
        start->turn_off_cmd = 1;
    }

    msgs->air_cs.bleed_air_demand = actm->bleed;

    msgs->eps.gen_switch = actm->gen;

    if (actm->mpu_start && state != STATE_MPU_START && state != STATE_MPU_START_LIMITED)
    {
        msgs->mpu.eng_start = 1;
        msgs->mpu.eng_start_cutoff = 0;
    }
    else if (!actm->mpu_start && (state == STATE_MPU_START || state == STATE_MPU_START_LIMITED))
    {
        msgs->mpu.eng_start = 0;
        msgs->mpu.eng_start_cutoff = 1;
    }
    else
    {
        msgs->mpu.eng_start = 0;
        msgs->mpu.eng_start_cutoff = 0;
    }
}

void perform_actions(
    Actions* acts,
    Messages* msgs,
    Actions_manual* actm,
    Starter* start,
    Gas_generator* ggen,
    Rotor* rotor,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys)
{
    if (acts->turn_on_cmd)
        start->power = ON;

    if (acts->stop_testing)
    {
        actm->test = 0;
        msgs->rcs.test = 0;
    }

    if (acts->abort_start)
    {
        actm->start = 0;
        msgs->rcs.start_cmd = 0;
    }

    if (acts->pump_on)
        pump->turn_on_cmd = 1;

    if (acts->ignite)
        ggen->ignition = ON;

    if (acts->set_N)
        rotor->N_target = acts->N_target;

    if (acts->open_asv)
        psys->asv.open_cmd = 1;
    else if (acts->close_asv)
        psys->asv.close_cmd = 1;

    if (acts->update_air_demand)
    {
        if (acts->open_bsv)
            psys->bsv.open_cmd = 1;
        else if (acts->close_bsv)
            psys->bsv.close_cmd = 1;

        if (acts->switch_fcv_to_acs)
            psys->fcv.open_cmd = 1;
        else if (acts->switch_fcv_to_mpu)
            psys->fcv.close_cmd = 1;

        if (acts->switch_xbleed_to_left)
            psys->mpu_xbleed.close_cmd = 1;
        else if (acts->switch_xbleed_to_right)
            psys->mpu_xbleed.open_cmd = 1;
    }

    if (acts->update_gen_demand)
        gen->on = acts->gen_demand;

    if (acts->mpu_start_cutoff_done)
        msgs->mpu.eng_start_cutoff = 0;

    if (acts->turn_off_cmd)
        ggen->ignition = OFF;
}
