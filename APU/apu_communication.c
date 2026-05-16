#include "apu_communication.h"
#include "apu_defs.h"

void init_messages(Messages* msgs)
{
    msgs->eps.ext_power_avail       = 0;
    msgs->eps.gen_switch            = 0;
    msgs->eps.apb_stat              = 0;
    msgs->fs.fuel_avail             = 1;
    msgs->fs.low_pres_warn          = 0;
    msgs->air_cs.bleed_air_demand   = 0;
    msgs->air_cs.duct_pres          = 0;
    msgs->mpu.eng_start             = 0;
    msgs->mpu.eng_switch            = 0;
    msgs->mpu.eng_start_cutoff      = 0;
    msgs->fps.fire_sig              = 0;
    msgs->rcs.apu_power             = 0;
    msgs->rcs.test                  = 0;
    msgs->rcs.start_cmd             = 0;
    msgs->rcs.stop_cmd              = 0;
    msgs->auto_cs.auto_start        = 0;
}

void init_responses(Responses* rsps)
{
    rsps->eps.power_avail           = 0;
    rsps->fs.fuel_demand            = 0;
    rsps->air_cs.apu_bleed_avail    = 0;
    rsps->mpu.apu_bleed_avail       = 0;
    rsps->mpu.apu_bleed_valve_stat  = 0;
    rsps->mpu.crossbleed_valve_stat = 0;
    rsps->mpu.eng_bleed_valve_stat  = 0;
    rsps->mpu.apu_bleed_pres        = 0.0;
    rsps->rcs.power_on              = 0;
    rsps->rcs.apu_fault             = 0;
    rsps->rcs.critical_fault        = 0;
    rsps->rcs.start                 = 0;
    rsps->rcs.stop                  = 0;
    rsps->rcs.avail                 = 0;
    rsps->rcs.auto_shutdown         = 0;
    rsps->rcs.N1                    = 0.0;
    rsps->rcs.EGT                   = ATMOSPHERIC_TEMP;
}

void init_actions(Actions* acts)
{
    acts->power_on_cmd              = 0;
    acts->turn_on_cmd               = 0;
    acts->stop_testing              = 0;
    acts->abort_start               = 0;
    acts->pump_on                   = 0;
    acts->ignite                    = 0;
    acts->set_N                     = 0;
    acts->N_target                  = 0;
    acts->open_asv                  = 0;
    acts->close_asv                 = 0;
    acts->update_air_demand         = 0;
    acts->open_bsv                  = 0;
    acts->close_bsv                 = 0;
    acts->switch_fcv_to_acs         = 0;
    acts->switch_fcv_to_mpu         = 0;
    acts->switch_xbleed_to_left     = 0;
    acts->switch_xbleed_to_right    = 0;
    acts->update_gen_demand         = 0;
    acts->gen_demand                = 0;
    acts->mpu_start_cutoff_done     = 0;
    acts->turn_off_cmd              = 0;
    acts->emergency_shtdn           = 0;
    acts->power_off_cmd             = 0;
    acts->watchdog_shutdown         = 0;
}

void init_actions_manual(Actions_manual* actm)
{
    actm->power             = 0;
    actm->test              = 0;
    actm->reset             = 0;
    actm->start             = 0;
    actm->bleed             = 0;
    actm->gen               = 0;
    actm->mpu_start         = 0;
}

void init_data(Data* data)
{
    data->prev_state                = STATE_INDEFINITE;
    data->parent_state              = STATE_INDEFINITE;
    data->fault                     = 0;
    data->critical_fault            = 0;
    data->emergency_found           = 0;
    data->fire                      = 0;
    data->starting                  = 0;
    data->last_start_cmd            = 0;
    data->EGT_cur                   = ATMOSPHERIC_TEMP;
    data->N_cur                     = 0;
    data->ovheat                    = 0;
    data->last_ovheat_detected      = 0;
    data->demanded_fuel             = 0;
    data->last_demanded_fuel        = 0;
    data->ignited                   = 0;
    data->last_ignited              = 0;
    data->flame_was_on              = 0;
    data->last_flame_out            = 0;
    data->last_flame_out_updated    = 0;
    data->just_started_working      = 0;
    data->cooling                   = 0;
    data->last_cooldown_start       = 0;
}

void init_physical(Physical* phys)
{
    phys->auto_start = 0;
    phys->fire = 0;
    phys->height = 0;
    phys->ignited = 0;
    phys->enough_pressure = 0;
}

void reset_messages(Messages* msgs)
{
    msgs->eps.ext_power_avail       = 0;
    msgs->eps.gen_switch            = 0;
    msgs->eps.apb_stat              = 0;
    msgs->fs.fuel_avail             = 1;
    msgs->fs.low_pres_warn          = 0;
    msgs->air_cs.bleed_air_demand   = 0;
    msgs->air_cs.duct_pres          = 0;
    msgs->mpu.eng_start             = 0;
    msgs->mpu.eng_switch            = 0;
    msgs->mpu.eng_start_cutoff      = 0;
    //msgs->fps.fire_sig не сбрасывается
    msgs->rcs.apu_power             = 1;
    msgs->rcs.test                  = 0;
    msgs->rcs.start_cmd             = 0;
    msgs->rcs.stop_cmd              = 0;
    msgs->auto_cs.auto_start        = 0;
}

void copy_responses(Responses* original, Responses* copy)
{
    copy->eps.power_avail = original->eps.power_avail;
    copy->fs.fuel_demand = original->fs.fuel_demand;
    copy->air_cs.apu_bleed_avail = original->air_cs.apu_bleed_avail;
    copy->mpu.apu_bleed_avail = original->mpu.apu_bleed_avail;
    copy->mpu.apu_bleed_valve_stat = original->mpu.apu_bleed_valve_stat;
    copy->mpu.crossbleed_valve_stat = original->mpu.crossbleed_valve_stat;
    copy->mpu.eng_bleed_valve_stat = original->mpu.eng_bleed_valve_stat;
    copy->mpu.apu_bleed_pres = original->mpu.apu_bleed_pres;
    copy->rcs.power_on = original->rcs.power_on;
    copy->rcs.apu_fault = original->rcs.apu_fault;
    copy->rcs.critical_fault = original->rcs.critical_fault;
    copy->rcs.start = original->rcs.start;
    copy->rcs.stop = original->rcs.stop;
    copy->rcs.avail = original->rcs.avail;
    copy->rcs.auto_shutdown = original->rcs.auto_shutdown;
    copy->rcs.N1 = original->rcs.N1;
    copy->rcs.EGT = original->rcs.EGT;
}
