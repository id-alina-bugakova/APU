/* @file   apu_sensors.c
*  @brief  Инициализация датчиков
*
*  @detail Файл содержит код функций, задающих начальные параметры датчикам в зависимости от типа.
*
*  @author Бугакова А.А.
*/

#include "apu_defs.h"
#include "apu_math.h"
#include "apu_sensors.h"

void init_digital_sensor(Digital_sensor* sensor, Sensor_type type, uint8_t id)
{
	sensor->type  = type;
	sensor->id    = id;
	sensor->fault = 0;
	sensor->power = OFF;
	switch (type)
	{
	case SENSOR_TYPE_P2:
		sensor->min_value = 0.5;
		sensor->max_value = 32;
		sensor->unit      = UNIT_PSI;
		sensor->value	  = ATMOSPHERIC_PRES;
		break;

	case SENSOR_TYPE_P3:
		sensor->min_value = 0;
		sensor->max_value = 600;
		sensor->unit      = UNIT_PSI;
		sensor->value	  = ATMOSPHERIC_PRES;
		break;

	case SENSOR_TYPE_P_DUCT:
		sensor->min_value = 0;
		sensor->max_value = 150;
		sensor->unit      = UNIT_PSI;
		sensor->value	  = ATMOSPHERIC_PRES;
		break;

	case SENSOR_TYPE_T2:
		sensor->min_value = -80;
		sensor->max_value = 80;
		sensor->unit      = UNIT_DEG_C;
		sensor->value	  = ATMOSPHERIC_TEMP;
		break;

	case SENSOR_TYPE_T3:
		sensor->min_value = 0;
		sensor->max_value = 700;
		sensor->unit      = UNIT_DEG_C;
		sensor->value	  = ATMOSPHERIC_TEMP;
		break;

	case SENSOR_TYPE_EGT:
		sensor->min_value = 0;
		sensor->max_value = 1100;
		sensor->unit      = UNIT_DEG_C;
		sensor->value	  = ATMOSPHERIC_TEMP;
		break;

	case SENSOR_TYPE_T_DUCT:
		sensor->min_value = -50;
		sensor->max_value = 300;
		sensor->unit      = UNIT_DEG_C;
		sensor->value	  = ATMOSPHERIC_TEMP;
		break;

	case SENSOR_TYPE_N1:
		sensor->min_value = 0;
		sensor->max_value = 48000;
		sensor->unit      = UNIT_RPM;
		sensor->value	  = 0;
		break;

	case SENSOR_TYPE_NGC:
		sensor->min_value = 0;
		sensor->max_value = 14400;
		sensor->unit      = UNIT_RPM;
		sensor->value	  = 0;
		break;
	}

	for (int i = 0; i < SENSOR_BUFFER_SIZE; i++)
		sensor->buffer[i] = 0;
}

void init_discrete_sensor(Discrete_sensor* sensor, Sensor_type type, uint8_t id)
{
	sensor->type  = type;
	sensor->id    = id;
	sensor->fault = 0;
	sensor->power = OFF;
	sensor->value = 0;
}

void update_digital_sensor(Digital_sensor* sensor, double value)
{
	sensor->value = sensor->buffer[0];
	for (int i = 1; i < SENSOR_BUFFER_SIZE; i++)
		sensor->buffer[i - 1] = sensor->buffer[i];
}

void update_T2(Digital_sensor* T2, int height)
{
	update_digital_sensor(T2, ATMOSPHERIC_TEMP - 0.65 * height / 100);
}

void update_P2(Digital_sensor* T2, int height)
{
	update_digital_sensor(T2, ATMOSPHERIC_PRES * binpow(1 - 0.0065 * height / 288.15, 5));
}

void update_discrete_sensor(Discrete_sensor* sensor, bool value)
{
	sensor->value = sensor->buffer[0];
	for (int i = 1; i < SENSOR_BUFFER_SIZE; i++)
		sensor->buffer[i - 1] = sensor->buffer[i];
}
