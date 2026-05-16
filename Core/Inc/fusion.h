#ifndef FC_FUSION_H
#define FC_FUSION_H

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Fusion/Fusion.h"
#include "errors.h"

#define FUSION_TASK_PRIORITY    (tskIDLE_PRIORITY + 3)
#define FUSION_TASK_STACK_SIZE  (1024 * 4)
#define FUSION_TASK_NAME        "Fusion"
#define FUSION_TASK_DELAY_MS    5

Error_t Fusion_Init(void);
Error_t Fusion_Update(FusionVector gyroscope, FusionVector accelerometer, FusionVector magnetometer, float dt);
Error_t Fusion_GetQuaternion(FusionQuaternion *out);
Error_t Fusion_GetEuler(FusionEuler *out);
void Fusion_Task(void *pvParameters);

#endif // FC_FUSION_H
