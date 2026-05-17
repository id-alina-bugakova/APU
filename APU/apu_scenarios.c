#define _CRT_SECURE_NO_WARNINGS

#include "apu_scenarios.h"
#include "apu_interface.h"
#include <string.h>
#include <stdio.h>

void init_fin_scenario(Fin_scenario* fis)
{
    for (int i = 0; i < N_SCENARIOS; i++)
        sprintf(&(fis->path[i]), "scenarios/%d.sc", i);
    fis->pos = 0;
}

void init_fout_scenario(Fout_scenario* fos)
{
    fos->first_record = 1;
    strcpy(fos->path, "output/_scenario.sc");
}

void write_scenario(
    Fout_scenario* fos, 
    Physical* phys, 
    Actions_manual* actm, 
    bool test,
    bool c0_fault, 
    bool c1_fault, 
    char fault)
{
    FILE* fout = fopen(fos->path, (fos->first_record ? "w" : "a"));
    fprintf(
        fout, 
        "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%c\n",
        phys->fire,
        phys->height,
        phys->auto_start,
        phys->ignited,
        phys->enough_pressure,
        actm->power,
        test,
        actm->reset,
        actm->start,
        actm->bleed,
        actm->gen,
        actm->mpu_start,
        c0_fault,
        c1_fault,
        fault);
    fclose(fout);
    fos->first_record = 0;
}

void start_scenario(
    Fin_scenario* scns,
    APU* apu, 
    Actions_manual* actm, 
    Physical* phys,
    ECU* c0, 
    ECU* c1)
{
    FILE* reader = fopen(scns->path[scns->n], "r");
    if (scns->n == 0)
    {
        // TO DO
    }
    else
    {
        int fire;
        int height;
        int auto_start;
        int ignited;
        int enough_pressure;
        int power;
        int test;
        int reset;
        int start;
        int bleed;
        int gen;
        int mpu_start;
        int c0_fault;
        int c1_fault;
        char faults = '0';
        // Читаем строку из файла
        int scanned = 0;
        fseek(reader, scns->pos, SEEK_SET);
        scanned = fscanf(
            reader,
            "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%c\n",
            &(fire),
            &(height),
            &(auto_start),
            &(ignited),
            &(enough_pressure),
            &(power),
            &(test),
            &(reset),
            &(start),
            &(bleed),
            &(gen),
            &(mpu_start),
            &(c0_fault),
            &(c1_fault),
            &faults);
        
        // Сохраняем позицию
        scns->pos = ftell(reader);
        fclose(reader);

        // Если дочитали до конца файла
        if (scanned == EOF)
            return;

        // Записываем считанные данные в bool
        phys->fire = fire;
        phys->height = height;
        phys->auto_start = auto_start;
        phys->ignited = ignited;
        phys->enough_pressure = enough_pressure;
        actm->power = power;
        actm->test = test;
        actm->reset = reset;
        actm->start = start;
        actm->bleed = bleed;
        actm->gen = gen;
        actm->mpu_start = mpu_start;
        c0->fault = c0_fault;
        c1->fault = c1_fault;

        if (faults == 'a')
            apu->ggen.flame_sensor.fault = !apu->ggen.flame_sensor.fault;
        else if (faults == 'b')
            apu->rotor.N1_0.fault = !apu->rotor.N1_0.fault;
        else if (faults == 'c')
            apu->rotor.N1_1.fault = !apu->rotor.N1_1.fault;
        else if (faults == 'd')
            apu->rotor.EGT_A0.fault = !apu->rotor.EGT_A0.fault;
        else if (faults == 'e')
            apu->rotor.EGT_A1.fault = !apu->rotor.EGT_A1.fault;
        else if (faults == 'f')
            apu->rotor.EGT_B0.fault = !apu->rotor.EGT_B0.fault;
        else if (faults == 'g')
            apu->rotor.EGT_B1.fault = !apu->rotor.EGT_B1.fault;
        else if (faults == 'h')
            apu->comp.P3.fault = !apu->comp.P3.fault;
        else if (faults == 'i')
            apu->comp.T3.fault = !apu->comp.T3.fault;
        else if (faults == 'j')
            apu->fan.fault = !apu->fan.fault;
        else if (faults == 'k')
            apu->gen.NGC.fault = !apu->gen.NGC.fault;
        else if (faults == 'l')
            apu->pump.fault = !apu->pump.fault;
        else if (faults == 'm')
            apu->pump.fuel_sov.fault = !apu->pump.fuel_sov.fault;
        else if (faults == 'n')
            apu->pump.fuel_sov.sensor.fault = !apu->pump.fuel_sov.sensor.fault;
        else if (faults == 'o')
            apu->psys.asv.fault = !apu->psys.asv.fault;
        else if (faults == 'p')
            apu->psys.asv.sensor.fault = !apu->psys.asv.sensor.fault;
        else if (faults == 'q')
            apu->psys.bsv.fault = !apu->psys.bsv.fault;
        else if (faults == 'r')
            apu->psys.bsv.sensor.fault = !apu->psys.bsv.sensor.fault;
        else if (faults == 's')
            apu->psys.fcv.fault = !apu->psys.fcv.fault;
        else if (faults == 't')
            apu->psys.fcv.sensor.fault = !apu->psys.fcv.sensor.fault;
        else if (faults == 'u')
            apu->psys.mpu_xbleed.fault = !apu->psys.mpu_xbleed.fault;
        else if (faults == 'v')
            apu->psys.mpu_xbleed.sensor.fault = !apu->psys.mpu_xbleed.sensor.fault;
        else if (faults == 'w')
            apu->psys.P2.fault = !apu->psys.P2.fault;
        else if (faults == 'x')
            apu->psys.T2.fault = !apu->psys.T2.fault;
        else if (faults == 'y')
            apu->psys.P_duct.fault = !apu->psys.P_duct.fault;
        else if (faults == 'z')
            apu->psys.T_duct.fault = !apu->psys.T_duct.fault;
    }
}

