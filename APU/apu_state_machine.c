/* @file   apu_state_machine.c
*  @brief  Логика перехода между состояниям
*
*  @detail Файл содержит код функции, описывающей переход между состояниями.
*
*  @author Бугакова А.А.
* 
*  @example
*  int state_machine = STATE_OFF;
*  state_machine = handle_event(state_machine, POWER_ON, 0);
*/

#include "apu_state_machine.h"


State handle_event(State current, Event event)
{
    // Неопредленное состояние
    if (current == STATE_INDEFINITE)
        return STATE_INDEFINITE;
    // Аварийные состояния
    switch (event)
    {
    // Пожар может произойти в любом состоянии, в выключенном не обнаруживается
    case EVENT_FIRE_DETECTED:
        if (current != STATE_OFF)
            return STATE_APU_FIRE;
        break;

    // Перегрев может произойти в любом состоянии кроме выкл./idle/тест, при пожаре не важно
    case EVENT_OVHEAT_DETECTED:
        if (current != STATE_OFF && current != STATE_IDLE &&
            current != STATE_TEST && current != STATE_APU_FIRE)
            return STATE_EGT_OVER_LIMIT;
        break;

    // Превышение скорости может произойти в любом состоянии кроме выкл./idle/тест, при пожаре не
    //     важно
    case EVENT_OVSPEED_DETECTED:
        if (current != STATE_OFF && current != STATE_IDLE &&
            current != STATE_TEST && current != STATE_APU_FIRE)
            return STATE_OVSPEED;
        break;

    // Факел может погаснуть в любом запущенном состоянии и при попытке зажечь его заново, 
    //     при других аварийных состояниях не важно
    case EVENT_FLAME_WENT_OUT:
        if (current == STATE_START      ||
            current == STATE_AUTO_START || current == STATE_EMERGENCY_START   ||
            current == STATE_IDLE_RUN   || current == STATE_IDLE_RUN_LIMITED  ||
            current == STATE_MPU_START  || current == STATE_MPU_START_LIMITED ||
            current == STATE_GEN        || current == STATE_GEN_LIMITED       ||
            current == STATE_BLEED      || current == STATE_LOAD              ||
            current == STATE_RELIGHT)
            return STATE_FLAME_OUT;
        break;

    // Для повторного зажигания факела необходимо обнаружить, что он погас
    case EVENT_RELIGHT_ATTEMPT_CMD:
        if (current == STATE_FLAME_OUT)
            return STATE_RELIGHT;
        break;

    // Восстановление факела происходит после попытки повторного зажигания
    case EVENT_FLAME_RESTORED:
        if (current == STATE_RELIGHT)
            return STATE_IDLE_RUN_LIMITED;
        break;

    // Аварийное отключение происходит из аварийных состояний
    case EVENT_EMERGENCY_SHUTDOWN_CMD:
        if (current == STATE_APU_FIRE || current == STATE_EGT_OVER_LIMIT ||
            current == STATE_OVSPEED  || current == STATE_FLAME_OUT)
            return STATE_EMERGENCY_SHUTDOWN;
        break;

    // Отключение питания после аварийного отключения
    case EVENT_POWER_OFF:
        if (current == STATE_EMERGENCY_SHUTDOWN)
            return STATE_OFF;
    }

    // Нормальные и специальные режимы
    switch (current)
    {
    // В отключенном состоянии можно подать питание
    case STATE_OFF:
        if (event == EVENT_POWER_ON)
            return STATE_IDLE;
        break;

    // Ожидание команды на запуск или диагностику
    case STATE_IDLE:
        switch (event)
        {
        case EVENT_TEST_START_CMD:
            return STATE_TEST;
        case EVENT_START_CMD:
            return STATE_START;
        case EVENT_AUTO_START_CMD:
            return STATE_AUTO_START;
        case EVENT_EMERGENCY_START_CMD:
            return STATE_EMERGENCY_START;
        case EVENT_POWER_OFF:
            return STATE_OFF;
        }
        break;

    // Переход из тестового состояния в покоящееся
    case STATE_TEST:
        if (event == EVENT_TEST_COMPLETED)
            return STATE_IDLE;
        break;

    // При ручном запуске запуск может быть успешен, провалиться или быть прерван
    case STATE_START:
        switch (event)
        {
        case EVENT_START_SUCCESS:
            return STATE_IDLE_RUN;
        case EVENT_START_FAILURE:
        case EVENT_START_ABORTED:
            return STATE_IDLE;
        }
        break;

    // При автоматическом запуске запуск может быть успешен или провалиться, но не может быть
    //     прерван, т.к. это действие производится пилотом
    case STATE_AUTO_START:
        switch (event)
        {
        case EVENT_START_SUCCESS:
            return STATE_IDLE_RUN;
        case EVENT_START_FAILURE:
            return STATE_IDLE;
        }
        break;

    // При аварийном запуске запуск может быть успешен или прерван пилотом. Логика провала запуска 
    //     не применяется
    case STATE_EMERGENCY_START:
        switch (event)
        {
        case EVENT_START_SUCCESS:
            return STATE_IDLE_RUN_LIMITED;
        case EVENT_START_ABORTED:
            return STATE_IDLE;
        }
        break;

    // В состоянии нормальной работы на малых оборотах можно включить подачу воздуха в СКВ, 
    //     генератор, начать запуск МСУ или нормально выключить ВСУ
    case STATE_IDLE_RUN:
        switch (event)
        {
        case EVENT_BLEED_ON_CMD:
            return STATE_BLEED;
        case EVENT_GEN_ON_CMD:
            return STATE_GEN;
        case EVENT_MPU_ON_CMD:
            return STATE_MPU_START;
        case EVENT_NORMAL_SHUTDOWN_CMD:
            return STATE_COOLDOWN;
        }
        break;

    // В состоянии подачи воздуха можно дополнительно включить генератор, начать запуск МСУ
    //     или выключить подачу воздуха
    case STATE_BLEED:
        switch (event)
        {
        case EVENT_BLEED_OFF_CMD:
            return STATE_IDLE_RUN;
        case EVENT_GEN_ON_CMD:
            return STATE_LOAD;
        case EVENT_MPU_ON_CMD:
            return STATE_MPU_START;
        }
        break;

    // В состоянии с включенным генератором можно дополнительно включить подачу воздуха, начать
    //     запуск МСУ или выключить генератор.
    case STATE_GEN:
        switch (event)
        {
        case EVENT_BLEED_ON_CMD:
            return STATE_LOAD;
        case EVENT_GEN_OFF_CMD:
            return STATE_IDLE_RUN;
        case EVENT_MPU_ON_CMD:
            return STATE_MPU_START;
        }
        break;

    // В состоянии полной нагрузки можно начать запуск МСУ или отключить подачу воздуха или
    //     генератор
    case STATE_LOAD:
        switch (event)
        {
        case EVENT_BLEED_OFF_CMD:
            return STATE_GEN;
        case EVENT_GEN_OFF_CMD:
            return STATE_BLEED;
        case EVENT_MPU_ON_CMD:
            return STATE_MPU_START;
        }
        break;

    // Из состояния запуска МСУ можно вернуться на малые обороты
    case STATE_MPU_START:
        if (event == EVENT_MPU_OFF_CMD)
            return STATE_IDLE_RUN;
        break;

    // Из ограниченной работы на малых оборотах можно запустить генератор, начать запуск МСУ или
    //     нормально выключить ВСУ
    case STATE_IDLE_RUN_LIMITED:
        switch (event)
        {
        case EVENT_GEN_ON_CMD:
            return STATE_GEN_LIMITED;
        case EVENT_MPU_ON_CMD:
            return STATE_MPU_START_LIMITED;
        case EVENT_NORMAL_SHUTDOWN_CMD:
            return STATE_COOLDOWN;
        }
        break;

    // Из ограниченного режима работы генератора можно выключить генератор, либо начать запуск МСУ
    case STATE_GEN_LIMITED:
        switch (event)
        {
        case EVENT_GEN_OFF_CMD:
            return STATE_IDLE_RUN_LIMITED;
        case EVENT_MPU_ON_CMD:
            return STATE_MPU_START_LIMITED;
        }
        break;

    // Из состояния ограниченного запуска МСУ можно вернуться на малые обороты
    case STATE_MPU_START_LIMITED:
        if (event == EVENT_MPU_OFF_CMD)
            return STATE_IDLE_RUN_LIMITED;
        break;

    // По истечении таймера охлаждения начинается снижение оборотов
    case STATE_COOLDOWN:
        if (event == EVENT_COOLDOWN_TIMEOUT)
            return STATE_NORMAL_SHUTDOWN;
        break;
        
    // После остановки ВСУ переходит в состояние ожидания команды
    case STATE_NORMAL_SHUTDOWN:
        if (event == EVENT_APU_STOPPED)
            return STATE_IDLE;
        break;
    }

    // Если не выполнено ни одно условие перехода, остаемся в текущем состоянии
    return current;
}
