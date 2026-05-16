#include "fusion.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "task.h"
#include "sensors/sensor_master.h"

static FusionAhrs fusion_ahrs;
static FusionBias fusion_bias;
static SemaphoreHandle_t fusion_mutex;

Error_t Fusion_Init(void)
{
    fusion_mutex = xSemaphoreCreateMutex();
    if (fusion_mutex == NULL) {
        return ERROR_MUTEX_CREATE_FAILED;
    }

    FusionAhrsInitialise(&fusion_ahrs);
    FusionBiasInitialise(&fusion_bias);

    const FusionAhrsSettings settings = {
        .convention             = FusionConventionNed,
        .gain                   = 0.3f,
        .gyroscopeRange         = 2000.0f,
        .accelerationRejection  = 30.0f,
        .magneticRejection      = 20.0f,
        .recoveryTriggerPeriod  = 5,
    };
    FusionAhrsSetSettings(&fusion_ahrs, &settings);

    const FusionBiasSettings biasSettings = {
        .sampleRate          = 200.0f,
        .stationaryThreshold = 3.0f,
        .stationaryPeriod    = 5.0f,
    };
    FusionBiasSetSettings(&fusion_bias, &biasSettings);

    return ERROR_NONE;
}

Error_t Fusion_Update(FusionVector gyroscope, FusionVector accelerometer, FusionVector magnetometer, float dt)
{
    if (xSemaphoreTake(fusion_mutex, portMAX_DELAY) != pdPASS) {
        return ERROR_MUTEX_TAKE_FAILED;
    }
    FusionVector gyro_corrected = FusionBiasUpdate(&fusion_bias, gyroscope);
    FusionAhrsUpdate(&fusion_ahrs, gyro_corrected, accelerometer, magnetometer, dt);
    xSemaphoreGive(fusion_mutex);
    return ERROR_NONE;
}

Error_t Fusion_GetQuaternion(FusionQuaternion *out)
{
    if (xSemaphoreTake(fusion_mutex, portMAX_DELAY) != pdPASS) {
        return ERROR_MUTEX_TAKE_FAILED;
    }
    *out = FusionAhrsGetQuaternion(&fusion_ahrs);
    xSemaphoreGive(fusion_mutex);
    return ERROR_NONE;
}

Error_t Fusion_GetEuler(FusionEuler *out)
{
    if (xSemaphoreTake(fusion_mutex, portMAX_DELAY) != pdPASS) {
        return ERROR_MUTEX_TAKE_FAILED;
    }
    *out = FusionQuaternionToEuler(FusionAhrsGetQuaternion(&fusion_ahrs));
    xSemaphoreGive(fusion_mutex);
    return ERROR_NONE;
}

void Fusion_Print_Quaternion(void)
{
    FusionQuaternion q;
    Fusion_GetQuaternion(&q);
    printf("Q:%.4f,%.4f,%.4f,%.4f\r\n", q.element.w, q.element.x, q.element.y, q.element.z);
}

void Fusion_Task(void *pvParameters)
{
    TickType_t lastWakeTime = xTaskGetTickCount();
    TickType_t prevTick = lastWakeTime;

    while (1) {
        TickType_t now = xTaskGetTickCount();
        float dt = (float)(now - prevTick) / (float)configTICK_RATE_HZ;
        prevTick = now;

        Sensor_Master_Data_t data;
        Sensor_Master_Get(&data);

        FusionVector gyro  = {{data.icm.gyro_x,  data.icm.gyro_y,  data.icm.gyro_z}};
        FusionVector accel = {{data.icm.accel_x, data.icm.accel_y, data.icm.accel_z}};
        FusionVector mag   = {{data.mag.x,        data.mag.y,        data.mag.z}};

        gyro  = FusionRemap(gyro,  FusionRemapAlignmentPXPYPZ);
        accel = FusionRemap(accel, FusionRemapAlignmentPXPYPZ);
        mag   = FusionRemap(mag,   FusionRemapAlignmentPXPYPZ);

        Fusion_Update(gyro, accel, mag, dt);
        Fusion_Print_Quaternion();
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(FUSION_TASK_DELAY_MS));
    }
}
