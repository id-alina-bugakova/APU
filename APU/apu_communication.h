#pragma once

#include <stdbool.h>
#include <stdint.h>
#include "apu_state_machine.h"

/* @brief Структура получения данных от СЭС
* 
*  Сообщения, которые передает ВСУ система электроснабжения
*/
typedef struct {
    /* @brief Признак наземного питания
    * 
    *  Для блокировки и/ил выключения генератора при наземном питании.
    */
    bool ext_power_avail;

    bool gen_switch;                    ///< Запрос на вкл./выкл. генератора

    /* @brief Признак подключения генератора к шине
    * 
    *  Подтверждение для ВСУ, что генератор подключен кшине СЭС.
    */
    bool apb_stat;                      
} EPS_message;

/* @brief Структура отправки данных СЭС
*
*  Сообщения, которые ВСУ передает системе электроснабжения
*/
typedef struct {
    bool power_avail;                   ///< Разрешение на запуск генератора 
} EPS_response;

/* @brief Структура получения данных от ТС
*
*  Сообщения, которые передает ВСУ топливная система
*/
typedef struct {
    bool fuel_avail;                    ///< В системе досточное количество топлива
    bool low_pres_warn;                 ///< Предупреждение о низком давлении топлива (сухой ход)
} FS_message;

/* @brief Структура отправки данных ТС
*
*  Сообщения, которые ВСУ передает топливной системе
*/
typedef struct {
    bool fuel_demand;                   ///< Запрос на подачу топлива
} FS_response;

/* @brief Структура получения данных от СКВ
*
*  Сообщения, которые передает ВСУ система кондиционирования воздуха
*/
typedef struct {
    bool bleed_air_demand;              ///< Запрос на отбор воздуха от ВСУ
    bool duct_pres;                     ///< Превышено давление в пневмосети (прикрыть клапан)
} AirCS_message;

/* @brief Структура отправки данных ТС
*
*  Сообщения, которые ВСУ передает системе кондиционирования воздуха
*/
typedef struct {
    bool apu_bleed_avail;               ///< Доступен забор воздуха от ВСУ
} AirCS_response;

/* @brief Структура получения данных от МСУ
*
*  Сообщения, которые передает ВСУ МСУ
*/
typedef struct {
    bool eng_start;                     ///< Сигнал на запуск МСУ
    bool eng_switch;                    ///< Выбор двигателя (0 - левый, 1 - правый)
    bool eng_start_cutoff;              ///< Запуск завершен/прерван
} MPU_message;

/* @brief Структура отправки данных МСУ
*
*  Сообщения, которые ВСУ передает МСУ
*/
typedef struct {
    bool apu_bleed_avail;               ///< Доступен забор воздуха от ВСУ
    bool apu_bleed_valve_stat;          ///< Статус клапана отбора воздуха от ВСУ
    bool crossbleed_valve_stat;         ///< Статус перекрестного клапана СКВ/МСУ
    bool eng_bleed_valve_stat;          ///< Статус перекрестного клапана МСУ
    bool apu_bleed_pres;                ///< Давление достаточно для запуска двигателя
} MPU_response;

/* @brief Структура получения данных от СПЗ
*
* Сообщения, которые передает ВСУ система пожарной защиты
*/
typedef struct {
    bool fire_sig;                      ///< Сигнал о пожаре
} FPS_message;

/* @brief Структура получения данных от СДУ
*
* Сообщения, которые передает ВСУ система дистанционного управления
*/
typedef struct {
    bool apu_power;                     ///< Подано питание на ВСУ
    bool test;                          ///< Запуск диагностики
    bool start_cmd;                     ///< Команда на запуск ВСУ
    bool stop_cmd;                      ///< Команда на остановку ВСУ
} RCS_message;

/* @brief Структура отправки данных СДУ
*
*  Сообщения, которые ВСУ передает системе дистанционного управления
*/
typedef struct {
    bool power_on;                      ///< Индикация включения
    bool apu_fault;                     ///< Индикация отказов
    bool critical_fault;                ///< Индикация критического отказа
    bool start;                         ///< Индикация запуска
    double N1;                          ///< Обороты двигателя
    double EGT;                         ///< Температура выхлопных газов
    bool avail;                         ///< Индикация выхода на малые обороты
    bool stop;                          ///< Индикация остановки
    bool auto_shutdown;                 ///< При превышении критического параматера
} RCS_response;

/* @brief Структура получения данных от САУ
*
* Сообщения, которые передает ВСУ система автоматического управления
*/
typedef struct {
    bool auto_start;                    ///< Команда автоматического запуска
} AutoCS_message;

/* @brief Объединенная структра получения данных (сообщений)
*/
typedef struct {
    EPS_message eps;                    ///< Система электроснабжения
    FS_message fs;                      ///< Топливная система
    AirCS_message air_cs;               ///< Система кондиционирования воздуха
    MPU_message mpu;                    ///< Маршевая силовая установка
    FPS_message fps;                    ///< Система пожарной защиты
    RCS_message rcs;                    ///< Система дистанционного управления
    AutoCS_message auto_cs;             ///< Система автоматического управления
} Messages;

/* @brief Объединенная структра отправки данных (ответов)
*/
typedef struct {
    EPS_response eps;                   ///< Система электроснабжения
    FS_response fs;                     ///< Топливная система
    AirCS_response air_cs;              ///< Система кондиционирования воздуха
    MPU_response mpu;                   ///< Маршевая силовая установка
    RCS_response rcs;                   ///< Система дистанционного управления
} Responses;

/* @brief Структура действий
* 
*  Действия выполняются менеджером после отработки контроллера 
*/
typedef struct {
    bool power_on_cmd;                  ///< Подача питания на ВСУ
    bool turn_on_cmd;                   ///< Запуск ВСУ
    bool stop_testing;                  ///< Тестирование завершено
    bool abort_start;                   ///< Команда прерывания запуска
    bool pump_on;                       ///< Включить топливный насос
    bool ignite;                        ///< Подать зажигание
    bool set_N;                         ///< Оповещение об измененеии целевых оборотов 
    double N_target;                    ///< Установить целевые обороты ротора
    bool open_asv;                      ///< Команда открыть клапан забора атмосферного воздуха
    bool close_asv;                     ///< Команда закрыть клапан забора атмосферного воздуха
    bool update_air_demand;             ///< Оповещение об изменении подачи воздуха
    bool open_bsv;                      ///< Команда открыть клапан отбора воздуха от ВСУ
    bool close_bsv;                     ///< Команда закрыть клапан отбора воздуха от ВСУ
    bool switch_fcv_to_acs;             ///< Команда открыть перекрестный клапан на СКВ
    bool switch_fcv_to_mpu;             ///< Команда открыть перекрестный клапан на МСУ
    bool switch_xbleed_to_left;         ///< Команда открыть перекрестный клапан на левый двигатель
    bool switch_xbleed_to_right;        ///< Команда открыть перекрестный клапан на прав. двигатель
    bool update_gen_demand;             ///< Оповещение об изменении запроса на генератор
    bool gen_demand;                    ///< Запрос на генератор
    bool mpu_start_cutoff_done;         ///< Сообщение о завершении страта МСУ
    bool turn_off_cmd;                  ///< Остановка ВСУ
    bool emergency_shtdn;               ///< Аварийная остановка ВСУ
    bool power_off_cmd;                 ///< Отключение ВСУ
    bool watchdog_shutdown;             ///< Остановка watchdog
} Actions;

typedef struct {
    bool power;                         ///< Подать/отключить питание
    bool test;                          ///< Запустить тестирование
    bool reset;                         ///< Сбросить неисправности
    bool start;                         ///< Запустить/остановить ВСУ
    bool bleed;                         ///< Подать воздух/прекратить подачу воздуха в СКВ
    bool gen;                           ///< Включить/выключить генератор
    bool mpu_start;                     ///< Начать/закончить запуск МСУ
} Actions_manual;

/* @brief Структура данных
* 
*  Общая структура, в которую каналы контроллера записывают информацию о состоянии системы
*/
typedef struct {
    State prev_state;                   ///< Предыдущее состояние (см. конец update_controller)
    State parent_state;                 ///< Родительское состояние (см. конец update_controller)
    bool fault;                         ///< Обнаружена неисправность
    bool critical_fault;                ///< Обнаружен критический отказ (напр. топливного насоса)
    bool emergency_found;               ///< Обнаружена аварийная ситуация (напр. перегрев)
    bool fire;                          ///< Обнаружен пожар
    bool starting;                      ///< ВСУ выходит на обороты
    uint32_t last_start_cmd;            ///< Время начала последнего перехода между оборотами
    double EGT_cur;                     ///< Текущая температура выхлопных газов
    double N_cur;                       ///< Текущие обороты ротора
    bool ovheat;                        ///< Признак перегрева
    uint32_t last_ovheat_detected;      ///< Последнее время обнаружения перегрева
    bool ovspeed;                       ///< Признак разноса ротора
    uint32_t last_ovspeed_detected;     ///< Последнее время обнаружения разноса ротора
    bool demanded_fuel;                 ///< Признак запроса топлива
    uint32_t last_demanded_fuel;        ///< Последнее время запроса топлива
    bool ignited;                       ///< Признак наличия зажигания
    uint32_t last_ignited;              ///< Последнее время зажигания
    bool flame_was_on;                  ///< Признак того, что в камере сгорания появлялся факел
    uint32_t last_flame_out;            ///< Последнее время погасания факела
    bool last_flame_out_updated;        ///< Последнее время погасания факела
    bool just_started_working;          ///< ВСУ только что вышла на рабочие обороты (для генератора)
    bool cooling;                       ///< Признак охлаждения
    uint32_t last_cooldown_start;       ///< Время начала последнего охлаждения
} Data;

/* @brief Физические параметры
*/
typedef struct {
    bool fire;                          ///< Пожар в отсеке
    unsigned int height;                ///< Текущая высота
    bool auto_start;                    ///< Автоматический запуск
    bool ignited;                       ///< Признак наличия зажигания
    bool enough_pressure;               ///< Достаточно ли давления для СКВ/МСУ
} Physical;

void init_messages(Messages* msgs);

void init_responses(Responses* rsps);

void init_actions(Actions* acts);

void init_actions_manual(Actions_manual* actm);

void init_data(Data* data);

void init_physical(Physical* phys);

void reset_messages(Messages* msgs);

void copy_responses(Responses* original, Responses* copy);