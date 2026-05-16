#define _CRT_SECURE_NO_WARNINGS

#include "apu_scenarios.h"
#include "apu_interface.h"
#include <string.h>
#include <stdio.h>

void init_scenarios(Scenarios* scns)
{
    for (int i = 0; i < N_SCENARIOS; i++)
        sprintf(&(scns->path[i]), "scenarios/%d.sc", i);
    scns->pos = 0;
    scns->finished = 0;
}

void start_scenario(int n, Scenarios* scns, Keys* k)
{
    FILE* reader = fopen(scns->path[n], "r");
    if (n == 0)
    {

    }
    else
    {
        Sc_commands cmds;

        int scanned = 0;
        while (true)
        {
            fseek(reader, scns->pos, SEEK_SET);
            scanned = fscanf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%c",
                &(cmds.power), &(cmds.test), &(cmds.start_stop), &(cmds.bleed),
                &(cmds.gen), &(cmds.mpu), &(cmds.fire), &(cmds.height),
                &(cmds.ignition), &(cmds.enough_pres), &(cmds.c0), &(cmds.c1),
                &(cmds.faults));
            // Если дочитали до конца файла
            if (scanned == EOF)
            {
                fclose(reader);
                scns->finished = 1;
                break;
            }
            if (cmds.power)
                k->key1 = 'p';
            scns->pos = ftell(reader);
        }

    }
}

