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

#define DOUBLE_EQUALS(a, b, precision) (MAX(a, b) - MIN(a, b) <= precision? 1 : 0)
#define COMP_CONST 1e-12
#define N_COMP_CONST 1e2
#define N_COMP_CONST2 1.5e3

// Физические
#define OFF 0
#define ON 1

#define CLOSED 0
#define OPEN 1

#define H 0.02

#define TIME(t) (t * H)

#define KELVIN 273.15

#define ATMOSPHERIC_PRES 14.7
#define ATMOSPHERIC_TEMP 15.0

// Функциональные
#define MAX_START_TIME 90.0

#define CRITICAL_HEIGHT 10000

#define EGT_LIMIT 600.0
#define OVHEAT_TIME_LIMIT 20.0

#define N_LIMIT 40000.0

#define FUEL_TIME_LIMIT 10.0

#define COOLDOWN_TIME 150.0

#define SHUTDOWN_N 0.05

