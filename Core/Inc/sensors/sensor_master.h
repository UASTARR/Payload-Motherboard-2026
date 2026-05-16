#ifndef SENSOR_MASTER_H
#define SENSOR_MASTER_H

#include <stdio.h>

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "sensors/bme680.h"
#include "sensors/ms5611.h"
#include "sensors/sgp41.h"
#include "sensors/icm40609d.h"
#include "sensors/mmc5983ma.h"
#include "errors.h"

#define SENSOR_MASTER_TASK_PRIORITY     (tskIDLE_PRIORITY + 2)
#define SENSOR_MASTER_TASK_STACK_SIZE   (1024 * 4)
#define SENSOR_MASTER_TASK_NAME         "Sensor Master"
#define SENSOR_MASTER_TASK_DELAY_MS     10

typedef struct {
    BME680_Data_t bme;
    MS5611_Data_t ms;
    SGP41_Data_t sgp;
    ICM40609D_Data_t icm;
    MMC5983MA_Data_t mag;
} Sensor_Master_Data_t;

Error_t Sensor_Master_Init(void);
Error_t Sensor_Master_Get(Sensor_Master_Data_t *out);
void Sensor_Master_Task(void *pvParameters);
void Sensor_Master_Print_All(void);
#endif // SENSOR_MASTER_H
