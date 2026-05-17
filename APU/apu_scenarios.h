/* @file   apu_scenarios.h
*  @brief  Шаблоны функций работы со сценариями
*
*  @detail Файл содержит функции создания и чтения сценариев, а также обработчик тактов.
*
*  @author Бугакова А.А.
*/


#pragma once


#define N_SCENARIOS 9   ///< Число сценариев


#include <stdbool.h>
#include "apu_communication.h"
#include "apu_controller.h"
#include "apu_defs.h"


/* @brief Стурктура файлового ввода сценариев
* 
*  Структура содержит относительные пути к сценариям, выбранный сцеарий и положение курсора в файле
*/
typedef struct {
    char path[N_SCENARIOS][STRING_LEN];     ///< Относительные пути к сценариям
    int n;                                  ///< Номер выбранного сценария
    long pos;                               ///< Положение курсора в читаемом файле
} Fin_scenario;

/* @brief Структура файлового вывода сценариев
* 
*  Структура содержит путь к записываемому сценарию и признак первой записи
*/
typedef struct
{
    bool first_record;                      ///< Признак первой записи в файл
    char path[STRING_LEN];                  ///< Относительный путь к файлу
} Fout_scenario;


/* @brief Функция инициализации структуры файлового ввода сценариев
* 
*  @param Fin_scenario* fis : инициализируемая структура
*/
void init_fin_scenario(Fin_scenario* fis);

/* @brief Функция инициализации структуры файлового вывода сценариев
*
*  @param Fout_scenario* fos : инициализируемая структура
*/
void init_fout_scenario(Fout_scenario* fos);


/* @brief Функция записи сценария в файл
* 
*  @param Fout_scenario* fos : структура файлового вывода сценариев
*  @param Physical* phys : структура физичсеких событий
*  @param Actions_manual* actm : структура ручных действий 
*  @param bool test : признак проведения диагностики
*  @param bool c0_fault : признак отказа первого канала контроллера
*  @param bool c1_fault : признак отказа второго канала контроллера
*  @param char fault : буквенный код назначенной неисправности
*/
void write_scenario(
    Fout_scenario* fos, 
    Physical* phys, 
    Actions_manual* actm, 
    bool test,
    bool c0_fault, 
    bool c1_fault, 
    char fault);

/* @brief Функция выполнения шага сценария
* 
*  @param Fin_scenario* scns : структура файлового ввода сценариев
*  @param APU* apu : структура агрегатов ВСУ
*  @param Actions_manual* actm : структура ручных действий 
*  @param Physical* phys : структура физичсеких событий
*  @param ECU* c0 : первый канал контроллера
*  @param ECU* c1 : второй канал контроллера
*/
void run_scenario(
    Fin_scenario* scns,
    APU* apu,
    Actions_manual* actm,
    Physical* phys,
    ECU* c0,
    ECU* c1);