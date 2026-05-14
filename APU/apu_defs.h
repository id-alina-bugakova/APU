/* @file   apu_defs.h
*  @brief  Объявление констант и макросов
*
*  @detail Файл содержит математические и физические константы и макросы.
*
*  @author Бугакова А.А.
*/

#pragma once

// Математические
#define PI 3.1415926535897932

#define MIN(a, b) (a < b? a : b)
#define MAX(a, b) (a > b? a : b)

// Функция сравнения чисел с плавающей точкой
#define DOUBLE_EQUALS(a, b, precision) (MAX(a, b) - MIN(a, b) <= precision? 1 : 0)
// Постоянная для сравнения чисел с плавающей точкой (точность сравнения)
#define COMP_CONST 1e-12
// Постоянная для сравнения оборотов двигателя (точность сравнения для малых оборотов) 
#define N_COMP_CONST 1e2
// Постоянная для сравнения оборотов двигателя (точность сравнения для полных оборотов) 
#define N_COMP_CONST2 1.5e3

// Физические
#define OFF 0
#define ON 1

#define CLOSED 0
#define OPEN 1

// Период дискретизации = такт
#define H 0.02
// Перевод тактов во время в секундах
#define TIME(t) (t * H)

#define KELVIN 273.15

#define ATMOSPHERIC_PRES 14.7
#define ATMOSPHERIC_TEMP 15.0

// Функциональные
// Максимальная длительность запуска (выхода на заданный режим)
#define MAX_START_TIME 90.0

// Высота, выше которой только аварийный запуск
#define CRITICAL_HEIGHT 10000

// Температура перегрева выхлопных газов
#define EGT_LIMIT 600.0
// Длительность, после которой при превышении температуры возникает состояние перегрева
#define OVHEAT_TIME_LIMIT 20.0

// Максимальные обороты ротора
#define N_LIMIT 40000.0

// Максимальное время ожидания топлива
#define FUEL_TIME_LIMIT 10.0

// Предельное время подачи зажигания
#define IGNITION_TIME_LIMIT 1

// Длительность охлаждения
#define COOLDOWN_TIME 60.0

// Обороты, при которых установка считаетсся выключенной
#define SHUTDOWN_N 0.05

// Длина выводимого сообщения
#define STRING_LEN 100

