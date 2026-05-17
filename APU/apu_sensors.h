/* @file   apu_sensors.h
*  @brief  Описание датчиков
*
*  @detail Файл содержит информацию о датчиках, структуры цифрового и дискретного датчиков  
*  и функции инициализации датчиков.
*
*  @author Бугакова А.А.
*/


#pragma once


#define SENSOR_BUFFER_SIZE 2


#include <stdbool.h>
#include <stdint.h>


/* @brief  Единицы измерения
*/
typedef enum 
{
    UNIT_PSI, 
    UNIT_DEG_C,     
    UNIT_RPM
} Unit;

/* @brief  Типы датчиков
* 
*  В зависимости от типа датчика инициализируются его параметры
*/
typedef enum
{
    /* @brief  Датчик давления топлива
    *
    *  Давление топлива после насоса высокого давления; 0 - низкое, 1 - нормальное
    */
    SENSOR_TYPE_P_FUEL,

    /* @brief  Датчик атмосферного давления
    *
    *  Давление за бортом; 0.5 .. 32 psi
    */
    SENSOR_TYPE_P2,

    /* @brief  Датчик давления после компрессора
    *
    *  Давление после компрессора, перед камерой сгорания; 0 .. 600 psi
    */
    SENSOR_TYPE_P3,

    /* @brief  Датчик давления воздуха, подаваемого в пневмосистему
    *
    *  0 .. 150 psi
    */
    SENSOR_TYPE_P_DUCT,

    /* @brief  Датчик температуры наружного воздуха
    *
    *  Температура за бортом, -80 .. 80 °С
    */
    SENSOR_TYPE_T2,

    /* @brief  Датчик температуры после компрессора
    *
    *  Температура после компрессора, перед камерой сгорания; 0 .. 700 °С
    */
    SENSOR_TYPE_T3,

    /* @brief  Датчик температуры выхлопных газов
    *
    *  Exhaust gas temperature; 0 .. 1100 °С
    */
    SENSOR_TYPE_EGT,

    /* @brief  Датчик температуры воздуха, подаваемого в пневмосистему
    *
    *  -50 .. 300 °С
    */
    SENSOR_TYPE_T_DUCT,

    /* @brief  Датчик частоты вращения ротора (газогенератора)
    *
    *  100% соответствует 40000 оборотов в минуту; 0 .. 48000 rpm (0 .. 120%)
    */
    SENSOR_TYPE_N1,

    /* @brief  Датчик частоты вращения генератора
    *
    *  100% соответствует 12000 оборотов в минуту (стабильно); 0 .. 14400 rpm (0 .. 120%)
    */
    SENSOR_TYPE_NGC,

    /* @brief  Датчик клапана отсечки топлива
    *
    *  Fuel shutoff valve; дискретный, 0 - закрыт
    */
    SENSOR_TYPE_FUEL_SOV,

    /* @brief  Датчик клапана отбора атмосферного воздуха
    *
    *  Atmospheric shutoff valve; дискретный, 0 - закрыт
    */
    SENSOR_TYPE_ASV,

    /* @brief  Датчик входного клапана пневмосистемы
    *
    *  Bleed shutoff valve; дискретный, 0 - закрыт
    */
    SENSOR_TYPE_BSV,

    /* @brief  Датчик перекрестного клапана СКВ / МСУ
    *
    *  Flow control valve; дискретный, 0 - МСУ, 1 - СКВ
    */
    SENSOR_TYPE_FCV,

    /* @brief  Датчик перекрестного клапана МСУ
    *
    *  Дискретный, 0 - левый двигатель, 1 - правый
    */
    SENSOR_TYPE_MPU_XBLEED,

    /* @brief  Датчик пламени в камере сгорания
    *
    *  Дискретный, 0 - нет факела
    */
    SENSOR_TYPE_FLAME
} Sensor_type;


/* @brief  Конфигурация цифрового датчика
* 
*  Хранит предельные значения и временные параметры одного цифрового датчика
*/
typedef struct
{
    Sensor_type type;                     ///< Псевдоним датчика на борту
    uint8_t id;                           ///< Порядковый номер датчика (при резервировании)
    double min_value;                     ///< Минимальное измеряемое значение
    double max_value;                     ///< Максимальное измеряемое значение
    Unit unit;                            ///< Единицы измерения датчика
    bool fault;                           ///< Признак отказа датчика (0 - норма, 1 - отказ)
    bool power;                           ///< Датчик подключен к электропитанию
    double buffer[SENSOR_BUFFER_SIZE];    ///< Буфер задержки
    double value;                         ///< Числовое значение датчика
} Digital_sensor;

/* @brief  Конфигурация дискретного датчика
*
*  Хранит предельные значения и временные параметры одного дискретного датчика
*/
typedef struct
{
    Sensor_type type;                     ///< Псевдоним датчика на борту
    uint8_t id;                           ///< Порядковый номер датчика (при резервировании)
    bool fault;                           ///< Признак отказа датчика (0 - норма, 1 - отказ)
    bool power;                           ///< Датчик подключен к электропитанию
    bool buffer[SENSOR_BUFFER_SIZE];      ///< Буфер задержки
    bool value;                           ///< Значение датчика (однобитное)
} Discrete_sensor;


/* @brief Функция инициализации цифровых датчиков
*
*  @param Digital_sensor* sensor : экземпляр сенсора, который будет инициализирован
*  @param Sensor_type type : тип сенсора
*  @param uint8_t id : идентификатор датчика в резерве (0 для датчиков без резервирования)
*/
void init_digital_sensor(Digital_sensor* sensor, Sensor_type type, uint8_t id);

/* @brief Функция инициализации дискретных датчиков
*
*  @param Discrete_sensor* sensor : экземпляр сенсора, который будет инициализирован
*  @param Sensor_type type : тип сенсора
*  @param uint8_t id : идентификатор датчика в резерве (0 для датчиков без резервирования)
*/
void init_discrete_sensor(Discrete_sensor* sensor, Sensor_type type, uint8_t id);


/* @brief Функция обновления цифрового датчика
* 
*  @param Digital_sensor* sensor : экземпляр сенсора, показания которого перезаписываем
*  @param double value : новое показание датчика (отправляется в буфер)
*/
void update_digital_sensor(Digital_sensor* sensor, double value);

/* @brief Функция обновления датчика температуры за бортом
*
*  @param Digital_sensor* T2 : экземпляр сенсора, показания которого перезаписываем
*  @param int height : высота в метрах
*/
void update_T2(Digital_sensor* T2, int height);

/* @brief Функция обновления датчка давления за бортом
*
*  @param Digital_sensor* P2 : экземпляр сенсора, показания которого перезаписываем
*  @param int height : высота в метрах
*/
void update_P2(Digital_sensor* P2, int height);

/* @brief Функция обновления дискретного датчика
*
*  @param Discrete_sensor* sensor : экземпляр сенсора, показания которого перезаписываем
*  @param bool value : новое показание датчика (отправляется в буфер)
*/
void update_discrete_sensor(Discrete_sensor* sensor, bool value);