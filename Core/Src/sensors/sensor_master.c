#include "sensors/sensor_master.h"

static Sensor_Master_Data_t sensor_master_data;
static SemaphoreHandle_t sensor_master_mutex;

Error_t Sensor_Master_Init(void)
{
    sensor_master_mutex = xSemaphoreCreateMutex();
    if (sensor_master_mutex == NULL) {
        return ERROR_MUTEX_CREATE_FAILED;
    }

    return ERROR_NONE;
}

static Error_t Sensor_Master_Read_All(void)
{
    if (xSemaphoreTake(sensor_master_mutex, portMAX_DELAY) != pdPASS) {
        return ERROR_MUTEX_TAKE_FAILED;
    }
    BME680_Read_All(&sensor_master_data.bme);
    MS5611_Read_All(&sensor_master_data.ms);
    SGP41_Read_All(&sensor_master_data.sgp);
    ICM40609D_Read_All(&sensor_master_data.icm);
    MMC5983MA_Read_All(&sensor_master_data.mag);
    xSemaphoreGive(sensor_master_mutex);
    return ERROR_NONE;
}

Error_t Sensor_Master_Get(Sensor_Master_Data_t *out)
{
    if (xSemaphoreTake(sensor_master_mutex, portMAX_DELAY) != pdPASS) {
        return ERROR_MUTEX_TAKE_FAILED;
    }
    *out = sensor_master_data;
    xSemaphoreGive(sensor_master_mutex);
    return ERROR_NONE;
}

void Sensor_Master_Task(void *pvParameters)
{
    while (1) {
        Sensor_Master_Read_All();
        // Sensor_Master_Print_All();
        vTaskDelay(pdMS_TO_TICKS(SENSOR_MASTER_TASK_DELAY_MS));
    }
}

void Sensor_Master_Print_All(void)
{
    Sensor_Master_Data_t data = sensor_master_data;
    printf("-------------------------------------------------\r\n");
    printf("SENSOR DATA:\r\n\r\n");
    printf("BME680: %.2f C, %.2f hPa, %.2f %%, %.2f Ohms\r\n", data.bme.temperature, data.bme.pressure, data.bme.humidity, data.bme.gas);
    printf("MS5611: %.2f hPa, %.2f m\r\n", data.ms.pressure, data.ms.altitude);
    printf("SGP41: %.0f ppb, %.0f ppb\r\n", data.sgp.TVOC, data.sgp.NOx);
    printf("ICM40609D: %.2f C, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f\r\n", data.icm.temp, data.icm.accel_x, data.icm.accel_y, data.icm.accel_z, data.icm.gyro_x, data.icm.gyro_y, data.icm.gyro_z);
    printf("MMC5983MA: %.2f uT, %.2f uT, %.2f uT\r\n", data.mag.x, data.mag.y, data.mag.z);
    printf("-------------------------------------------------\r\n\r\n");
}
