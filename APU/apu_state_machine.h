/* @file   apu_state_machine.h
*  @brief  Описание конечного автомата
* 
*  @detail Файл содержит описания состояний и событий, вызывающих переходы между состояниями
*  конечного автомата, наборы названий этих состояний и событий в текстовом формате
*  и функцию перехода между состояниями.
* 
*  @author Бугакова А.А.
*/

#pragma once

///< Названия состояний
#define APU_STATES                    \
    {                                 \
        "OFF",                        \
        "IDLE",                       \
        "TEST",                       \
        "START",                      \
        "AUTO START",                 \
        "EMERGENCY START",            \
        "IDLE RUN",                   \
        "BLEED",                      \
        "GENERATOR",                  \
        "BLEED + GENERATOR",          \
        "MPU START",                  \
        "IDLE RUN [LIMITED]",         \
        "GENERATOR [LIMITED]",        \
        "MPU START [LIMITED]",        \
        "COOLDOWN",                   \
        "NORMAL SHUTDOWN",            \
        "APU FIRE",                   \
        "OVERHEAT",                   \
        "OVERSPEED",                  \
        "FLAME OUT",                  \
        "RELIGHT",                    \
        "EMERGENCY SHUTDOWN",         \
        "INDEFINITE"                  \
    }

///< Названия событий
#define APU_EVENTS                    \
    {                                 \
        "POWER ON",                   \
        "TEST START COMMAND",         \
        "TEST COMPLETED",             \
        "START COMMAND",              \
        "AUTO START COMMAND",         \
        "EMERGENCY START COMMAND",    \
        "START SUCCESS",              \
        "START FAILURE",              \
        "START ABORTED",              \
        "BLEED ON COMMAND",           \
        "GENERATOR ON COMMAND",       \
        "MPU ON COMMAND",             \
        "BLEED OFF COMMAND",          \
        "GENERATOR OFF COMMAND",      \
        "MPU OFF COMMAND",            \
        "NORMAL SHUTDOWN COMMAND",    \
        "COOLDOWN TIMEOUT",           \
        "APU STOPPED",                \
        "POWER OFF",                  \
        "FIRE DETECTED",              \
        "OVERHEAT DETECTED",          \
        "OVERSPEED DETECTED",         \
        "FLAME WENT OUT",             \
        "RELIGHT ATTEMPT COMMAND",    \
        "FLAME_RESTORED",             \
        "EMERGENCY SHUTDOWN COMMAND", \
        "NONE"                        \
    }

/* @brief  Состояния конечного автомата
*/
typedef enum {
    ///< Нормальные и специальные режимы
    STATE_OFF,                      ///< ВСУ выключена

    STATE_IDLE,                     ///< Ожидание команды запуска

    STATE_TEST,                     ///< Автоматическая диагностика системы

    /* @brief  Ручное включение ВСУ
    * 
    *  Включает запуск при помощи электростартера, зажигание, открытие клапанов, включение 
    *  агрегатов
    */
    STATE_START,

    /* @brief  Автоматическое включение в воздухе
    *
    *  Выполняется при отказе бортовой сети
    */
    STATE_AUTO_START,

    /* @brief  Аварийный запуск
    *
    *  Выполняется вручную в обход ВСЕХ мер безопасности (пожар, большая высота); ограниченные
    *  обороты + запрет отбора воздуха
    */
    STATE_EMERGENCY_START,

    STATE_IDLE_RUN,                 ///< Работа на малых оборотах (без нагрузки)

    STATE_BLEED,                    ///< Работа под нагрузкой (СКВ)

    STATE_GEN,                      ///< Работа под нагрузкой (генератор)

    STATE_LOAD,                     ///< Работа под нагрузкой (СКВ + генератор)

    STATE_MPU_START,                ///< Работа на максимальных оборотах (для запуска МСУ)

    STATE_IDLE_RUN_LIMITED,         ///< Аварийная работа на малых оборотах

    STATE_GEN_LIMITED,              ///< Аварийная работа на генератор ВСУ

    STATE_MPU_START_LIMITED,        ///< Аварийная работа на запуск МСУ

    STATE_COOLDOWN,                 ///< Охлаждение: в течение 2 минут без нагрузки

    STATE_NORMAL_SHUTDOWN,          ///< Снижение оборотов. Отключение при оборотах N < 5%

    ///< Аварийные состояния

    /* @brief  Пожар ВСУ
    *
    *  Перекрытие клапанов, выключение ВСУ и запрет на повторное включение
    */
    STATE_APU_FIRE,  

    STATE_EGT_OVER_LIMIT,           ///< Перегрев турбины: остановка, отсечка топлива

    STATE_OVSPEED,                  ///< Разнос ротора: остановка, отсечка топлива

    STATE_FLAME_OUT,                ///< Потеря пламени: попытка повторного зажигания до 2 раз

    STATE_RELIGHT,                  ///< Аварийное состояние - попытка повторного зажигания

    STATE_EMERGENCY_SHUTDOWN,       ///< Аварийное отключение: быстрая остановка ВСУ

    STATE_INDEFINITE                ///< Неопредленное состояние для алгоритма
} State;

/* @brief  События, приводящие к смене состояний конечного автомата
*/
typedef enum {
    EVENT_POWER_ON,                 ///< Подача питания ВСУ
    EVENT_TEST_START_CMD,           ///< Начало диагностики
    EVENT_TEST_COMPLETED,           ///< Диагностика завершена
    EVENT_START_CMD,                ///< Получена команда ручного запуска
    EVENT_AUTO_START_CMD,           ///< Получена команда автоматического запуска
    EVENT_EMERGENCY_START_CMD,      ///< Получена команда аварийного запуска
    EVENT_START_SUCCESS,            ///< Успешный запуск
    EVENT_START_FAILURE,            ///< Неудачный запуск
    EVENT_START_ABORTED,            ///< Запуск прерван
    EVENT_BLEED_ON_CMD,             ///< Получена команда включения отбора воздуха в СКВ
    EVENT_GEN_ON_CMD,               ///< Получена команда включения генератора
    EVENT_MPU_ON_CMD,               ///< Получена команда запуска МСУ
    EVENT_BLEED_OFF_CMD,            ///< Получена команда выключения отбора воздуха в СКВ
    EVENT_GEN_OFF_CMD,              ///< Получена команда выключения генератора
    EVENT_MPU_OFF_CMD,              ///< Получена команда завершения запуска МСУ
    EVENT_NORMAL_SHUTDOWN_CMD,      ///< Получена команда выключения
    EVENT_COOLDOWN_TIMEOUT,         ///< Таймер охлаждения истек
    EVENT_APU_STOPPED,              ///< ВСУ остановлена (менее 5% оборотов)
    EVENT_POWER_OFF,                ///< Отключено питание
    EVENT_FIRE_DETECTED,            ///< Обнаружен пожар в отсеке ВСУ
    EVENT_OVHEAT_DETECTED,          ///< Произошел перегрев
    EVENT_OVSPEED_DETECTED,         ///< Произошло превышение скорости вращения двигателя
    EVENT_FLAME_WENT_OUT,           ///< Пропал факел в камере сгорания
    EVENT_RELIGHT_ATTEMPT_CMD,      ///< Получена команда произвести попытку повторного зажигания
    EVENT_FLAME_RESTORED,           ///< После повторного зажигания пламя появилось
    EVENT_EMERGENCY_SHUTDOWN_CMD,   ///< Получена команда аварийного выключения
    EVENT_NONE                      ///< Пустое событие
} Event;


/* @brief  Функция перехода между состояниями конечного автомата на основе событий
*  
*  @param  State current : текущее состояние автомата
*  @param  Event event : обрабатываемое событие
*  @return Новое остояние конечного автомата
*/
State handle_event(State current, Event event);