/* @file   apu_outer.c
*  @brief  Код функций имитации внешних систем
*
*  @detail Файл содержит код функций, принимающих базовые решения об управлении на основе
*  получаемых ответов и физичсеких признаков.
*
*  @author Бугакова А.А.
*/


#include "apu_outer.h"
#include "apu_components.h"


void get_physical(Physical* phys, bool auto_start, int height, bool ignited, bool enough_pressure)
{
    if (auto_start)
        auto_start = 1;
    phys->auto_start = auto_start;
    phys->height = height;
    phys->ignited = ignited;
    phys->enough_pressure = enough_pressure;
}


void by_eps(
    EPS_message* msg, 
    EPS_response* rsp, 
    Generator* gen, 
    bool first_interaction, 
    bool ext_power,
    bool gen_demand)
{
    // Первым сообщением, при подключении внешнего источника или если не требуется генератор
    if (first_interaction || ext_power || !gen_demand)
    {
        msg->ext_power_avail = ext_power;
        msg->gen_switch = 0;
        msg->apb_stat = 0;
    }
    else
    {
        // Если хотим подключиться к генератору и можно это сделать
        if (gen_demand && rsp->power_avail && !gen->on)
        {
            msg->ext_power_avail = 0;
            msg->gen_switch = 1;
            msg->apb_stat = 0;
        }
        // Когда подключились к генератору, сообщаем "генератор на шине"
        else if (gen_demand && gen->on)
        {
            msg->ext_power_avail = 0;
            msg->gen_switch = 1;
            msg->apb_stat = 1;
        }
    }
}

void by_fs(FS_message* msg, bool first_interaction, bool low_pressure)
{
    // Первым сообщением
    if (first_interaction)
    {
        msg->fuel_avail = 1;
        msg->low_pres_warn = 0;
    }
    // Если низкое давление
    else if (low_pressure)
    {
        msg->fuel_avail = 0;
        msg->low_pres_warn = 1;
    }
    // Ответ ВСУ в данном случае не влияет - требуется только ТС для расчета
}

void by_air_cs(
    AirCS_message* msg, 
    AirCS_response* rsp, 
    bool first_interaction, 
    bool bleed_demand, 
    bool high_pressure)
{
    // Первым сообщением, если высокое давление или нет запроса на воздух
    if (first_interaction || high_pressure || !bleed_demand)
    {
        msg->bleed_air_demand = 0;
        msg->duct_pres = high_pressure;
    }
    else
    {
        // Если доступна подача воздуха и есть запрос на подачу
        if (rsp->apu_bleed_avail && bleed_demand)
        {
            msg->bleed_air_demand = 1;
            msg->duct_pres = 0;
        }
    }
}

void by_mpu(
    MPU_message* msg, 
    MPU_response* rsp, 
    bool first_interaction, 
    bool bleed_demand, 
    bool engine)
{
    // Первым сообщением
    if (first_interaction)
    {
        msg->eng_start = 0;
        msg->eng_switch = engine;
        msg->eng_start_cutoff = 0;
    }
    else
    {
        // Если доступен воздух и есть достаточное давление и заявка на подачу
        if (rsp->apu_bleed_avail && rsp->apu_bleed_pres && bleed_demand)
        {
            msg->eng_start = 1;
            msg->eng_switch = engine;
            msg->eng_start_cutoff = 0;
        }
        // Если низкое даление или запуск завершен
        else if (!rsp->apu_bleed_pres ||
            (!bleed_demand && rsp->apu_bleed_valve_stat && rsp->crossbleed_valve_stat))
        {
            msg->eng_start = 0;
            msg->eng_switch = engine;
            msg->eng_start_cutoff = 1;
        }
    }
}

void by_fps(FPS_message* msg, bool fire)
{
    msg->fire_sig = fire;
}

void by_rcs(
    RCS_message* msg, 
    RCS_response* rsp, 
    State state,
    bool first_interaction,
    bool manual,
    bool power, 
    bool test)
{
    // Первым сообщением или при аварийном отключении
    if (first_interaction || rsp->auto_shutdown)
    {
        msg->apu_power = 0;
        msg->test = 0;
        msg->start_cmd = 0;
        msg->stop_cmd = 0;
    }
    else
    {
        if (power)
            msg->apu_power = 1;
        // Если питания нет/пропало
        else
        {
            msg->apu_power = 0;
            // Если питания и не было
            if (state == STATE_OFF)
            {
                msg->test = 0;
                msg->start_cmd = 0;
                msg->stop_cmd = 0;
            }
            // Если питание пропало
            else
            {
                msg->test = 0;
                msg->start_cmd = 0;
                msg->stop_cmd = 1;
            }
        }
    }
}

void by_auto_cs(AutoCS_message* msg, bool auto_start)
{
    msg->auto_start = auto_start;
}
