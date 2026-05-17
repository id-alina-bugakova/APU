#pragma once

#define NORMAL_LAYOUT                                                        \
    {                                                                        \
        "============================================================\n",    \
        "    STATE:                                                  \n",    \
        "    HEIGHT:                                                 \n",    \
        "    TIME:                                                   \n",    \
        "============================================================\n",    \
        "        POWER    [ ]                FAULT       [ ]         \n",    \
        "        START    [ ]                CRIT FAULT  [ ]         \n",    \
        "        AVAIL    [ ]                AUTO SHTDN  [ ]         \n",    \
        "        STOP     [ ]                                        \n",    \
        "                                                            \n",    \
        "        N1                                                  \n",    \
        "        EGT                                                 \n",    \
        "============================================================\n",    \
    }
#define N_NORMAL_LAYOUT 13

#define DETAILED_LAYOUT                                                                                                                                      \
    {                                                                                                                                                        \
        "============================================================================================================================================\n",    \
        "    STATE:                                                                                                                                  \n",    \
        "    HEIGHT:                                                                                                                                 \n",    \
        "    TIME:                                                                                                                                   \n",    \
        "============================================================================================================================================\n",    \
        "        POWER    [ ]                FAULT       [ ]         |        EPS:                            MPU:                                   \n",    \
        "        START    [ ]                CRIT FAULT  [ ]         |          POWER AVAIL  [ ]                 BLEED AVAIL         [ ]             \n",    \
        "        AVAIL    [ ]                AUTO SHTDN  [ ]         |        FS:                                BLEED VLV STAT      [ ]             \n",    \
        "        STOP     [ ]                                        |          FUEL DEMAND  [ ]                 XBLEED VLV STAT     [ ]             \n",    \
        "                                                            |        AirCS:                             ENG BLEED VLV STAT  [ ]             \n",    \
        "        N1                                                  |          BLEED AVAIL  [ ]                 ENOUGH BLEED PRES   [ ]             \n",    \
        "        EGT                                                 |                                                                               \n",    \
        "============================================================================================================================================\n",    \
        "    STARTER:         [ ]    ROTOR:                COMPRESSOR:      GENERATOR:                PNUEMATIC SYSTEM:                              \n",    \
        "      M                       N TARGET             BLEED    [ ]       ON              [ ]    ASV:           [ ]    FCV:              [ ]    \n",    \
        "                              N                    M                  M                        FAULT        [ ]      FAULT           [ ]    \n",    \
        "    PID:                      EGT                  P3               NGC:              [ ]      OPEN CMD     [ ]      OPEN CMD        [ ]    \n",    \
        "      FUEL FEED             N1 0:         [ ]      T3                 FAULT           [ ]      CLOSE CMD    [ ]      CLOSE CMD       [ ]    \n",    \
        "      P COMP                  FAULT       [ ]    P3:        [ ]       VALUE                    OPEN         [ ]      OPEN            [ ]    \n",    \
        "      I COMP                  VALUE                FAULT    [ ]                              ASV SENSOR:    [ ]    FCV SENSOR:       [ ]    \n",    \
        "      D COMP               N1 1:         [ ]      VALUE            FUEL PUMP:        [ ]       FAULT        [ ]      FAULT           [ ]    \n",    \
        "                              FAULT       [ ]    T3:        [ ]       TURN ON CMD     [ ]      VALUE        [ ]      VALUE           [ ]    \n",    \
        "    GAS GENERATOR:            VALUE                FAULT    [ ]       TURN OFF CMD    [ ]    BSV:           [ ]    XBLEED:           [ ]    \n",    \
        "      IGNITED        [ ]    EGT A0:       [ ]      VALUE              ON              [ ]      FAULT        [ ]      FAULT           [ ]    \n",    \
        "      FUEL CMD              FAULT       [ ]                         M                          OPEN CMD     [ ]      OPEN CMD        [ ]    \n",    \
        "      FUEL                    VALUE              COOLING FAN:       FUEL SOV:         [ ]      CLOSE CMD    [ ]      CLOSE CMD       [ ]    \n",    \
        "      M                     EGT A1:       [ ]      FAULT    [ ]       FAULT           [ ]      OPEN         [ ]      OPEN            [ ]    \n",    \
        "    FLAME SENSOR:    [ ]      FAULT       [ ]      N                  OPEN CMD        [ ]    BSV SENSOR:    [ ]    XBLEED SENSOR:    [ ]    \n",    \
        "      FAULT          [ ]      VALUE                M                  CLOSE CMD       [ ]      FAULT        [ ]      FAULT           [ ]    \n",    \
        "      VALUE          [ ]    EGT B0:       [ ]                         OPEN            [ ]      VALUE        [ ]      VALUE           [ ]    \n",    \
        "                              FAULT       [ ]                       FUEL SOV SENSOR:  [ ]    P2:            [ ]    P DUCT:           [ ]    \n",    \
        "========================      VALUE                                   FAULT           [ ]      FAULT        [ ]      FAULT           [ ]    \n",    \
        "    ECU channel 0:          EGT B1:       [ ]                         VALUE           [ ]      VALUE                 VALUE                  \n",    \
        "      FAILURE        [ ]      FAULT       [ ]                                                T2:            [ ]    T DUCT:           [ ]    \n",    \
        "    ECU channel 1:            VALUE                                                            FAULT        [ ]      FAULT           [ ]    \n",    \
        "      FAILURE        [ ]                                                                       VALUE                 VALUE                  \n",    \
        "============================================================================================================================================\n"     \
    }
#define N_DETAILED_LAYOUT 37

// Общие символы и форматные строки
#define SMB_1 'X'                                           // Символ, которым отмечаются истинные поля
#define SMB_0 ' '                                           // Символ, которым отмечаются ложные поля
#define BRACKET_FORMAT "  [%c] "                            // Форматная строка для вывода скобки
#define BRACKET_SMB(condition) (condition? SMB_1 : SMB_0)   // Условие выбора символа в скобке
#define EXP_FORMAT "%8.1e "                                  // Экспоненциальный формат для коэффициентов ПИД
#define N_FORMAT "%5.0f "                                   // Форматная строка для оборотов
#define M_FORMAT "%5.1f "                                   // Форматная строка для момента
#define P_FORMAT "%5.1f "                                   // Форматная строка для давления
#define T_FORMAT "%5.1f "                                   // Форматная строка для температуры
#define FUEL_FEED_FORMAT "%5.3f "                           // Форматная строка для подачи топлива (0..1)
#define FUEL_FORMAT "%5.2f "                                // Форматная строка для топлива (0..12)
#define HEIGHT_FORMAT "%d        "                          // Форматная строка высоты
#define TIME_FORMAT "%.2f s (%d tacts)        "             // Форматная строка для времени

// Положения отдельных элементов (начиная с 0, X - колонка, Y - строка)
#define SCENARIO_MODE_SHORTER 8

#define POS_X_GENERAL_COL 12
#define POS_Y_STATE 1
#define POS_Y_HEIGHT (POS_Y_STATE + 1)
#define POS_Y_TIME   (POS_Y_STATE + 2)

#define POS_X_COMMON_COL1 15
#define POS_Y_POWER (POS_Y_TIME + 2)
#define POS_Y_START (POS_Y_POWER + 1)
#define POS_Y_AVAIL (POS_Y_POWER + 2)
#define POS_Y_STOP  (POS_Y_POWER + 3)
#define POS_Y_N1    (POS_Y_POWER + 5)
#define POS_Y_EGT   (POS_Y_POWER + 6)

#define POS_X_COMMON_COL2 46
#define POS_Y_FAULT       POS_Y_POWER
#define POS_Y_CRIT_FAULT (POS_Y_FAULT + 1)
#define POS_Y_AUTO_SHTDN (POS_Y_FAULT + 2)

#define POS_X_RSPS_COL1 82
#define POS_Y_POWER_AVAIL     (POS_Y_POWER + 1)
#define POS_Y_FUEL_DEMAND     (POS_Y_POWER_AVAIL + 2)
#define POS_Y_ACS_BLEED_AVAIL (POS_Y_POWER_AVAIL + 4)

#define POS_X_RSPS_COL2 122
#define POS_Y_MPU_BLEED_AVAIL     POS_Y_POWER_AVAIL
#define POS_Y_BLEED_VLV_STAT     (POS_Y_MPU_BLEED_AVAIL + 1)
#define POS_Y_XBLEED_VLV_STAT    (POS_Y_MPU_BLEED_AVAIL + 2)
#define POS_Y_ENG_BLEED_VLV_STAT (POS_Y_MPU_BLEED_AVAIL + 3)
#define POS_Y_ENOUGH_BLEED_PRES  (POS_Y_MPU_BLEED_AVAIL + 4)

#define POS_X_COMPS_COL1 19
#define POS_Y_START_POWER      (POS_Y_EGT + 2)
#define POS_Y_START_M          (POS_Y_START_POWER + 1)
#define POS_Y_PID_FUEL_FEED    (POS_Y_START_M + 3)
#define POS_Y_PID_P_COMP       (POS_Y_START_M + 4)
#define POS_Y_PID_I_COMP       (POS_Y_START_M + 5)
#define POS_Y_PID_D_COMP       (POS_Y_START_M + 6)
#define POS_Y_GGEN_IGNITED     (POS_Y_START_M + 9)
#define POS_Y_GGEN_FUEL_CMD    (POS_Y_START_M + 10)
#define POS_Y_GGEN_FUEL        (POS_Y_START_M + 11)
#define POS_Y_GGEN_M           (POS_Y_START_M + 12)
#define POS_Y_GGEN_FLAME_POWER (POS_Y_START_M + 13)
#define POS_Y_GGEN_FLAME_FAULT (POS_Y_START_M + 14)
#define POS_Y_GGEN_FLAME_VALUE (POS_Y_START_M + 15)
// Также канала ECU
#define POS_Y_ECU0 (POS_Y_GGEN_FLAME_VALUE + 4)
#define POS_Y_ECU1 (POS_Y_ECU0 + 2)

#define POS_X_COMPS_COL2 40
#define POS_Y_ROTOR_NTARGET      POS_Y_START_M
#define POS_Y_ROTOR_N           (POS_Y_ROTOR_NTARGET + 1)
#define POS_Y_ROTOR_EGT         (POS_Y_ROTOR_NTARGET + 2)
#define POS_Y_ROTOR_N10_POWER   (POS_Y_ROTOR_NTARGET + 3)
#define POS_Y_ROTOR_N10_FAULT   (POS_Y_ROTOR_NTARGET + 4)
#define POS_Y_ROTOR_N10_VALUE   (POS_Y_ROTOR_NTARGET + 5)
#define POS_Y_ROTOR_N11_POWER   (POS_Y_ROTOR_NTARGET + 6)
#define POS_Y_ROTOR_N11_FAULT   (POS_Y_ROTOR_NTARGET + 7)
#define POS_Y_ROTOR_N11_VALUE   (POS_Y_ROTOR_NTARGET + 8)
#define POS_Y_ROTOR_EGTA0_POWER (POS_Y_ROTOR_NTARGET + 9)
#define POS_Y_ROTOR_EGTA0_FAULT (POS_Y_ROTOR_NTARGET + 10)
#define POS_Y_ROTOR_EGTA0_VALUE (POS_Y_ROTOR_NTARGET + 11)
#define POS_Y_ROTOR_EGTA1_POWER (POS_Y_ROTOR_NTARGET + 12)
#define POS_Y_ROTOR_EGTA1_FAULT (POS_Y_ROTOR_NTARGET + 13)
#define POS_Y_ROTOR_EGTA1_VALUE (POS_Y_ROTOR_NTARGET + 14)
#define POS_Y_ROTOR_EGTB0_POWER (POS_Y_ROTOR_NTARGET + 15)
#define POS_Y_ROTOR_EGTB0_FAULT (POS_Y_ROTOR_NTARGET + 16)
#define POS_Y_ROTOR_EGTB0_VALUE (POS_Y_ROTOR_NTARGET + 17)
#define POS_Y_ROTOR_EGTB1_POWER (POS_Y_ROTOR_NTARGET + 18)
#define POS_Y_ROTOR_EGTB1_FAULT (POS_Y_ROTOR_NTARGET + 19)
#define POS_Y_ROTOR_EGTB1_VALUE (POS_Y_ROTOR_NTARGET + 20)

#define POS_X_COMPS_COL3 58
#define POS_Y_COMP_BLEED     POS_Y_START_M
#define POS_Y_COMP_M        (POS_Y_COMP_BLEED + 1)
#define POS_Y_COMP_P3       (POS_Y_COMP_BLEED + 2)
#define POS_Y_COMP_T3       (POS_Y_COMP_BLEED + 3)
#define POS_Y_COMP_P3_POWER (POS_Y_COMP_BLEED + 4)
#define POS_Y_COMP_P3_FAULT (POS_Y_COMP_BLEED + 5)
#define POS_Y_COMP_P3_VALUE (POS_Y_COMP_BLEED + 6)
#define POS_Y_COMP_T3_POWER (POS_Y_COMP_BLEED + 7)
#define POS_Y_COMP_T3_FAULT (POS_Y_COMP_BLEED + 8)
#define POS_Y_COMP_T3_VALUE (POS_Y_COMP_BLEED + 9)
#define POS_Y_FAN_FAULT     (POS_Y_COMP_T3_VALUE + 3)
#define POS_Y_FAN_N         (POS_Y_FAN_FAULT + 1)
#define POS_Y_FAN_M         (POS_Y_FAN_FAULT + 2)

#define POS_X_COMPS_COL4 84
#define POS_Y_GEN_ON                      POS_Y_START_M
#define POS_Y_GEN_M                      (POS_Y_GEN_ON + 1)
#define POS_Y_GEN_NGC_POWER              (POS_Y_GEN_ON + 2)
#define POS_Y_GEN_NGC_FAULT              (POS_Y_GEN_ON + 3)
#define POS_Y_GEN_NGC_VALUE              (POS_Y_GEN_ON + 4)
#define POS_Y_PUMP_POWER                 (POS_Y_GEN_NGC_VALUE + 2)
#define POS_Y_PUMP_TURN_ON               (POS_Y_PUMP_POWER + 1)
#define POS_Y_PUMP_TURN_OFF              (POS_Y_PUMP_POWER + 2)
#define POS_Y_PUMP_ON                    (POS_Y_PUMP_POWER + 3)
#define POS_Y_PUMP_M                     (POS_Y_PUMP_POWER + 4)
#define POS_Y_PUMP_FUEL_SOV_POWER        (POS_Y_PUMP_POWER + 5)
#define POS_Y_PUMP_FUEL_SOV_FAULT        (POS_Y_PUMP_POWER + 6)
#define POS_Y_PUMP_FUEL_SOV_OPEN_CMD     (POS_Y_PUMP_POWER + 7)
#define POS_Y_PUMP_FUEL_SOV_CLOSE_CMD    (POS_Y_PUMP_POWER + 8)
#define POS_Y_PUMP_FUEL_SOV_OPEN         (POS_Y_PUMP_POWER + 9)
#define POS_Y_PUMP_FUEL_SOV_SENSOR_POWER (POS_Y_PUMP_POWER + 10)
#define POS_Y_PUMP_FUEL_SOV_SENSOR_FAULT (POS_Y_PUMP_POWER + 11)
#define POS_Y_PUMP_FUEL_SOV_SENSOR_VALUE (POS_Y_PUMP_POWER + 12)

#define POS_X_COMPS_COL5 106
#define POS_Y_PSYS_ASV_POWER         POS_Y_START_M
#define POS_Y_PSYS_ASV_FAULT        (POS_Y_PSYS_ASV_POWER + 1)
#define POS_Y_PSYS_ASV_OPEN_CMD     (POS_Y_PSYS_ASV_POWER + 2)
#define POS_Y_PSYS_ASV_CLOSE_CMD    (POS_Y_PSYS_ASV_POWER + 3)
#define POS_Y_PSYS_ASV_OPEN         (POS_Y_PSYS_ASV_POWER + 4)
#define POS_Y_PSYS_ASV_SENSOR_POWER (POS_Y_PSYS_ASV_POWER + 5)
#define POS_Y_PSYS_ASV_SENSOR_FAULT (POS_Y_PSYS_ASV_POWER + 6)
#define POS_Y_PSYS_ASV_SENSOR_VALUE (POS_Y_PSYS_ASV_POWER + 7)
#define POS_Y_PSYS_BSV_POWER        (POS_Y_PSYS_ASV_POWER + 8)
#define POS_Y_PSYS_BSV_FAULT        (POS_Y_PSYS_ASV_POWER + 9)
#define POS_Y_PSYS_BSV_OPEN_CMD     (POS_Y_PSYS_ASV_POWER + 10)
#define POS_Y_PSYS_BSV_CLOSE_CMD    (POS_Y_PSYS_ASV_POWER + 11)
#define POS_Y_PSYS_BSV_OPEN         (POS_Y_PSYS_ASV_POWER + 12)
#define POS_Y_PSYS_BSV_SENSOR_POWER (POS_Y_PSYS_ASV_POWER + 13)
#define POS_Y_PSYS_BSV_SENSOR_FAULT (POS_Y_PSYS_ASV_POWER + 14)
#define POS_Y_PSYS_BSV_SENSOR_VALUE (POS_Y_PSYS_ASV_POWER + 15)
#define POS_Y_PSYS_P2_POWER         (POS_Y_PSYS_ASV_POWER + 16)
#define POS_Y_PSYS_P2_FAULT         (POS_Y_PSYS_ASV_POWER + 17)
#define POS_Y_PSYS_P2_VALUE         (POS_Y_PSYS_ASV_POWER + 18)
#define POS_Y_PSYS_T2_POWER         (POS_Y_PSYS_ASV_POWER + 19)
#define POS_Y_PSYS_T2_FAULT         (POS_Y_PSYS_ASV_POWER + 20)
#define POS_Y_PSYS_T2_VALUE         (POS_Y_PSYS_ASV_POWER + 21)

#define POS_X_COMPS_COL6 131
#define POS_Y_PSYS_FCV_POWER            POS_Y_START_M
#define POS_Y_PSYS_FCV_FAULT           (POS_Y_PSYS_FCV_POWER + 1)
#define POS_Y_PSYS_FCV_OPEN_CMD        (POS_Y_PSYS_FCV_POWER + 2)
#define POS_Y_PSYS_FCV_CLOSE_CMD       (POS_Y_PSYS_FCV_POWER + 3)
#define POS_Y_PSYS_FCV_OPEN            (POS_Y_PSYS_FCV_POWER + 4)
#define POS_Y_PSYS_FCV_SENSOR_POWER    (POS_Y_PSYS_FCV_POWER + 5)
#define POS_Y_PSYS_FCV_SENSOR_FAULT    (POS_Y_PSYS_FCV_POWER + 6)
#define POS_Y_PSYS_FCV_SENSOR_VALUE    (POS_Y_PSYS_FCV_POWER + 7)
#define POS_Y_PSYS_XBLEED_POWER        (POS_Y_PSYS_FCV_POWER + 8)
#define POS_Y_PSYS_XBLEED_FAULT        (POS_Y_PSYS_FCV_POWER + 9)
#define POS_Y_PSYS_XBLEED_OPEN_CMD     (POS_Y_PSYS_FCV_POWER + 10)
#define POS_Y_PSYS_XBLEED_CLOSE_CMD    (POS_Y_PSYS_FCV_POWER + 11)
#define POS_Y_PSYS_XBLEED_OPEN         (POS_Y_PSYS_FCV_POWER + 12)
#define POS_Y_PSYS_XBLEED_SENSOR_POWER (POS_Y_PSYS_FCV_POWER + 13)
#define POS_Y_PSYS_XBLEED_SENSOR_FAULT (POS_Y_PSYS_FCV_POWER + 14)
#define POS_Y_PSYS_XBLEED_SENSOR_VALUE (POS_Y_PSYS_FCV_POWER + 15)
#define POS_Y_PSYS_PDUCT_POWER         (POS_Y_PSYS_FCV_POWER + 16)
#define POS_Y_PSYS_PDUCT_FAULT         (POS_Y_PSYS_FCV_POWER + 17)
#define POS_Y_PSYS_PDUCT_VALUE         (POS_Y_PSYS_FCV_POWER + 18)
#define POS_Y_PSYS_TDUCT_POWER         (POS_Y_PSYS_FCV_POWER + 19)
#define POS_Y_PSYS_TDUCT_FAULT         (POS_Y_PSYS_FCV_POWER + 20)
#define POS_Y_PSYS_TDUCT_VALUE         (POS_Y_PSYS_FCV_POWER + 21)

// Размер кольцевого буфера = число последних сообщений на экране
#define N_MESSAGES 10

#include "apu_defs.h"
#include "apu_components.h"
#include "apu_communication.h"
#include "apu_state_machine.h"

typedef struct {
    bool detailed_output;       ///< Признак режима детального вывода
    bool setup_done;            ///< Признак вывода фона страницы
    int normal_height;          ///< Высота консоли в нормальном режиме
    int normal_width;           ///< Ширина консоли в нормальном режиме
    int detailed_height;        ///< Высота консоли в режиме детального вывода
    int detailed_width;         ///< Ширина консоли в режиме детального вывода
    int normal_message_begin;   ///< Индекс строки, начиная с которой выводятся сообщения в норм.
    int detailed_message_begin; ///< Индекс строки, начиная с которой выводятся сообщения в детал.
} Output;

void init_output(Output* out);

typedef struct
{
    bool first_output;                  ///<
    char starter_M[STRING_LEN];         ///< Относительные пути к файлам
    char pid_fuel_feed[STRING_LEN];
    char pid_p_component[STRING_LEN];
    char pid_i_component[STRING_LEN];
    char pid_d_component[STRING_LEN];
    char ggen_fuel_cmd[STRING_LEN];
    char ggen_fuel[STRING_LEN];
    char ggen_M[STRING_LEN];
    char rotor_N[STRING_LEN];
    char rotor_EGT[STRING_LEN];
    char comp_M[STRING_LEN];
    char comp_P3[STRING_LEN];
    char comp_T3[STRING_LEN];
    char fan_M[STRING_LEN];
    char gen_M[STRING_LEN];
    char pump_M[STRING_LEN];
    char psys_P2[STRING_LEN];
    char psys_T2[STRING_LEN];
    char psys_P_duct[STRING_LEN];
    char psys_T_duct[STRING_LEN];
    char temp[STRING_LEN];        
    bool first_log;            
    char log[STRING_LEN];
} File_output;

void init_file_output(File_output* fout);
void write_files(File_output* fout, APU* apu);

typedef struct {
    char buffer[N_MESSAGES][STRING_LEN];    ///< Кольцевой буфер строк
    int filled;                             ///< Число записей в буфере (от 0 до N_MESSAGES)
    int cur_begin;                          ///< Текущее начало буфера
    bool updated;                           ///< Добавлена новая запись
} Message_buffer;

void init_buffer(Message_buffer* mb);
// Запись в буфер
void print_to_buffer(Message_buffer* mb, char* str[STRING_LEN], File_output* fout);

void move_to(int x, int y);

// Основная функция вывода
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
    APU* prev_apu);

int get_scenario();

// Обработка ввода
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
    Actions_manual* actm);