#include "apu_driver.h"
#include "apu_diagnostic.h"

void toggle_components(
    ECU* c0,
    ECU* c1,
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys,
    bool value)
{
    c0->power = value;
    c1->power = value;
    ggen->flame_sensor.power = value;
    rotor->N1_0.power = value;
    rotor->N1_1.power = value;
    rotor->EGT_A0.power = value;
    rotor->EGT_A1.power = value;
    rotor->EGT_B0.power = value;
    rotor->EGT_B1.power = value;
    comp->P3.power = value;
    comp->T3.power = value;
    gen->NGC.power = value;
    pump->fuel_sov.power = value;
    pump->fuel_sov.sensor.power = value;
    psys->asv.power = value;
    psys->asv.sensor.power = value;
    psys->bsv.power = value;
    psys->bsv.sensor.power = value;
    psys->fcv.power = value;
    psys->fcv.sensor.power = value;
    psys->mpu_xbleed.power = value;
    psys->mpu_xbleed.sensor.power = value;
    psys->P2.power = value;
    psys->T2.power = value;
    psys->P_duct.power = value;
    psys->T_duct.power = value;
}

void perform_manual_actions(
    Actions_manual* actm,
    Physical* phys,
    Messages* msgs,
    Data* data,
    Problem_notifications* ntfs,
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
        reset_fault(msgs, data, ntfs, ggen, rotor, fan, gen, comp, pump, psys);
        actm->reset = 0;
    }

    if ((actm->start || phys->auto_start) && state == STATE_IDLE)
    {
        msgs->rcs.start_cmd = 1;
        msgs->rcs.stop_cmd = 0;
    }
    else if (!actm->start && !phys->auto_start &&
        (state == STATE_START ||
        state == STATE_IDLE_RUN || state == STATE_IDLE_RUN_LIMITED))
    {
        msgs->rcs.start_cmd = 0;
        msgs->rcs.stop_cmd = 1;
        phys->auto_start = 0;
        msgs->auto_cs.auto_start = 0;
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


    if (phys->fire)
        msgs->fps.fire_sig = 1;

    if (!phys->ignited)
        ggen->ignition = OFF;

    if (!phys->enough_pressure)
        msgs->mpu.eng_start_cutoff;
}

void perform_actions(
    Actions* acts,
    Messages* msgs,
    Data* data,
    Actions_manual* actm,
    Physical* phys,
    State* state,
    ECU* c0,
    ECU* c1,
    Starter* start,
    PID* pid,
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys)
{
    if (acts->power_on_cmd)
    {
        reset_messages(msgs);
        init_data(data);
        toggle_components(c0, c1, ggen, rotor, comp, gen, pump, psys, ON);
    }

    if (acts->turn_on_cmd)
    {
        start->power = ON;
        init_PID(pid);
    }

    if (acts->stop_testing)
    {
        actm->test = 0;
        msgs->rcs.test = 0;
    }

    if (acts->abort_start)
    {
        actm->start = 0;
        msgs->rcs.start_cmd = 0;
        start->power = OFF;
        ggen->ignition = OFF;
        pump->on = 0;
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
        {
            comp->bleed = 1;
            psys->bsv.open_cmd = 1;
        }
        else if (acts->close_bsv)
        {
            comp->bleed = 0;
            psys->bsv.close_cmd = 1;
        }

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

    if (acts->watchdog_shutdown || acts->emergency_shtdn || acts->power_off_cmd)
    {
        // Убираем команды на старт
        msgs->auto_cs.auto_start = 0;
        phys->auto_start = 0;
        msgs->rcs.start_cmd = 0;
        msgs->rcs.apu_power = OFF;
        actm->power = OFF;
        actm->start = 0;
        // Закрываем клапаны, выключаем зажигание
        if(psys->asv.power && !psys->asv.fault)
            psys->asv.open = CLOSED;
        if (psys->bsv.power && !psys->bsv.fault)
            psys->bsv.open = CLOSED;
        ggen->ignition = OFF;
        // Все выключаем
        toggle_components(c0, c1, ggen, rotor, comp, gen, pump, psys, OFF);
    }

    if (acts->watchdog_shutdown)
        *state = STATE_OFF;

    if (phys->auto_start)
    {
        // Выполняем включение всех агрегатов и сброс сообщений об отказах
        if (actm->power == OFF)
        {
            actm->power = ON;
            reset_messages(msgs);
            init_data(data);
        }
        toggle_components(c0, c1, ggen, rotor, comp, gen, pump, psys, ON);
        msgs->rcs.apu_power = ON;
        msgs->auto_cs.auto_start = 1;
        msgs->rcs.stop_cmd = 0;

    }
}
