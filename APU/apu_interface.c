#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "apu_interface.h"

void init_file_output(File_output* fout)
{
    fout->first_output = 1;
    strcpy(fout->starter_M, "output/starter_M.data");
    strcpy(fout->pid_fuel_feed, "output/pid_fuel_feed.data");
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


void print_to_buffer(Message_buffer* mb, char* str[STRING_LEN])
{
    strcpy(mb->buffer[(mb->cur_begin + mb->filled) % N_MESSAGES], str);
    if (mb->filled == 10)
        mb->cur_begin = (mb->cur_begin + 1) % N_MESSAGES;
    mb->filled = MIN(mb->filled + 1, 10);
    mb->updated = 1;
}

void move_to(int x, int y)
{
    char temp[STRING_LEN];
    sprintf(&temp, "\033[%d;%dH", y + 1, x + 1);
    printf("%s", temp);
}

void start_menu()
{

}

void printer(
    Output* out,
    Message_buffer* mb,
    Responses* rsps,
    Responses* prev_rsps,
    uint32_t cur_time,
    int height,
    int* prev_height,
    State state,
    State* prev_state,
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
        if (!out->detailed_output)
        {
            // Выводим подложку
            for (int i = 0; i < N_NORMAL_LAYOUT; i++)
                printf("%s", normal_layout[i]);
        }
        else
        {
            // Выводим подложку
            for (int i = 0; i < N_DETAILED_LAYOUT; i++)
                printf("%s", detailed_layout[i]);
        }
        // Выводим команды
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
        printf("%s\n", out->detailed_output? detailed_layout[0] : normal_layout[0]);

        fflush(stdout);
    }

    // Обновляем значения на панели
    // Состояние
    if (state != prev_state || !out->setup_done)
    {
        move_to(POS_X_GENERAL_COL, POS_Y_STATE);
        printf("%s                    ", states[state]);
    }
    // Высота
    if (prev_height != height)
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
        
    }
    fflush(stdout);

    // Перемещаемся к выводу сообщений
    if (mb->updated || !out->setup_done)
    {
        if (!out->detailed_output)
            move_to(0, out->normal_message_begin - 1);
        else
            move_to(0, out->detailed_message_begin - 1);
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
    copy_responses(rsps, prev_rsps);
    copy_APU(apu, prev_apu);

}

void handle_key_press(
    char key, 
    bool manual_mode, 
    Output* out, 
    Message_buffer* mb, 
    State state,
    bool power,
    Actions_manual* actm)
{
    const char* apu_states[] = APU_STATES;

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
    if (manual_mode)
    {
        if (key == 'p')
        {
            if (power)
            {
                print_to_buffer(&mb, " === POWER ON ===\n");
                actm->power = 1;
            }
            else
            {
                print_to_buffer(&mb, " === POWER OFF ===\n");
                actm->power = 0;
            }
        }
        else if (key == 't')
        {
            if (state == STATE_IDLE)
            {
                print_to_buffer(&mb, " === TESTING ===\n");
                actm->test = 1;
            }
            else
            {
                print_to_buffer(&mb, " === UNABLE TO CONDUCT TEST FROM CURRENT STATE ===\n");
                char temp_str[STRING_LEN];
                sprintf(&temp_str, " === CURRENT STATE: %s ===\n", apu_states[state]);
                print_to_buffer(&mb, temp_str);
            }
        }
        else if (key == 'r')
        {
            print_to_buffer(&mb, " === PARAMETERS RESET ===\n");
            actm->reset = 1;
        }
        else if (key == 's')
        {
            if (state == STATE_IDLE)
            {
                print_to_buffer(&mb, " === START ===\n");
                actm->start = 1;
            }
            else if (state == STATE_START ||
                state == STATE_IDLE_RUN || state == STATE_IDLE_RUN_LIMITED)
            {
                print_to_buffer(&mb, " === STOP ===\n");
                actm->start = 0;
            }
            else
            {
                print_to_buffer(&mb, " === UNABLE TO START/STOP FROM CURRENT STATE ===\n");
                char temp_str[STRING_LEN];
                sprintf(&temp_str, " === CURRENT STATE: %s ===\n", apu_states[state]);
                print_to_buffer(&mb, temp_str);
            }
        }
        else if (key == 'b')
        {
            if (state == STATE_IDLE_RUN || state == STATE_GEN)
            {
                print_to_buffer(&mb, " === BLEED ON ===\n");
                actm->bleed = 1;
            }
            else if (state == STATE_BLEED || state == STATE_LOAD)
            {
                print_to_buffer(&mb, " === BLEED OFF ===\n");
                actm->bleed = 0;
            }
            else
            {
                print_to_buffer(&mb, " === UNABLE TO TOGGLE BLEED FROM CURRENT STATE ===\n");
                char temp_str[STRING_LEN];
                sprintf(&temp_str, " === CURRENT STATE: %s ===\n", apu_states[state]);
                print_to_buffer(&mb, temp_str);
            }
        }
        else if (key == 'g')
        {
            if (state == STATE_IDLE_RUN || state == STATE_BLEED ||
                state == STATE_IDLE_RUN_LIMITED)
            {
                print_to_buffer(&mb, " === GEN ON ===\n");
                actm->gen = 1;
            }
            else if (state == STATE_GEN || state == STATE_LOAD)
            {
                print_to_buffer(&mb, " === GEN OFF ===\n");
                actm->gen = 0;
            }
            else
            {
                print_to_buffer(
                    &mb, " === UNABLE TO SWITCH GENERATOR FROM CURRENT STATE ===\n");
                char temp_str[STRING_LEN];
                sprintf(&temp_str, " === CURRENT STATE: %s ===\n", apu_states[state]);
                print_to_buffer(&mb, temp_str);
            }
        }
        else if (key == 'm')
        {
            if (state == STATE_IDLE_RUN || state == STATE_GEN ||
                state == STATE_BLEED || state == STATE_LOAD)
            {
                print_to_buffer(&mb, " === MPU ON ===\n");
                actm->mpu_start = 1;
                actm->bleed = 0;
                actm->gen = 0;
            }
            else if (state == STATE_MPU_START)
            {
                print_to_buffer(&mb, " === MPU OFF ===\n");
                actm->mpu_start = 0;
            }
            else
            {
                print_to_buffer(&mb, " === UNABLE TO SWITCH MPU FROM CURRENT STATE ===\n");
                char temp_str[STRING_LEN];
                sprintf(&temp_str, " === CURRENT STATE: %s ===\n", apu_states[state]);
                print_to_buffer(&mb, temp_str);
            }
        }
    }
}