/* @file   apu_sensors.c
*  @brief  Инициализация датчиков
*
*  @detail Файл содержит код функций, задающих начальные параметры датчикам в зависимости от типа.
*
*  @author Бугакова А.А.
*
*  @example
*/

#include "apu_sensors.h"

void init_digital_sensor(Digital_sensor* sensor, Sensor_type type, uint8_t id)
{
	switch (type)
	{
	case SENSOR_TYPE_P_FUEL:
		sensor->type        = type;
		sensor->id          = id;
		sensor->min_value   = 0;
		sensor->max_value   = 5000;
		sensor->value       = 0;
		sensor->unit        = UNIT_PSI;
		sensor->has_power   = 0;
		sensor->last_update = 0;
		sensor->last_read   = 0;
		break;

	case SENSOR_TYPE_P2:
		sensor->type        = type;
		sensor->id          = id;
		sensor->min_value   = 0.5;
		sensor->max_value   = 32;
		sensor->value       = 0.5;
		sensor->unit        = UNIT_PSI;
		sensor->has_power   = 0;
		sensor->last_update = 0;
		sensor->last_read   = 0;
		break;

	case SENSOR_TYPE_P3:
		sensor->type        = type;
		sensor->id          = id;
		sensor->min_value   = 0;
		sensor->max_value   = 600;
		sensor->value       = 0;
		sensor->unit        = UNIT_PSI;
		sensor->has_power   = 0;
		sensor->last_update = 0;
		sensor->last_read   = 0;
		break;

	case SENSOR_TYPE_P_DUCT:
		sensor->type        = type;
		sensor->id          = id;
		sensor->min_value   = 0;
		sensor->max_value   = 150;
		sensor->value       = 0;
		sensor->unit        = UNIT_PSI;
		sensor->has_power   = 0;
		sensor->last_update = 0;
		sensor->last_read   = 0;
		break;

	case SENSOR_TYPE_T2:
		sensor->type        = type;
		sensor->id          = id;
		sensor->min_value   = -80;
		sensor->max_value   = 80;
		sensor->value       = 0;
		sensor->unit        = UNIT_DEG_C;
		sensor->has_power   = 0;
		sensor->last_update = 0;
		sensor->last_read   = 0;
		break;

	case SENSOR_TYPE_T3:
		sensor->type        = type;
		sensor->id          = id;
		sensor->min_value   = 0;
		sensor->max_value   = 700;
		sensor->value       = 0;
		sensor->unit        = UNIT_DEG_C;
		sensor->has_power   = 0;
		sensor->last_update = 0;
		sensor->last_read   = 0;
		break;

	case SENSOR_TYPE_EGT:
		sensor->type        = type;
		sensor->id          = id;
		sensor->min_value   = 0;
		sensor->max_value   = 1100;
		sensor->value       = 0;
		sensor->unit        = UNIT_DEG_C;
		sensor->has_power   = 0;
		sensor->last_update = 0;
		sensor->last_read   = 0;
		break;

	case SENSOR_TYPE_T_DUCT:
		sensor->type        = type;
		sensor->id          = id;
		sensor->min_value   = -50;
		sensor->max_value   = 300;
		sensor->value       = 0;
		sensor->unit        = UNIT_DEG_C;
		sensor->has_power   = 0;
		sensor->last_update = 0;
		sensor->last_read   = 0;
		break;

	case SENSOR_TYPE_N:
		sensor->type        = type;
		sensor->id          = id;
		sensor->min_value   = 0;
		sensor->max_value   = 18000;
		sensor->value       = 0;
		sensor->unit        = UNIT_RPM;
		sensor->has_power   = 0;
		sensor->last_update = 0;
		sensor->last_read   = 0;
		break;
	}
}

void init_discrete_sensor(Discrete_sensor* sensor, Sensor_type type, uint8_t id)
{
	sensor->type        = type;
	sensor->id          = id;
	sensor->value       = 0;
	sensor->has_power   = 0;
	sensor->last_update = 0;
	sensor->last_read   = 0;
}


