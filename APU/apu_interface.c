#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "apu_interface.h"
#include "apu_scenarios.h"
#include "apu_math.h"

void init_file_output(File_output* fout)
{
    fout->first_output = 1;
    strcpy(fout->starter_M, "output/starter_M.data");
    strcpy(fout->pid_fuel_feed, "output/pid_fuel_feed.data");
    strcpy(fout->pid_p_component, "output/pid_p_component.data");
    strcpy(fout->pid_i_component, "output/pid_i_component.data");
    strcpy(fout->pid_d_component, "output/pid_d_component.data");
    strcpy(fout->ggen_fuel_cmd, "output/ggen_fuel_cmd.data");
    strcpy(fout->ggen_fuel, "output/ggen_fuel.data");
    strcpy(fout->ggen_M, "output/ggen_M.data");
    strcpy(fout->rotor_N, "output/rotor_N.data");
    strcpy(fout->rotor_EGT, "output/rotor_EGT.data");
    strcpy(fout->comp_M, "output/comp_M.data");
    strcpy(fout->comp_P3, "output/comp_P3.data");
    strcpy(fout->comp_T3, "output/comp_T3.data");
    strcpy(fout->fan_M, "output/fan_M.data");
    strcpy(fout->gen_M, "output/gen_M.data");
    strcpy(fout->pump_M, "output/pump_M.data");
    strcpy(fout->psys_P2, "output/psys_P2.data");
    strcpy(fout->psys_T2, "output/psys_T2.data");
    strcpy(fout->psys_P_duct, "output/psys_P_duct.data");
    strcpy(fout->psys_T_duct, "output/psys_T_duct.data");

    strcpy(fout->temp, "output/temp.data");

    fout->first_log = 1;
    strcpy(fout->log, "output/_ECU.log");
}

void write_files(File_output* fout, APU* apu)
{
    // starter_M
    FILE* starter_M = fopen(fout->starter_M, (fout->first_output ? "w" : "a"));
    fprintf(starter_M, "%f\n", apu->start.M);
    fclose(starter_M);

    // pid_fuel_feed
    FILE* pid_fuel_feed = fopen(fout->pid_fuel_feed, (fout->first_output ? "w" : "a"));
    fprintf(pid_fuel_feed, "%f\n", apu->pid.fuel_feed);
    fclose(pid_fuel_feed);

    // pid_p_component
    FILE* pid_p_component = fopen(fout->pid_p_component, (fout->first_output ? "w" : "a"));
    fprintf(pid_p_component, "%f\n", apu->pid.p);
    fclose(pid_p_component);

    // pid_i_component
    FILE* pid_i_component = fopen(fout->pid_i_component, (fout->first_output ? "w" : "a"));
    fprintf(pid_i_component, "%f\n", apu->pid.i);
    fclose(pid_i_component);

    // pid_d_component
    FILE* pid_d_component = fopen(fout->pid_d_component, (fout->first_output ? "w" : "a"));
    fprintf(pid_d_component, "%f\n", apu->pid.d);
    fclose(pid_d_component);

    // ggen_fuel_cmd
    FILE* ggen_fuel_cmd = fopen(fout->ggen_fuel_cmd, (fout->first_output ? "w" : "a"));
    fprintf(ggen_fuel_cmd, "%f\n", apu->ggen.fuel_cmd);
    fclose(ggen_fuel_cmd);

    // ggen_fuel
    FILE* ggen_fuel = fopen(fout->ggen_fuel, (fout->first_output ? "w" : "a"));
    fprintf(ggen_fuel, "%f\n", apu->ggen.fuel);
    fclose(ggen_fuel);

    // ggen_M
    FILE* ggen_M = fopen(fout->ggen_M, (fout->first_output ? "w" : "a"));
    fprintf(ggen_M, "%f\n", apu->ggen.M);
    fclose(ggen_M);

    // rotor_N
    FILE* rotor_N = fopen(fout->rotor_N, (fout->first_output ? "w" : "a"));
    fprintf(rotor_N, "%f\n", apu->rotor.N);
    fclose(rotor_N);

    // rotor_EGT
    FILE* rotor_EGT = fopen(fout->rotor_EGT, (fout->first_output ? "w" : "a"));
    fprintf(rotor_EGT, "%f\n", apu->rotor.EGT);
    fclose(rotor_EGT);

    // comp_M
    FILE* comp_M = fopen(fout->comp_M, (fout->first_output ? "w" : "a"));
    fprintf(comp_M, "%f\n", apu->comp.M);
    fclose(comp_M);

    // comp_P3
    FILE* comp_P3 = fopen(fout->comp_P3, (fout->first_output ? "w" : "a"));
    fprintf(comp_P3, "%f\n", apu->comp.P);
    fclose(comp_P3);

    // comp_T3
    FILE* comp_T3 = fopen(fout->comp_T3, (fout->first_output ? "w" : "a"));
    fprintf(comp_T3, "%f\n", apu->comp.T);
    fclose(comp_T3);

    // fan_M
    FILE* fan_M = fopen(fout->fan_M, (fout->first_output ? "w" : "a"));
    fprintf(fan_M, "%f\n", apu->fan.M);
    fclose(fan_M);

    // gen_M
    FILE* gen_M = fopen(fout->gen_M, (fout->first_output ? "w" : "a"));
    fprintf(gen_M, "%f\n", apu->gen.M);
    fclose(gen_M);

    // pump_M
    FILE* pump_M = fopen(fout->pump_M, (fout->first_output ? "w" : "a"));
    fprintf(pump_M, "%f\n", apu->pump.M);
    fclose(pump_M);

    // psys_P2
    FILE* psys_P2 = fopen(fout->psys_P2, (fout->first_output ? "w" : "a"));
    fprintf(psys_P2, "%f\n", apu->psys.P2.value);
    fclose(psys_P2);

    // psys_T2
    FILE* psys_T2 = fopen(fout->psys_T2, (fout->first_output ? "w" : "a"));
    fprintf(psys_T2, "%f\n", apu->psys.T2.value);
    fclose(psys_T2);

    // psys_P_duct
    FILE* psys_P_duct = fopen(fout->psys_P_duct, (fout->first_output ? "w" : "a"));
    fprintf(psys_P_duct, "%f\n", apu->psys.P_duct.value);
    fclose(psys_P_duct);

    // psys_T_duct
    FILE* psys_T_duct = fopen(fout->psys_T_duct, (fout->first_output ? "w" : "a"));
    fprintf(psys_T_duct, "%f\n", apu->psys.T_duct.value);
    fclose(psys_T_duct);

    // temp
    /*FILE* temp = fopen(fout->temp, (fout->first_output ? "w" : "a"));
    fprintf(temp, "%f\n", apu->ggen.flame_sensor.value + apu->ggen.ignition);
    fclose(temp);*/

    fout->first_output = 0;
}

void init_output(Output* out)
{
    out->setup_done = 0;
    out->detailed_output = OFF;
    out->normal_height = 36;
    out->normal_width = 100;
    out->detailed_height = 60;
    out->detailed_width = 140;
    out->normal_message_begin = out->normal_height - N_MESSAGES;
    out->detailed_message_begin = out->detailed_height - N_MESSAGES;
}

void init_buffer(Message_buffer* mb)
{
    for (int i = 0; i < N_MESSAGES; i++)
        *mb->buffer[i] = "\0";
    mb->filled = 0;
    mb->cur_begin = 0;
    mb->updated = 0;
}


void print_to_buffer(Message_buffer* mb, char* str[STRING_LEN], File_output* fout)
{
    strcpy(mb->buffer[(mb->cur_begin + mb->filled) % N_MESSAGES], str);
    if (mb->filled == 10)
        mb->cur_begin = (mb->cur_begin + 1) % N_MESSAGES;
    mb->filled = MIN(mb->filled + 1, 10);
    mb->updated = 1;

    FILE* log = fopen(fout->log, (fout->first_log ? "w" : "a"));
    fprintf(log, "%s", str);
    fclose(log);
    fout->first_log = 0;
}

void move_to(int x, int y)
{
    char temp[STRING_LEN];
    sprintf(&temp, "\033[%d;%dH", y + 1, x + 1);
    printf("%s", temp);
}

void printer(
    bool manual_mode,
    Output* out,
    Message_buffer* mb,
    Responses* rsps,
    Responses* prev_rsps,
    uint32_t cur_time,
    int height,
    int* prev_height,
    State state,
    State* prev_state,
    bool ECU0_fault,
    bool* ECU0_fault_prev,
    bool ECU1_fault,
    bool* ECU1_fault_prev,
    APU* apu,
    APU* prev_apu)
{
    const char* normal_layout[] = NORMAL_LAYOUT;
    const char* detailed_layout[] = DETAILED_LAYOUT;
    const char* states[] = APU_STATES;
    
    char temp[STRING_LEN];

    if (!out->setup_done)
    {
        system("cls");
        // Выводим подложку
        if (!out->detailed_output)
        {
            for (int i = 0; i < N_NORMAL_LAYOUT; i++)
                printf("%s", normal_layout[i]);
        }
        else
        {
            for (int i = 0; i < N_DETAILED_LAYOUT; i++)
                printf("%s", detailed_layout[i]);
        }
        // Выводим команды
        if (manual_mode)
        {
            printf("  - Press S - stop/start simulation\n");
            printf("  - Press D - toggle detailed output mode\n");
            printf("    Press p - power ON/OFF\n");
            printf("    Press c - change physical parameters (e.g. height)\n");
            printf("    Press t - conduct testing\n");
            printf("    Press r - reset fault parameters\n");
            printf("    Press s - start/stop APU\n");
            printf("    Press b - toggle air bleed\n");
            printf("    Press g - toggle generator\n");
            printf("    Press m - start/stop MPU\n");
        }
        else
        {
            printf("  - Press S - stop/start simulation\n");
            printf("  - Press D - toggle detailed output mode\n");
        }
        printf("%s\n", out->detailed_output? detailed_layout[0] : normal_layout[0]);

        fflush(stdout);
    }

    // Обновляем значения на панели
    // Состояние
    if (state != *prev_state || !out->setup_done)
    {
        move_to(POS_X_GENERAL_COL, POS_Y_STATE);
        printf("%s                    ", states[state]);
    }
    // Высота
    if (*prev_height != height || !out->setup_done)
    {
        move_to(POS_X_GENERAL_COL, POS_Y_HEIGHT);
        printf(HEIGHT_FORMAT, height);
    }
    // Время
    move_to(POS_X_GENERAL_COL, POS_Y_TIME);
    printf(TIME_FORMAT, TIME(cur_time), cur_time);
    // Питание
    if (prev_rsps->rcs.power_on != rsps->rcs.power_on || !out->setup_done)
    {
        move_to(POS_X_COMMON_COL1, POS_Y_POWER);
        printf(BRACKET_FORMAT, BRACKET_SMB(rsps->rcs.power_on));
    }
    // Индикация запуска
    if (prev_rsps->rcs.start != rsps->rcs.start || !out->setup_done)
    {
        move_to(POS_X_COMMON_COL1, POS_Y_START);
        printf(BRACKET_FORMAT, BRACKET_SMB(rsps->rcs.start));
    }
    // Доступность ВСУ
    if (prev_rsps->rcs.avail != rsps->rcs.avail || !out->setup_done)
    {
        move_to(POS_X_COMMON_COL1, POS_Y_AVAIL);
        printf(BRACKET_FORMAT, BRACKET_SMB(rsps->rcs.avail));
    }
    // Индикация остановки
    if (prev_rsps->rcs.stop != rsps->rcs.stop || !out->setup_done)
    {
        move_to(POS_X_COMMON_COL1, POS_Y_STOP);
        printf(BRACKET_FORMAT, BRACKET_SMB(rsps->rcs.stop));
    }
    // N1
    if (!DOUBLE_EQUALS(prev_rsps->rcs.N1, rsps->rcs.N1, COMP_CONST) || !out->setup_done)
    {
        move_to(POS_X_COMMON_COL1, POS_Y_N1);
        printf(N_FORMAT, rsps->rcs.N1);
    }
    // EGT
    if (!DOUBLE_EQUALS(prev_rsps->rcs.EGT, rsps->rcs.EGT, COMP_CONST) || !out->setup_done)
    {
        move_to(POS_X_COMMON_COL1, POS_Y_EGT);
        printf(T_FORMAT, rsps->rcs.EGT);
    }
    // Индикация неисправности
    if (prev_rsps->rcs.apu_fault != rsps->rcs.apu_fault || !out->setup_done)
    {
        move_to(POS_X_COMMON_COL2, POS_Y_FAULT);
        printf(BRACKET_FORMAT, BRACKET_SMB(rsps->rcs.apu_fault));
    }
    // Индикация критической неисправности
    if (prev_rsps->rcs.critical_fault != rsps->rcs.critical_fault || !out->setup_done)
    {
        move_to(POS_X_COMMON_COL2, POS_Y_CRIT_FAULT);
        printf(BRACKET_FORMAT, BRACKET_SMB(rsps->rcs.critical_fault));
    }
    // Индикация автовыключения
    if (prev_rsps->rcs.auto_shutdown != rsps->rcs.auto_shutdown || !out->setup_done)
    {
        move_to(POS_X_COMMON_COL2, POS_Y_AUTO_SHTDN);
        printf(BRACKET_FORMAT, BRACKET_SMB(rsps->rcs.auto_shutdown));
    }
    // Детальный вывод
    if (out->detailed_output)
    {
        // Область структуры Responses
        if (prev_rsps->eps.power_avail != rsps->eps.power_avail)
        {
            move_to(POS_X_RSPS_COL1, POS_Y_POWER_AVAIL);
            printf(BRACKET_FORMAT, BRACKET_SMB(rsps->eps.power_avail));
        }
        if (prev_rsps->fs.fuel_demand != rsps->fs.fuel_demand)
        {
            move_to(POS_X_RSPS_COL1, POS_Y_FUEL_DEMAND);
            printf(BRACKET_FORMAT, BRACKET_SMB(rsps->fs.fuel_demand));
        }
        if (prev_rsps->air_cs.apu_bleed_avail != rsps->air_cs.apu_bleed_avail)
        {
            move_to(POS_X_RSPS_COL1, POS_Y_ACS_BLEED_AVAIL);
            printf(BRACKET_FORMAT, BRACKET_SMB(rsps->air_cs.apu_bleed_avail));
        }
        if (prev_rsps->mpu.apu_bleed_avail != rsps->mpu.apu_bleed_avail)
        {
            move_to(POS_X_RSPS_COL2, POS_Y_MPU_BLEED_AVAIL);
            printf(BRACKET_FORMAT, BRACKET_SMB(rsps->mpu.apu_bleed_avail));
        }
        if (prev_rsps->mpu.apu_bleed_valve_stat != rsps->mpu.apu_bleed_valve_stat)
        {
            move_to(POS_X_RSPS_COL2, POS_Y_BLEED_VLV_STAT);
            printf(BRACKET_FORMAT, BRACKET_SMB(rsps->mpu.apu_bleed_valve_stat));
        }
        if (prev_rsps->mpu.crossbleed_valve_stat != rsps->mpu.crossbleed_valve_stat)
        {
            move_to(POS_X_RSPS_COL2, POS_Y_XBLEED_VLV_STAT);
            printf(BRACKET_FORMAT, BRACKET_SMB(rsps->mpu.crossbleed_valve_stat));
        }
        if (prev_rsps->mpu.eng_bleed_valve_stat != rsps->mpu.eng_bleed_valve_stat)
        {
            move_to(POS_X_RSPS_COL2, POS_Y_ENG_BLEED_VLV_STAT);
            printf(BRACKET_FORMAT, BRACKET_SMB(rsps->mpu.eng_bleed_valve_stat));
        }
        if (prev_rsps->mpu.apu_bleed_pres != rsps->mpu.apu_bleed_pres)
        {
            move_to(POS_X_RSPS_COL2, POS_Y_ENOUGH_BLEED_PRES);
            printf(BRACKET_FORMAT, BRACKET_SMB(rsps->mpu.apu_bleed_pres));
        }

        // Область физики, колонка 1 (стартер, ПИД-регулятор, газогенератор)
        if (prev_apu->start.power != apu->start.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1, POS_Y_START_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->start.power));
        }
        if (prev_apu->start.M != apu->start.M || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1, POS_Y_START_M);
            printf(M_FORMAT, apu->start.M);
        }
        if (prev_apu->pid.fuel_feed != apu->pid.fuel_feed || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1, POS_Y_PID_FUEL_FEED);
            printf(FUEL_FEED_FORMAT, apu->pid.fuel_feed);
        }
        if (prev_apu->pid.p != apu->pid.p || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1 - 3, POS_Y_PID_P_COMP);
            printf(EXP_FORMAT, apu->pid.p);
        }
        if (prev_apu->pid.i != apu->pid.i || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1 - 3, POS_Y_PID_I_COMP);
            printf(EXP_FORMAT, apu->pid.i);
        }
        if (prev_apu->pid.d != apu->pid.d || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1 - 3, POS_Y_PID_D_COMP);
            printf(EXP_FORMAT, apu->pid.d);
        }
        if (prev_apu->ggen.ignited != apu->ggen.ignited || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1, POS_Y_GGEN_IGNITED);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->ggen.ignited));
        }
        if (prev_apu->ggen.fuel_cmd != apu->ggen.fuel_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1, POS_Y_GGEN_FUEL_CMD);
            printf(FUEL_FORMAT, apu->ggen.fuel_cmd);
        }
        if (prev_apu->ggen.fuel != apu->ggen.fuel || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1, POS_Y_GGEN_FUEL);
            printf(FUEL_FORMAT, apu->ggen.fuel);
        }
        if (prev_apu->ggen.M != apu->ggen.M || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1, POS_Y_GGEN_M);
            printf(M_FORMAT, apu->ggen.M);
        }
        if (prev_apu->ggen.flame_sensor.power != apu->ggen.flame_sensor.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1, POS_Y_GGEN_FLAME_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->ggen.flame_sensor.power));
        }
        if (prev_apu->ggen.flame_sensor.fault != apu->ggen.flame_sensor.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1, POS_Y_GGEN_FLAME_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->ggen.flame_sensor.fault));
        }
        if (prev_apu->ggen.flame_sensor.value != apu->ggen.flame_sensor.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1, POS_Y_GGEN_FLAME_VALUE);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->ggen.flame_sensor.value));
        }
        // Область каналов контроллера
        if (ECU0_fault_prev != ECU0_fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1, POS_Y_ECU0);
            printf(BRACKET_FORMAT, BRACKET_SMB(ECU0_fault));
        }
        if (ECU1_fault_prev != ECU1_fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL1, POS_Y_ECU1);
            printf(BRACKET_FORMAT, BRACKET_SMB(ECU1_fault));
        }

        // Область физики, колонка 2 (ротор)
        if (prev_apu->rotor.N_target != apu->rotor.N_target || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_NTARGET);
            printf(N_FORMAT, apu->rotor.N_target);
        }
        if (prev_apu->rotor.N != apu->rotor.N || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_N);
            printf(N_FORMAT, apu->rotor.N);
        }
        if (prev_apu->rotor.EGT != apu->rotor.EGT || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGT);
            printf(T_FORMAT, apu->rotor.EGT);
        }
        if (prev_apu->rotor.N1_0.power != apu->rotor.N1_0.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_N10_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->rotor.N1_0.power));
        }
        if (prev_apu->rotor.N1_0.fault != apu->rotor.N1_0.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_N10_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->rotor.N1_0.fault));
        }
        if (prev_apu->rotor.N1_0.value != apu->rotor.N1_0.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_N10_VALUE);
            printf(N_FORMAT, apu->rotor.N1_0.value);
        }
        if (prev_apu->rotor.N1_1.power != apu->rotor.N1_1.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_N11_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->rotor.N1_1.power));
        }
        if (prev_apu->rotor.N1_1.fault != apu->rotor.N1_1.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_N11_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->rotor.N1_1.fault));
        }
        if (prev_apu->rotor.N1_1.value != apu->rotor.N1_1.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_N11_VALUE);
            printf(N_FORMAT, apu->rotor.N1_1.value);
        }
        if (prev_apu->rotor.EGT_A0.power != apu->rotor.EGT_A0.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGTA0_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->rotor.EGT_A0.power));
        }
        if (prev_apu->rotor.EGT_A0.fault != apu->rotor.EGT_A0.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGTA0_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->rotor.EGT_A0.fault));
        }
        if (prev_apu->rotor.EGT_A0.value != apu->rotor.EGT_A0.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGTA0_VALUE);
            printf(T_FORMAT, apu->rotor.EGT_A0.value);
        }
        if (prev_apu->rotor.EGT_A1.power != apu->rotor.EGT_A1.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGTA1_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->rotor.EGT_A1.power));
        }
        if (prev_apu->rotor.EGT_A1.fault != apu->rotor.EGT_A1.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGTA1_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->rotor.EGT_A1.fault));
        }
        if (prev_apu->rotor.EGT_A1.value != apu->rotor.EGT_A1.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGTA1_VALUE);
            printf(T_FORMAT, apu->rotor.EGT_A1.value);
        }
        if (prev_apu->rotor.EGT_B0.power != apu->rotor.EGT_B0.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGTB0_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->rotor.EGT_B0.power));
        }
        if (prev_apu->rotor.EGT_B0.fault != apu->rotor.EGT_B0.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGTB0_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->rotor.EGT_B0.fault));
        }
        if (prev_apu->rotor.EGT_B0.value != apu->rotor.EGT_B0.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGTB0_VALUE);
            printf(T_FORMAT, apu->rotor.EGT_B0.value);
        }
        if (prev_apu->rotor.EGT_B1.power != apu->rotor.EGT_B1.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGTB1_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->rotor.EGT_B1.power));
        }
        if (prev_apu->rotor.EGT_B1.fault != apu->rotor.EGT_B1.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGTB1_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->rotor.EGT_B1.fault));
        }
        if (prev_apu->rotor.EGT_B1.value != apu->rotor.EGT_B1.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL2, POS_Y_ROTOR_EGTB1_VALUE);
            printf(T_FORMAT, apu->rotor.EGT_B1.value);
        }

        // Область физики, колонка 3 (компрессор, вентилятор)
        if (prev_apu->comp.bleed != apu->comp.bleed || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_COMP_BLEED);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->comp.bleed));
        }
        if (prev_apu->comp.M != apu->comp.M || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_COMP_M);
            printf(M_FORMAT, apu->comp.M);
        }
        if (prev_apu->comp.P != apu->comp.P || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_COMP_P3);
            printf(P_FORMAT, apu->comp.P);
        }
        if (prev_apu->comp.T != apu->comp.T || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_COMP_T3);
            printf(T_FORMAT, apu->comp.T);
        }
        if (prev_apu->comp.P3.power != apu->comp.P3.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_COMP_P3_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->comp.P3.power));
        }
        if (prev_apu->comp.P3.fault != apu->comp.P3.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_COMP_P3_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->comp.P3.fault));
        }
        if (prev_apu->comp.P3.value != apu->comp.P3.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_COMP_P3_VALUE);
            printf(P_FORMAT, apu->comp.P3.value);
        }
        if (prev_apu->comp.T3.power != apu->comp.T3.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_COMP_T3_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->comp.T3.power));
        }
        if (prev_apu->comp.T3.fault != apu->comp.T3.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_COMP_T3_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->comp.T3.fault));
        }
        if (prev_apu->comp.T3.value != apu->comp.T3.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_COMP_T3_VALUE);
            printf(T_FORMAT, apu->comp.T3.value);
        }
        if (prev_apu->fan.fault != apu->fan.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_FAN_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->fan.fault));
        }
        if (prev_apu->fan.N != apu->fan.N || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_FAN_N);
            printf(N_FORMAT, apu->fan.N);
        }
        if (prev_apu->fan.M != apu->fan.M || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL3, POS_Y_FAN_M);
            printf(N_FORMAT, apu->fan.M);
        }
        
        // Область физики, колонка 4 (генератор, насос)
        if (prev_apu->gen.on != apu->gen.on || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_GEN_ON);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->gen.on));
        }
        if (prev_apu->gen.M != apu->gen.M || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_GEN_M);
            printf(M_FORMAT, apu->gen.M);
        }
        if (prev_apu->gen.NGC.power != apu->gen.NGC.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_GEN_NGC_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->gen.NGC.power));
        }
        if (prev_apu->gen.NGC.fault != apu->gen.NGC.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_GEN_NGC_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->gen.NGC.fault));
        }
        if (prev_apu->gen.NGC.value != apu->gen.NGC.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_GEN_NGC_VALUE);
            printf(N_FORMAT, apu->gen.NGC.value);
        }
        if (prev_apu->pump.power != apu->pump.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->pump.power));
        }
        if (prev_apu->pump.turn_on_cmd != apu->pump.turn_on_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_TURN_ON);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->pump.turn_on_cmd));
        }
        if (prev_apu->pump.turn_off_cmd != apu->pump.turn_off_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_TURN_OFF);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->pump.turn_off_cmd));
        }
        if (prev_apu->pump.on != apu->pump.on || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_ON);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->pump.on));
        }
        if (prev_apu->pump.M != apu->pump.M || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_M);
            printf(M_FORMAT, apu->pump.M);
        }
        if (prev_apu->pump.fuel_sov.power != apu->pump.fuel_sov.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_FUEL_SOV_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->pump.fuel_sov.power));
        }
        if (prev_apu->pump.fuel_sov.fault != apu->pump.fuel_sov.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_FUEL_SOV_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->pump.fuel_sov.fault));
        }
        if (prev_apu->pump.fuel_sov.open_cmd != apu->pump.fuel_sov.open_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_FUEL_SOV_OPEN_CMD);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->pump.fuel_sov.open_cmd));
        }
        if (prev_apu->pump.fuel_sov.close_cmd != apu->pump.fuel_sov.close_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_FUEL_SOV_CLOSE_CMD);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->pump.fuel_sov.close_cmd));
        }
        if (prev_apu->pump.fuel_sov.open != apu->pump.fuel_sov.open || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_FUEL_SOV_OPEN);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->pump.fuel_sov.open));
        }
        if (prev_apu->pump.fuel_sov.sensor.power != apu->pump.fuel_sov.sensor.power || 
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_FUEL_SOV_SENSOR_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->pump.fuel_sov.sensor.power));
        }
        if (prev_apu->pump.fuel_sov.sensor.fault != apu->pump.fuel_sov.sensor.fault ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_FUEL_SOV_SENSOR_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->pump.fuel_sov.sensor.fault));
        }
        if (prev_apu->pump.fuel_sov.sensor.value != apu->pump.fuel_sov.sensor.value ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL4, POS_Y_PUMP_FUEL_SOV_SENSOR_VALUE);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->pump.fuel_sov.sensor.value));
        }

        // Область физики, колонка 5 (пневмосистема)
        if (prev_apu->psys.asv.power != apu->psys.asv.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_ASV_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.asv.power));
        }
        if (prev_apu->psys.asv.fault != apu->psys.asv.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_ASV_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.asv.fault));
        }
        if (prev_apu->psys.asv.open_cmd != apu->psys.asv.open_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_ASV_OPEN_CMD);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.asv.open_cmd));
        }
        if (prev_apu->psys.asv.close_cmd != apu->psys.asv.close_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_ASV_CLOSE_CMD);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.asv.close_cmd));
        }
        if (prev_apu->psys.asv.open != apu->psys.asv.open || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_ASV_OPEN);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.asv.open));
        }
        if (prev_apu->psys.asv.sensor.power != apu->psys.asv.sensor.power ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_ASV_SENSOR_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.asv.sensor.power));
        }
        if (prev_apu->psys.asv.sensor.fault != apu->psys.asv.sensor.fault ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_ASV_SENSOR_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.asv.sensor.fault));
        }
        if (prev_apu->psys.asv.sensor.value != apu->psys.asv.sensor.value ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_ASV_SENSOR_VALUE);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.asv.sensor.value));
        }
        if (prev_apu->psys.bsv.power != apu->psys.bsv.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_BSV_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.bsv.power));
        }
        if (prev_apu->psys.bsv.fault != apu->psys.bsv.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_BSV_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.bsv.fault));
        }
        if (prev_apu->psys.bsv.open_cmd != apu->psys.bsv.open_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_BSV_OPEN_CMD);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.bsv.open_cmd));
        }
        if (prev_apu->psys.bsv.close_cmd != apu->psys.bsv.close_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_BSV_CLOSE_CMD);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.bsv.close_cmd));
        }
        if (prev_apu->psys.bsv.open != apu->psys.bsv.open || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_BSV_OPEN);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.bsv.open));
        }
        if (prev_apu->psys.bsv.sensor.power != apu->psys.bsv.sensor.power ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_BSV_SENSOR_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.bsv.sensor.power));
        }
        if (prev_apu->psys.bsv.sensor.fault != apu->psys.bsv.sensor.fault ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_BSV_SENSOR_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.bsv.sensor.fault));
        }
        if (prev_apu->psys.bsv.sensor.value != apu->psys.bsv.sensor.value ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_BSV_SENSOR_VALUE);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.bsv.sensor.value));
        }
        if (prev_apu->psys.P2.power != apu->psys.P2.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_P2_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.P2.power));
        }
        if (prev_apu->psys.P2.fault != apu->psys.P2.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_P2_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.P2.fault));
        }
        if (prev_apu->psys.P2.value != apu->psys.P2.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_P2_VALUE);
            printf(P_FORMAT, apu->psys.P2.value);
        }
        if (prev_apu->psys.T2.power != apu->psys.T2.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_T2_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.T2.power));
        }
        if (prev_apu->psys.T2.fault != apu->psys.T2.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_T2_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.T2.fault));
        }
        if (prev_apu->psys.T2.value != apu->psys.T2.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL5, POS_Y_PSYS_T2_VALUE);
            printf(T_FORMAT, apu->psys.T2.value);
        }
        
        // Область физики, колонка 6 (пневмосистема)
        if (prev_apu->psys.fcv.power != apu->psys.fcv.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_FCV_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.fcv.power));
        }
        if (prev_apu->psys.fcv.fault != apu->psys.fcv.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_FCV_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.fcv.fault));
        }
        if (prev_apu->psys.fcv.open_cmd != apu->psys.fcv.open_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_FCV_OPEN_CMD);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.fcv.open_cmd));
        }
        if (prev_apu->psys.fcv.close_cmd != apu->psys.fcv.close_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_FCV_CLOSE_CMD);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.fcv.close_cmd));
        }
        if (prev_apu->psys.fcv.open != apu->psys.fcv.open || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_FCV_OPEN);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.fcv.open));
        }
        if (prev_apu->psys.fcv.sensor.power != apu->psys.fcv.sensor.power ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_FCV_SENSOR_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.fcv.sensor.power));
        }
        if (prev_apu->psys.fcv.sensor.fault != apu->psys.fcv.sensor.fault ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_FCV_SENSOR_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.fcv.sensor.fault));
        }
        if (prev_apu->psys.fcv.sensor.value != apu->psys.fcv.sensor.value ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_FCV_SENSOR_VALUE);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.fcv.sensor.value));
        }
        if (prev_apu->psys.mpu_xbleed.power != apu->psys.mpu_xbleed.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_XBLEED_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.mpu_xbleed.power));
        }
        if (prev_apu->psys.mpu_xbleed.fault != apu->psys.mpu_xbleed.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_XBLEED_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.mpu_xbleed.fault));
        }
        if (prev_apu->psys.mpu_xbleed.open_cmd != apu->psys.mpu_xbleed.open_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_XBLEED_OPEN_CMD);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.mpu_xbleed.open_cmd));
        }
        if (prev_apu->psys.mpu_xbleed.close_cmd != apu->psys.mpu_xbleed.close_cmd || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_XBLEED_CLOSE_CMD);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.mpu_xbleed.close_cmd));
        }
        if (prev_apu->psys.mpu_xbleed.open != apu->psys.mpu_xbleed.open || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_XBLEED_OPEN);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.mpu_xbleed.open));
        }
        if (prev_apu->psys.mpu_xbleed.sensor.power != apu->psys.mpu_xbleed.sensor.power ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_XBLEED_SENSOR_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.mpu_xbleed.sensor.power));
        }
        if (prev_apu->psys.mpu_xbleed.sensor.fault != apu->psys.mpu_xbleed.sensor.fault ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_XBLEED_SENSOR_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.mpu_xbleed.sensor.fault));
        }
        if (prev_apu->psys.mpu_xbleed.sensor.value != apu->psys.mpu_xbleed.sensor.value ||
            !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_XBLEED_SENSOR_VALUE);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.mpu_xbleed.sensor.value));
        }
        if (prev_apu->psys.P_duct.power != apu->psys.P_duct.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_PDUCT_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.P_duct.power));
        }
        if (prev_apu->psys.P_duct.fault != apu->psys.P_duct.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_PDUCT_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.P_duct.fault));
        }
        if (prev_apu->psys.P_duct.value != apu->psys.P_duct.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_PDUCT_VALUE);
            printf(P_FORMAT, apu->psys.P_duct.value);
        }
        if (prev_apu->psys.T_duct.power != apu->psys.T_duct.power || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_TDUCT_POWER);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.T_duct.power));
        }
        if (prev_apu->psys.T_duct.fault != apu->psys.T_duct.fault || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_TDUCT_FAULT);
            printf(BRACKET_FORMAT, BRACKET_SMB(apu->psys.T_duct.fault));
        }
        if (prev_apu->psys.T_duct.value != apu->psys.T_duct.value || !out->setup_done)
        {
            move_to(POS_X_COMPS_COL6, POS_Y_PSYS_TDUCT_VALUE);
            printf(T_FORMAT, apu->psys.T_duct.value);
        }

    }
    fflush(stdout);

    // Перемещаемся к выводу сообщений
    if (mb->updated || !out->setup_done)
    {
        if (!out->detailed_output)
            move_to(0, out->normal_message_begin - 1 - (manual_mode? 0 : SCENARIO_MODE_SHORTER));
        else
            move_to(0, out->detailed_message_begin - 1 - (manual_mode ? 0 : SCENARIO_MODE_SHORTER));
        // Очистить от курсора до конца экрана
        printf("\033[0J");
        for (int i = 0; i < mb->filled; i++)
            printf("%s", mb->buffer[(mb->cur_begin + i) % N_MESSAGES]);
        mb->updated = 0;
        fflush(stdout);
    }

    out->setup_done = 1;

    *prev_state = state;
    *prev_height = height;
    *ECU0_fault_prev = ECU0_fault;
    *ECU1_fault_prev = ECU1_fault;
    copy_responses(rsps, prev_rsps);
    copy_APU(apu, prev_apu);

}


void scenario_menu()
{
    printf("  Select scenario:\n");
    printf("  - Press D - data translation demo (ARINC/analog signals)\n");
    printf("    Press 1 - normal manual run on ground with test;\n");
    printf("              generator, then MPU\n");
    printf("    Press 2 - bleed shutoff valve jam detected during test,\n");
    printf("              launch impossible\n");
    printf("    Press 3 - manual run on ground, ACS + generator, then \n");
    printf("              cooling fan failure, overheat\n");
    printf("    Press 4 - manual run on ground, supplying MPU, then \n");
    printf("              fuel pump failure \n");
    printf("    Press 5 - manual run on ground, ACS + generator, then\n");
    printf("              main ECU channel failure, then loss of\n");
    printf("              control, watchdog shutdown\n");
    printf("    Press 6 - manual run in the air, supplies generator,\n");
    printf("              then a fire occurs\n");
    printf("    Press 7 - auto start in the air to restart MPU, success\n");
    printf("    Press 8 - emergency start in the air to restart MPU,\n");
    printf("              failure to restart MPU; auto start on lower \n");
    printf("              height, MPU restart successful\n");
    printf("============================================================\n");
    printf("  - Press M - start in manual mode instead\n");
    printf("============================================================\n");
}

int get_scenario()
{
    scenario_menu();
    char key = _getch();
    while (!(key >= '1' && key <= '8' || key == 'D' || key == 'M'))
        key = _getch();
    if (key == 'M')
        return  -1;
    else if (key == 'D')
    {
        printf("  You selected SCENARIO D\n");
        Sleep(500);
        return 0;
    }
    else
    {
        printf("  You selected SCENARIO %c\n", key);
        Sleep(500);
        return key - '0';
    }
}


void phys_params_menu(Physical* phys, bool c0_fault, bool c1_fault)
{
    printf("============================================================\n");
    printf("  You are changing physical parameters; simulation is paused\n");
    printf("  Change parameters: select parameter to toggle or enter\n");
    printf("  its value\n");
    printf("  - Press Q - quit menu\n");
    printf("  - Press A - auto start\n");
    printf("    Press f - toggle fire                                [%c]\n",
        BRACKET_SMB(phys->fire));
    printf("    Press h - height                                   %5d\n",
        phys->height);
    printf("    Press i - ignition                                   [%c]\n",
        BRACKET_SMB(phys->ignited));
    printf("    Press p - bleed pressure sufficient for MPU          [%c]\n",
        BRACKET_SMB(phys->enough_pressure));
    printf("    Press E - toggle ECU channel 0 failure               [%c]\n",
        BRACKET_SMB(c0_fault));
    printf("    Press e - toggle ECU channel 1 failure               [%c]\n",
        BRACKET_SMB(c1_fault));
    printf("    Press c - components fault                           ...\n");
    printf("============================================================\n");
}

void toggle_fault_menu(APU* apu)
{
    printf("============================================================\n");
    printf("  You are toggling fault parameters; simulation is paused\n");
    printf("  Change parameters: select parameter to toggle\n");
    printf("  - Press Q - quit menu\n");
    printf("    Press a - (gas generator) flame sensor fault         [%c]\n",
        BRACKET_SMB(apu->ggen.flame_sensor.fault));
    printf("    Press b - (rotor) N1-0 sensor fault                  [%c]\n",
        BRACKET_SMB(apu->rotor.N1_0.fault));
    printf("    Press c - (rotor) N1-1 sensor fault                  [%c]\n",
        BRACKET_SMB(apu->rotor.N1_1.fault));
    printf("    Press d - (rotor) EGT-A0 sensor fault                [%c]\n",
        BRACKET_SMB(apu->rotor.EGT_A0.fault));
    printf("    Press e - (rotor) EGT-A1 sensor fault                [%c]\n",
        BRACKET_SMB(apu->rotor.EGT_A1.fault));
    printf("    Press f - (rotor) EGT-B0 sensor fault                [%c]\n",
        BRACKET_SMB(apu->rotor.EGT_B0.fault));
    printf("    Press g - (rotor) EGT-B1 sensor fault                [%c]\n",
        BRACKET_SMB(apu->rotor.EGT_B1.fault));
    printf("    Press h - (compressor) P3 sensor fault               [%c]\n",
        BRACKET_SMB(apu->comp.P3.fault));
    printf("    Press i - (compressor) T3 sensor fault               [%c]\n",
        BRACKET_SMB(apu->comp.T3.fault));
    printf("    Press j - cooling fan fault                          [%c]\n",
        BRACKET_SMB(apu->fan.fault));
    printf("    Press k - (generator) NGC sensor fault               [%c]\n",
        BRACKET_SMB(apu->gen.NGC.fault));
    printf("    Press l - fuel pump fault                            [%c]\n",
        BRACKET_SMB(apu->pump.fault));
    printf("    Press m - (fuel pump) fuel SOV fault                 [%c]\n",
        BRACKET_SMB(apu->pump.fuel_sov.fault));
    printf("    Press n - (fuel pump) fuel SOV sensor fault          [%c]\n",
        BRACKET_SMB(apu->pump.fuel_sov.sensor.fault));
    printf("    Press o - (pneumatic) ASV fault                      [%c]\n",
        BRACKET_SMB(apu->psys.asv.fault));
    printf("    Press p - (pneumatic) ASV sensor fault               [%c]\n",
        BRACKET_SMB(apu->psys.asv.sensor.fault));
    printf("    Press q - (pneumatic) BSV fault                      [%c]\n",
        BRACKET_SMB(apu->psys.bsv.fault));
    printf("    Press r - (pneumatic) BSV sensor fault               [%c]\n",
        BRACKET_SMB(apu->psys.bsv.sensor.fault));
    printf("    Press s - (pneumatic) FCV fault                      [%c]\n",
        BRACKET_SMB(apu->psys.fcv.fault));
    printf("    Press t - (pneumatic) FCV sensor fault               [%c]\n",
        BRACKET_SMB(apu->psys.fcv.sensor.fault));
    printf("    Press u - (pneumatic) MPU Xbleed valve fault         [%c]\n",
        BRACKET_SMB(apu->psys.mpu_xbleed.fault));
    printf("    Press v - (pneumatic) MPU Xbleed valve sensor fault  [%c]\n",
        BRACKET_SMB(apu->psys.mpu_xbleed.sensor.fault));
    printf("    Press w - (pneumatic) P2 sensor fault                [%c]\n",
        BRACKET_SMB(apu->psys.P2.fault));
    printf("    Press x - (pneumatic) T2 sensor fault                [%c]\n",
        BRACKET_SMB(apu->psys.T2.fault));
    printf("    Press y - (pneumatic) P duct sensor fault            [%c]\n",
        BRACKET_SMB(apu->psys.P_duct.fault));
    printf("    Press z - (pneumatic) T duct sensor fault            [%c]\n",
        BRACKET_SMB(apu->psys.T_duct.fault));
    printf("============================================================\n");
}

char handle_key_press(
    char key, 
    bool manual_mode, 
    Output* out, 
    File_output* fout,
    Message_buffer* mb, 
    State state,
    bool power,
    bool* c0_fault,
    bool* c1_fault,
    APU* apu,
    Physical* phys,
    Actions_manual* actm)
{
    const char* apu_states[] = APU_STATES;
    char fault = '0';

    if (key == 'S')
    {
        if (!out->detailed_output)
            move_to(0, out->normal_height - 1);
        else
            move_to(0, out->detailed_height - 1);
        system("pause");
        system("cls");
        out->setup_done = 0;
    }
    else if (key == 'D')
    {
        out->detailed_output = !out->detailed_output;
        out->setup_done = 0;
    }
    else if (manual_mode)
    {
        if (key == 'p')
        {
            if (power && state == STATE_OFF)
            {
                print_to_buffer(mb, " === POWER ON ===\n", fout);
                actm->power = 1;
            }
            else if (!power && state == STATE_IDLE)
            {
                print_to_buffer(mb, " === POWER OFF ===\n", fout);
                actm->power = 0;
            }
            else
            {
                print_to_buffer(mb, " === UNABLE TO POWER ON/OFF FROM CURRENT STATE ===\n", fout);
                char temp_str[STRING_LEN];
                sprintf(&temp_str, " === CURRENT STATE: %s ===\n", apu_states[state]);
                print_to_buffer(mb, temp_str, fout);
            }
        }
        else if (key == 'c')
        {
            system("cls");
            out->setup_done = 0;
            phys_params_menu(phys, *c0_fault, *c1_fault);
            char key2 = _getch();
            if (key2 == 'A')
                phys->auto_start = 1;
            while (key2 != 'Q' && key2 != 'A')
            {
                unsigned int new_height = phys->height;
                if (key2 == 'f')
                    phys->fire = !phys->fire;
                else if (key2 == 'h')
                {
                    printf("  Enter new value for HEIGHT and press ENTER: ");
                    scanf("%d", &new_height);
                    phys->height = new_height;
                }
                else if (key2 == 'i')
                    phys->ignited = !phys->ignited;
                else if (key2 == 'p')
                    phys->enough_pressure = !phys->enough_pressure;
                else if (key2 == 'E')
                    *c0_fault = !(*c0_fault);
                else if (key2 == 'e')
                    *c1_fault = !(*c1_fault);
                else if(key2 == 'c')
                {
                    system("cls");
                    toggle_fault_menu(apu);
                    char key3 = _getch();
                    while(key3 != 'Q')
                    {
                        if (key3 == 'a')
                            apu->ggen.flame_sensor.fault = !apu->ggen.flame_sensor.fault;
                        else if (key3 == 'b')
                            apu->rotor.N1_0.fault = !apu->rotor.N1_0.fault;
                        else if (key3 == 'c')
                            apu->rotor.N1_1.fault = !apu->rotor.N1_1.fault;
                        else if (key3 == 'd')
                            apu->rotor.EGT_A0.fault = !apu->rotor.EGT_A0.fault;
                        else if (key3 == 'e')
                            apu->rotor.EGT_A1.fault = !apu->rotor.EGT_A1.fault;
                        else if (key3 == 'f')
                            apu->rotor.EGT_B0.fault = !apu->rotor.EGT_B0.fault;
                        else if (key3 == 'g')
                            apu->rotor.EGT_B1.fault = !apu->rotor.EGT_B1.fault;
                        else if (key3 == 'h')
                            apu->comp.P3.fault = !apu->comp.P3.fault;
                        else if (key3 == 'i')
                            apu->comp.T3.fault = !apu->comp.T3.fault;
                        else if (key3 == 'j')
                            apu->fan.fault = !apu->fan.fault;
                        else if (key3 == 'k')
                            apu->gen.NGC.fault = !apu->gen.NGC.fault;
                        else if (key3 == 'l')
                            apu->pump.fault = !apu->pump.fault;
                        else if (key3 == 'm')
                            apu->pump.fuel_sov.fault = !apu->pump.fuel_sov.fault;
                        else if (key3 == 'n')
                            apu->pump.fuel_sov.sensor.fault = !apu->pump.fuel_sov.sensor.fault;
                        else if (key3 == 'o')
                            apu->psys.asv.fault = !apu->psys.asv.fault;
                        else if (key3 == 'p')
                            apu->psys.asv.sensor.fault = !apu->psys.asv.sensor.fault;
                        else if (key3 == 'q')
                            apu->psys.bsv.fault = !apu->psys.bsv.fault;
                        else if (key3 == 'r')
                            apu->psys.bsv.sensor.fault = !apu->psys.bsv.sensor.fault;
                        else if (key3 == 's')
                            apu->psys.fcv.fault = !apu->psys.fcv.fault;
                        else if (key3 == 't')
                            apu->psys.fcv.sensor.fault = !apu->psys.fcv.sensor.fault;
                        else if (key3 == 'u')
                            apu->psys.mpu_xbleed.fault = !apu->psys.mpu_xbleed.fault;
                        else if (key3 == 'v')
                            apu->psys.mpu_xbleed.sensor.fault = !apu->psys.mpu_xbleed.sensor.fault;
                        else if (key3 == 'w')
                            apu->psys.P2.fault = !apu->psys.P2.fault;
                        else if (key3 == 'x')
                            apu->psys.T2.fault = !apu->psys.T2.fault;
                        else if (key3 == 'y')
                            apu->psys.P_duct.fault = !apu->psys.P_duct.fault;
                        else if (key3 == 'z')
                            apu->psys.T_duct.fault = !apu->psys.T_duct.fault;

                        // Обновление вывода
                        if (key3 >= 'a' && key3 <= 'y')
                        {
                            fault = key3;
                            system("cls");
                            toggle_fault_menu(apu);
                        }
                        key3 = _getch();
                    }
                }
                // Обновление вывода
                if (key2 == 'f' || key2 == 'h' || key2 == 'i' || key2 == 'p' || key2 == 'c'
                    || key2 == 'E' || key2 == 'e')
                {
                    system("cls");
                    phys_params_menu(phys, *c0_fault, *c1_fault);
                }
                key2 = _getch();
                if (key2 == 'A')
                    phys->auto_start = 1;
            }
        }
        else if (key == 't')
        {
            if (state == STATE_IDLE)
            {
                print_to_buffer(mb, " === TESTING ===\n", fout);
                actm->test = 1;
            }
            else
            {
                print_to_buffer(mb, " === UNABLE TO CONDUCT TEST FROM CURRENT STATE ===\n", fout);
                char temp_str[STRING_LEN];
                sprintf(&temp_str, " === CURRENT STATE: %s ===\n", apu_states[state]);
                print_to_buffer(mb, temp_str, fout);
            }
        }
        else if (key == 'r')
        {
            print_to_buffer(mb, " === PARAMETERS RESET ===\n", fout);
            actm->reset = 1;
        }
        else if (key == 's')
        {
            if (state == STATE_IDLE)
            {
                print_to_buffer(mb, " === START ===\n", fout);
                actm->start = 1;
            }
            else if (state == STATE_START ||
                state == STATE_IDLE_RUN || state == STATE_IDLE_RUN_LIMITED)
            {
                print_to_buffer(mb, " === STOP ===\n", fout);
                actm->start = 0;
                phys->auto_start = 0;
            }
            else
            {
                print_to_buffer(mb, " === UNABLE TO START/STOP FROM CURRENT STATE ===\n", fout);
                char temp_str[STRING_LEN];
                sprintf(&temp_str, " === CURRENT STATE: %s ===\n", apu_states[state]);
                print_to_buffer(mb, temp_str, fout);
            }
        }
        else if (key == 'b')
        {
            if (state == STATE_IDLE_RUN || state == STATE_GEN)
            {
                print_to_buffer(mb, " === BLEED ON ===\n", fout);
                actm->bleed = 1;
            }
            else if (state == STATE_BLEED || state == STATE_LOAD)
            {
                print_to_buffer(mb, " === BLEED OFF ===\n", fout);
                actm->bleed = 0;
            }
            else
            {
                print_to_buffer(mb, " === UNABLE TO TOGGLE BLEED FROM CURRENT STATE ===\n", fout);
                char temp_str[STRING_LEN];
                sprintf(&temp_str, " === CURRENT STATE: %s ===\n", apu_states[state]);
                print_to_buffer(mb, temp_str, fout);
            }
        }
        else if (key == 'g')
        {
            if (state == STATE_IDLE_RUN || state == STATE_BLEED ||
                state == STATE_IDLE_RUN_LIMITED)
            {
                print_to_buffer(mb, " === GEN ON ===\n", fout);
                actm->gen = 1;
            }
            else if (state == STATE_GEN || state == STATE_LOAD)
            {
                print_to_buffer(mb, " === GEN OFF ===\n", fout);
                actm->gen = 0;
            }
            else
            {
                print_to_buffer(
                    mb, " === UNABLE TO SWITCH GENERATOR FROM CURRENT STATE ===\n", fout);
                char temp_str[STRING_LEN];
                sprintf(&temp_str, " === CURRENT STATE: %s ===\n", apu_states[state]);
                print_to_buffer(mb, temp_str, fout);
            }
        }
        else if (key == 'm')
        {
            if (state == STATE_IDLE_RUN || state == STATE_GEN ||
                state == STATE_BLEED || state == STATE_LOAD ||
                state == STATE_IDLE_RUN_LIMITED || state == STATE_GEN_LIMITED)
            {
                print_to_buffer(mb, " === MPU ON ===\n", fout);
                actm->mpu_start = 1;
                actm->bleed = 0;
                actm->gen = 0;
            }
            else if (state == STATE_MPU_START || state == STATE_MPU_START_LIMITED)
            {
                print_to_buffer(mb, " === MPU OFF ===\n", fout);
                actm->mpu_start = 0;
            }
            else
            {
                print_to_buffer(mb, " === UNABLE TO SWITCH MPU FROM CURRENT STATE ===\n", fout);
                char temp_str[STRING_LEN];
                sprintf(&temp_str, " === CURRENT STATE: %s ===\n", apu_states[state]);
                print_to_buffer(mb, temp_str, fout);
            }
        }
    }
    return fault;
}