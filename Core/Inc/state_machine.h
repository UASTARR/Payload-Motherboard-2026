#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include <stdatomic.h>

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

#define STATE_MACHINE_TASK_PRIORITY   (tskIDLE_PRIORITY + 5)
#define STATE_MACHINE_TASK_STACK_SIZE (1024 * 2)
#define STATE_MACHINE_TASK_NAME       "Primary State Machine"
#define STATE_MACHINE_TASK_DELAY_MS   10

#define STATE_LAUNCH_DETECT_THRESHOLD_G         (4.0f)
#define STATE_LAUNCH_CONSECUTIVE_DETECT_COUNT   (3u)

#define STATE_MIN_ASCENT_TIME_MS                (600u)

#define STATE_APOGEE_POS_G                      (2.5f)
#define STATE_APOGEE_NEG_G                      (-2.5f)

#define STATE_DESCENT_MAIN_DEPLOY_M             (5000u)

typedef enum {
    STATE_INIT = 0,
    STATE_PAD,
    STATE_ASCENT,
    STATE_APOGEE,
    STATE_DESCENT,
    STATE_RECOVERY,
} State_t;

void State_Machine_Task(void *pvParameters);

State_t StateMachine_GetState(void);
unsigned char StateMachine_PIDEnabled(void);
unsigned char StateMachine_SDLogEnabled(void);

#endif // STATE_MACHINE_H