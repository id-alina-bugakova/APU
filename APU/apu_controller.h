/* @file   apu_controller.h
*  @brief  Объявление контроллера и связанных функций 
*
*  @detail Файл содержит структуру каналов контролера, функции его инициализации и обновления, 
*  а также функции, используемые в функции обновления.
*
*  @author Бугакова А.А.
*/

#pragma once

#include "apu_components.h"
#include "apu_communication.h"
#include "apu_interface.h"
#include "apu_diagnostic.h"

#include "stdbool.h"

/* @brief Структура канала контроллера
* 
*  Структура содержит id контроллера, признак подачи питания, признак работы на главном канале,
*  признак отказа, время последнего обновления и время, по истечении которого контроллер считается
*  "упавшим".
*/
typedef struct {
    int id;                             ///< Идентификатор контроллера
    bool power;                         ///< Признак питания (0 - нет питания, 1 - есть)
    bool main_channel;                  ///< Признак работы на главном канале (0 - резерв)
    bool fault;                         ///< Признак неисправности (0 - исправен, 1 - неисправен)
    unsigned long long last_updated;    ///< Время последнего обновления (в тактах)
    unsigned long long tolerance;       ///< Время, после которого контроллер считается "упавшим"
} ECU;


/* @brief Функция инициализации контроллера
* 
*  @param ECU* contr : инициализируемый контроллер
*  @param int id : уникальный идентификатор контроллера
*  @param bool main_channel : признак назначения контроллера на главный канал
*/
void init_controller(ECU* contr, int id, bool main_channel);

/* @brief Вычисление EGT на основе данных со всех датчиков EGT 
* 
*  @param Rotor* rotor : ротор, с датчиков которого снимаем показания 
*/
double calculate_EGT(Rotor* rotor);

/* @brief Вычисление оборотов на основе данных со всех датчиков частоты вращения  ротора
*
*  @param Rotor* rotor : ротор, с датчиков которого снимаем показания
*/
double calculate_N(Rotor* rotor);

/* @brief Проверка перегрева или превышения оборотов ротором
*
*  @param uint32_t cur_time : текущее время для определения длительности состояния
*  @param Rotor* rotor : указатель на ротор
*  @param Responses* rsps : структура ответа, в которую записываем показатели ротора
*  @param Data* data : структура, в которую сохраняем данные о состоянии ротора
*/
void check_N_EGT(uint32_t cur_time, Rotor* rotor, Responses* rsps, Data* data);

/* @brief Проверка наличия аварийных состояний (нормальный запуск)
* 
*  @param ECU* c1 : указатель на проверяющий канал контроллера
*  @param uint32_t cur_time : текущее время для определения длительности состояния
*  @param Event* last_event : указатель на последнее событие для записи
*  @param Rotor* rotor : указатель на ротор
*  @param Gas_generator* ggen : указатель на газогенератор
*  @param Responses* rsps : структура ответа (для отсечки топлива)
*  @param Data* data : структура, в которую сохраняем данные о состоянии
*/
void check_for_emergencies(
    ECU* c1, 
    uint32_t cur_time, 
    Event* last_event, 
    Rotor* rotor,
    Gas_generator* ggen,
    Responses* rsps, 
    Data* data,
    Message_buffer* mb,
    File_output* fout);

/* @brief Проверка наличия аварийных состояний (аварийный запуск)
*
*  @param ECU* c1 : указатель на проверяющий канал контроллера
*  @param uint32_t cur_time : текущее время для определения длительности состояния
*  @param Event* last_event : указатель на последнее событие для записи
*  @param Rotor* rotor : указатель на ротор
*  @param Gas_generator* ggen : указатель на газогенератор
*  @param Responses* rsps : структура ответа (для отсечки топлива)
*  @param Data* data : структура, в которую сохраняем данные о состоянии
*/
void check_for_emergencies_emergent(
    ECU* c1,
    uint32_t cur_time,
    Event* last_event,
    Rotor* rotor,
    Gas_generator* ggen,
    Messages* msgs,
    Responses* rsps,
    Data* data,
    Message_buffer* mb,
    File_output* fout);

/* @brief Функция обновления канала контроллера
* 
*  @param ECU* c1 : текущий канал контроллера
*  @param ECU* c2 : другой канал контроллера
*  @param uint32_t cur_time : текущее время в тактах
*  @param State state : текущее состояние
*  @param Starter* start : указатель на электростартер
*  @param Gas_generator* ggen : указатель на газогенератор
*  @param Rotor* rotor : указатель на ротор
*  @param Compressor* comp : указатель на компрессор
*  @param Cooling_fan* fan : указатель на вентилятор отсека
*  @param Generator* gen : указатель на генератор
*  @param Fuel_pump* pump : указатель на топливный насос
*  @param Pneumatic_system* psys : указатель на пневмосистему
*  @param Messages* msgs : структура с сообщениями от внешних систем
*  @param Responses* rsps : структура для ответа внешним системам
*  @param Actions* acts : структра с действиями, которые производятся после обновления контроллера
*  @param Data* data : общая структура для записи данных контроллерами
*  @param Physical* phys : структура с физическими параметрами
*/
void update_controller(
    ECU* c1,
    ECU* c2,
    uint32_t cur_time,
    State* state,
    Starter* start,
    Gas_generator* ggen,
    Rotor* rotor,
    Compressor* comp,
    Cooling_fan* fan,
    Generator* gen,
    Fuel_pump* pump,
    Pneumatic_system* psys,
    Messages* msgs,
    Responses* rsps,
    Actions* acts,
    Data* data,
    Physical* phys,
    Problem_notifications* ntfs,
    Message_buffer* mb,
    File_output* fout);