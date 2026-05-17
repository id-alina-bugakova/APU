/* @file   apu_driver.h
*  @brief  Объявление функций выполнения действий с системой
*
*  @detail Файл содержит объявления функций выполнения ручных и автоматичсеких действий
*
*  @author Бугакова А.А.
*/


#pragma once


#include "apu_communication.h"
#include "apu_components.h"
#include "apu_controller.h"
#include "apu_defs.h"


/* @brief Функция выполнения ручных действий и применения физических воздействий
* 
*  @param Actions_manual* actm : структура ручных действий
*  @param Physical* phys : структура физических воздействий
*  @param Messages* msgs : структура сообщений (принимает результат воздействия)
*  @param Data* data : структура состояний системы (для сброса при выключении)
*  @param Problem_notifications* ntfs : структура признаков уведосления об отказах 
*  @param State state : текущее состояние
*  @param ECU* c0 : указатель на первый канал контроллера
*  @param ECU* c1 : указатель на второй канал контроллера
*  @param Starter* start : указатель на стратер
*  @param Gas_generator* ggen : указатель на газогенератор
*  @param Rotor* rotor : указатель на ротор
*  @param Compressor* comp : указатель на компрессор
*  @param Cooling_fan* fan : указатель на вентилятор отсека
*  @param Generator* gen : указатель на генератор
*  @param Fuel_pump* pump : указатель на топливный насос
*  @param Pneumatic_system* psys : указатель на пневмосистему
*/
void perform_manual_actions(
    Actions_manual* actm,
    Physical* phys,
    Messages* msgs,
    Data* data,
    Problem_notifications* ntfs,
    State state,
    ECU* c0,
    ECU* c1,
    Starter* start,
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Cooling_fan* fan,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys);

/* @brief Функция выполнения ручных действий и применения физических воздействий
*
*  @param Actions* acts : структура автоматичсеких действий
*  @param Messages* msgs : структура сообщений (принимает результат воздействия)
*  @param Data* data : структура состояний системы 
*  @param Actions_manual* actm : структура ручных действий
*  @param Physical* phys : структура физических воздействий
*  @param State state : текущее состояние
*  @param ECU* c0 : указатель на первый канал контроллера
*  @param ECU* c1 : указатель на второй канал контроллера
*  @param Starter* start : указатель на стратер
*  @param PID* pid
*  @param Gas_generator* ggen : указатель на газогенератор
*  @param Rotor* rotor : указатель на ротор
*  @param Compressor* comp : указатель на компрессор
*  @param Generator* gen : указатель на генератор
*  @param Fuel_pump* pump : указатель на топливный насос
*  @param Pneumatic_system* psys : указатель на пневмосистему
*/
void perform_actions(
    Actions* acts,
    Messages* msgs,
    Data* data,
    Actions_manual* actm,
    Physical* phys,
    State* state,
    ECU* c0,
    ECU* c1,
    Starter* start,
    PID* pid,
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys);