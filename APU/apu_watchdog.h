/* @file   apu_watchdog.h
*  @brief  Структура и функции сторожевого таймера
*
*  @detail Файл содержит сторожевого таймера и функции его инициализации и обновления.
*
*  @author Бугакова А.А.
*/


#pragma once


#include <stdint.h>
#include <stdbool.h>
#include "apu_components.h"
#include "apu_controller.h"
#include "apu_interface.h"


/* @brief Структура сторожевого таймера
* 
*  Сторожевой таймер отслеживает состояние контроллеров и производит остановку установки в случае
*  потери контроля.
*/
typedef struct {
    uint32_t tolerance;         ///< Время в тактах, после которого таймер считает контроллер "упавшим"
    bool initiated_shutdown;    ///< Признак инициирования таймером аварийной остановки
} Watchdog;


/* @brief Функция инициализации сторожевого таймера
* 
*  @param Watchdog* wd : указатель на инициализируемый таймер
*/
void init_watchdog(Watchdog* wd);

/* @brief Функция обновления сторожевого таймера (1 раз в такт)
* 
*  @param Watchdog* wd : обновляемый таймер
*  @param uint32_t cur_time : текущее время
*  @param ECU* c0 : первый канал контроллера
*  @param ECU* c1 : второй канал контроллера
*  @param Actions* acts : структура автоматических действий
*  @param Message_buffer* mb : буфер вывода сообщений
*  @param File_output* fout : файл логирования сообщений
*/
void update_watchdog(
    Watchdog* wd,
    uint32_t cur_time,
    ECU* c0,
    ECU* c1,
    Actions* acts,
    Message_buffer* mb,
    File_output* fout);