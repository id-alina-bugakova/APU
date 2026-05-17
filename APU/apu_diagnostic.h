/* @file   apu_diagnostic.h
*  @brief  Функции для проверки и сброса отказов в системе
*
*  @detail Файл содержит функции для проверки и сброса отказов в системе, а также структуру
*  признаков уведомления об отказах.
*
*  @author Бугакова А.А.
*/


#pragma once


#include "apu_communication.h"
#include "apu_components.h"
#include "apu_interface.h"


/* @brief Структура признаков уведомления об отказах
* 
*  Структура содержит признаки того, что сообщение о конкретном отказе уже было выведено. Префиксом
*  c_ обозначаются критичсекие отказы
*/
typedef struct {
    bool c_flame_sensor;        ///< Отказ дачика пламени в камере сгорания
    bool c_2_N1;                ///< Отказ всех (2) датчиков частоты вращения ротора
    bool c_4_EGT;               ///< Отказ всех (4) датчиков температуры выхлопных газов
    bool c_P3;                  ///< Отказ датчика давления за компрессором
    bool c_T3;                  ///< Отказ датчика температуры за компрессором
    bool c_pump;                ///< Отказ топливного насоса
    bool c_fuel_sov;            ///< Отказ клапана отсечки топлива
    bool c_fuel_sov_sensor;     ///< Отказ датчка положения клапана отсечки топлива
    bool c_asv;                 ///< Отказ клапана забора атмосферного воздуха (закрыт)
    bool c_asv_sensor;          ///< Отказ датчика положения клапана забора атмосферного воздуха
    bool c_bsv;                 ///< Отказ клапана отбора воздуха от ВСУ
    bool c_bsv_sensor;          ///< Отказ датчка положения клапана отбора воздуха от ВСУ
    bool c_fcv;                 ///< Отказ перекрестного клапана СКВ/МСУ (открыт на СКВ)
    bool c_fcv_sensor;          ///< Отказ датчика положения перекрестного клапана СКВ/МСУ
    bool c_xbleed;              ///< Отказ перекрестного клапана МСУ
    bool c_xbleed_sensor;       ///< Отказ датчика положения перекрестного клапана МСУ
    bool N1_0;                  ///< Отказ датчика частоты вращения ротора #0
    bool N1_1;                  ///< Отказ датчика частоты вращения ротора #1
    bool EGT_A0;                ///< Отказ датчика температуры выхлопных газов #A0
    bool EGT_A1;                ///< Отказ датчика температуры выхлопных газов #A1
    bool EGT_B0;                ///< Отказ датчика температуры выхлопных газов #B0
    bool EGT_B1;                ///< Отказ датчика температуры выхлопных газов #B1
    bool fan;                   ///< Отказ вентилятора отсека
    bool NGC;                   ///< Отказ датчика частоты вращения генератора
    bool P_fuel;                ///< Отказ датчика давления топлива
    bool asv;                   ///< Отказ клапана забора атмосферного воздуха (открыт)
    bool P2;                    ///< Отказ датчика давления за бортом
    bool T2;                    ///< Отказ датчика температуры за бортом
    bool fcv;                   ///< Отказ перекрестного клапана СКВ/МСУ (открыт на МСУ) 
    bool P_duct;                ///< Отказ датчика давления на входе в пневмосистему
    bool T_duct;                ///< Отказ датчика температуры на входе в пневмосистему
} Problem_notifications;


/* @brief Функция инициализации структуры признаков уведомления об отказах
*  
*  @param Problem_notifications* ntfs : инициализируемая структура
*/
void init_problem_notifications(Problem_notifications* ntfs);


/* @brief Проверка отказов без вывода (запись признака отказа/крит. отказа в структуру Data)
*  
*  @param Gas_generator* ggen : указатель на газогенератор
*  @param Rotor* rotor : указаетль на ротор
*  @param Compressor* comp : указаетль на компрессор
*  @param Cooling_fan* fan : указатель на вентилятор отсека
*  @param Generator* gen : указатель на генератор
*  @param Fuel_pump* pump : указатель на топливный насос
*  @param Pneumatic_system* psys : указатель на пневмосистему
*  @param Messages* msgs : указатель на сообщения от внешних систем (для проверки пожара)
*  @param Data* data : указатель на структуру данных для записи признаков отказа/крит. отказа
*/
void wellness_check(
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Cooling_fan* fan,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys,
    Messages* msgs,
    Data* data);

/* @brief Проверка отказов без вывода (запись признака отказа/крит. отказа в структуру Data)
*
*  @param Gas_generator* ggen : указатель на газогенератор
*  @param Rotor* rotor : указаетль на ротор
*  @param Compressor* comp : указаетль на компрессор
*  @param Cooling_fan* fan : указатель на вентилятор отсека
*  @param Generator* gen : указатель на генератор
*  @param Fuel_pump* pump : указатель на топливный насос
*  @param Pneumatic_system* psys : указатель на пневмосистему
*  @param int controller_id : id контроллера, выполняющего проверку
*  @param uint32_t cur_time : текущее время
*  @param Data* data : указатель на структуру данных для записи признаков отказа/крит. отказа
*  @param Problem_notifications* ntfs : структура признаков уведомления об отказах
*  @param Message_buffer* mb : буфер для вывода сообщений
*  @param File_output* fout : файл для логирования сообщений
*/
void wellness_check_verbose(
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Cooling_fan* fan,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys,
    int controller_id,
    uint32_t cur_time,
    Data* data,
    Problem_notifications* ntfs,
    Message_buffer* mb,
    File_output* fout);


/* @brief Функция сброса отказов
*   
*  @param Messages* msgs : структура сообщений от внешних систем (сброс сообщения о пожаре)
*  @param Data* data : структура информации о состоянии системы (сброс признаков отказа)
*  @param Problem_notifications* ntfs : структура признаков уведослений об отказах
*  @param Gas_generator* ggen : указатель на газогенератор
*  @param Rotor* rotor : указаетль на ротор
*  @param Cooling_fan* fan : указатель на вентилятор отсека
*  @param Generator* gen : указатель на генератор
*  @param Compressor* comp : указатель на компрессор
*  @param Fuel_pump* pump : указатель на топливный насос
*  @param Pneumatic_system* psys : указатель на пневмосистему
*/
void reset_fault(
    Messages* msgs,
    Data* data,
    Problem_notifications* ntfs,
    Gas_generator* ggen,
    Rotor* rotor,
    Cooling_fan* fan,
    Generator* gen,
    Compressor* comp,
    Fuel_pump* pump,
    Pneumatic_system* psys);