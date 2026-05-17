/* @file   apu_outer.h
*  @brief  Объявление функций имитации внешних систем
*
*  @detail Файл содержит шаблоны функций, имитирующих поведение внешних систем путем формирования 
*  сообщений от них.
*
*  @author Бугакова А.А.
*/


#pragma once


#include "apu_communication.h"
#include "apu_components.h"


/* @brief Получение физичсеких событий
* 
*  @param Physical* phys : записываемая структура
*  @param bool auto_start : признак автоматического запуска
*  @param int height : высота полета
*  @param bool ignited : признак зажигания (погаснет ли факел в камере сгорания)
*  @param bool enough_pressure : признак достаточности давления для запуска двигателей
*/
void get_physical(Physical* phys, bool auto_start, int height, bool ignited, bool enough_pressure);


/* @brief Функция имитации системы электроснабжения
* 
*  @param EPS_message* msg : структура сообещния от СЭС
*  @param EPS_response* rsp : структура ответа СЭС
*  @param Generator* gen : указатель на генератор
*  @param bool first_interaction : признак первого взаимодействия с системой
*  @param bool ext_power : признак наземного питания
*  @param bool gen_demand : признак запроса на генератор
*/
void by_eps(
    EPS_message* msg,
    EPS_response* rsp,
    Generator* gen,
    bool first_interaction,
    bool ext_power,
    bool gen_demand);

/* @brief Функция имитации топливной системы
* 
*  @param FS_message* msg : структура сообщения от ТС
*  @param bool first_interaction : признак первого взаимодействия с системой
*  @param bool low_pressure : признак низкого давления топлива
*/
void by_fs(FS_message* msg, bool first_interaction, bool low_pressure);

/* @brief Функция имитации системы кондиционирования воздуха
* 
*  @param AirCS_message* msg : структура сообщения от СКВ
*  @param AirCS_response* rsp : структура ответа СКВ 
*  @param bool first_interaction : признак первого взаимодействия с системой
*  @param bool bleed_demand : признак запроса на подачу воздуха
*  @param bool high_pressure : признак высокого давления в пневмосети
*/
void by_air_cs(
    AirCS_message* msg,
    AirCS_response* rsp,
    bool first_interaction,
    bool bleed_demand,
    bool high_pressure);

/* @brief Функция имитации маршевой силовой установки
*
*  @param MPU_message* msg : структура сообщения от МСУ
*  @param MPU_response* rsp : структура ответа МСУ
*  @param bool first_interaction : признак первого взаимодействия с системой
*  @param bool bleed_demand : признак запроса на подачу воздуха
*  @param bool engine : признак запроса воздуха на левый/правый двигатель
*/
void by_mpu(
    MPU_message* msg,
    MPU_response* rsp,
    bool first_interaction,
    bool bleed_demand,
    bool engine);

/* @brief Функция имитации системы пожарной защиты
* 
*  @param FPS_message* msg : структура сообщения от СПЗ
*  @param bool fire : признак наличия пожара
*/
void by_fps(FPS_message* msg, bool fire);

/* @brief Функция имитации системы дистанционного управления
* 
*  @param RCS_message* msg : структра сообщения от СДУ
*  @param RCS_response* rsp : структура ответа СДУ
*  @param State state : текущее состояние
*  @param bool first_interaction : признак первого взаимодействия с системой
*  @param bool manual : признак ручного управления
*  @param bool power : признак питания установки
*  @param bool test : признак задачи на тестирование
*/
void by_rcs(
    RCS_message* msg,
    RCS_response* rsp,
    State state,
    bool first_interaction,
    bool manual,
    bool power,
    bool test);

/* @brief Функция имитации системы автоматического управления
*
*  @param AutoCS_message* msg : структура сообщения от САУ
*  @param bool auto_start : признак автоматичсекого запуска
*/
void by_auto_cs(AutoCS_message* msg, bool auto_start);