/* @file   apu_watchdog.c
*  @brief  Реализация сторожевого таймера
*
*  @detail Файл содержит код функций инициализации и обновления сторожевого таймера
*
*  @author Бугакова А.А.
*/


#include "apu_watchdog.h"
#include "apu_communication.h"
#include "apu_defs.h"


void init_watchdog(Watchdog* wd)
{
    wd->tolerance          = 6;
    wd->initiated_shutdown = 0;
}

void update_watchdog(
    Watchdog* wd,
    uint32_t cur_time, 
    ECU* c0, 
    ECU* c1, 
    Actions* acts, 
    Message_buffer* mb, 
    File_output* fout)
{
    bool c0_fail = 0;
    bool c1_fail = 0;
    if (c0->power && cur_time > c0->last_updated + wd->tolerance)
        c0_fail = 1;
    else
        c0_fail = 0;
    if (c1->power && cur_time > c1->last_updated + wd->tolerance)
        c1_fail = 1;
    else
        c1_fail = 0;

    if (c0_fail && c1_fail && !wd->initiated_shutdown)
    {
        char temp_str[STRING_LEN];
        sprintf(&temp_str, "WATCHDOG     at %7.2f : Loss of control. Initiating shutdown.\n", TIME(cur_time));
        print_to_buffer(mb, temp_str, fout);
        acts->watchdog_shutdown = 1;
        wd->initiated_shutdown  = 1;
    }
    else if (!c0_fail || !c1_fail)
        wd->initiated_shutdown = 0;
}
