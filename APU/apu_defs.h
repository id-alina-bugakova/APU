/* @file   apu_defs.h
*  @brief  Объявление констант и макросов
*
*  @detail Файл содержит математические и физические константы и макросы.
*
*  @author Бугакова А.А.
*/

#pragma once

// Mathematical
#define PI 3.1415926535897932

#define MIN(a, b) (a < b? a : b)
#define MAX(a, b) (a > b? a : b)

// Physical
#define OFF 0
#define ON 1

#define CLOSED 0
#define OPEN 1

#define H 0.02

#define TIME(t) (t * H)

#define KELVIN 273

#define ATMOSPHERIC_PRES 14.7
#define ATMOSPHERIC_TEMP 15

