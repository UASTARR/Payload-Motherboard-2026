#include "state_machine.h"
#include "main.h"
#include "FreeRTOS.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

static State_t state = STATE_INIT;
static State_t next_state = STATE_INIT;

_Atomic(unsigned char) state_pid_enabled;
_Atomic(unsigned char) state_sdlog_enabled;

static TickType_t t_launch_ticks;
static unsigned int consec_launch_hi;
static float pad_altitude_m;

#define SET_PID(en) atomic_store_explicit(&state_pid_enabled, (en), memory_order_relaxed)
#define SET_SDLOG(en) atomic_store_explicit(&state_sdlog_enabled, (en), memory_order_relaxed)

static inline float vec_mag_g(float ax, float ay, float az)
{
    return sqrtf(ax * ax + ay * ay + az * az);
}

static void set_servos_neutral(void)
{
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1500);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1500);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1500);
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1500);
}

static void enable_buzzer(void)
{
    HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
    __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (__HAL_TIM_GET_AUTORELOAD(&htim1) / 2U));
}

unsigned char StateMachine_PIDEnabled(void)
{
    return atomic_load_explicit(&state_pid_enabled, memory_order_relaxed);
}

unsigned char StateMachine_SDLogEnabled(void)
{
    return atomic_load_explicit(&state_sdlog_enabled, memory_order_relaxed);
}

void State_Machine_Task(void *pvParameters)
{
    SET_PID(0);
    SET_SDLOG(0);

    int apogee_phase = 0;
    float prev_axial_filtered = 0.0f;
    unsigned consec_near_pad = 0;
    TickType_t t_apogee_ticks = 0;
    unsigned char buzzer_started = 0;

    for (;;)
    {
        Sensor_Master_Data_t data;
        if (Sensor_Master_Get(&data) != ERROR_NONE)
        {
            vTaskDelay(pdMS_TO_TICKS(STATE_MACHINE_TASK_DELAY_MS));
            continue;
        }

        switch (state)
        {
            case (STATE_INIT):
            {
                SET_PID(0);
                SET_SDLOG(0);
                set_servos_neutral();
                consec_launch_hi = 0;
                pad_altitude_m = data.ms.altitude;
                state = STATE_PAD;
                break;
            }
            
            case (STATE_PAD):
            {
                SET_PID(0);
                SET_SDLOG(0);
                set_servos_neutral();

                // Adjust pad altitude
                pad_altitude_m += 0.02f * (data.ms.altitude - pad_altitude_m);

                float accel_g = vec_mag_g(data.icm.accel_x, data.icm.accel_y, data.icm.accel_z);
                consec_launch_hi = (accel_g > STATE_LAUNCH_DETECT_THRESHOLD_G) ? (consec_launch_hi + 1) : (0);
                if (consec_launch_hi >= STATE_LAUNCH_CONSECUTIVE_DETECT_COUNT)
                {
                    t_launch_ticks = xTaskGetTickCount();
                    apogee_phase = 0;
                    prev_axial_filtered = data.icm.accel_z;
                    state = STATE_ASCENT;
                }
                break;
            }

            case (STATE_ASCENT):
            {
                SET_PID(0);
                SET_SDLOG(1);

                float alpha = 0.25f;
                float axial = alpha * data.icm.accel_z + (1.0f - alpha) * prev_axial_filtered;
                prev_axial_filtered = axial;

                uint32_t elapsed_ms = 
                        (xTaskGetTickCount() - t_launch_ticks) * portTICK_PERIOD_MS;
                
                if (elapsed_ms >= STATE_MIN_ASCENT_TIME_MS &&
                    prev_axial_filtered < STATE_APOGEE_NEG_G &&
                    data.icm.accel_z > STATE_APOGEE_NEG_G)
                    state = STATE_APOGEE;
                
                break;
            }

            case (STATE_APOGEE):
            {
                SET_PID(0);
                SET_SDLOG(1);
                set_servos_neutral();

                printf("APOGEE (altitude: %.2f m, time = %lu ms)\r\n",
                        data.ms.altitude,
                        (xTaskGetTickCount() - t_launch_ticks) * portTICK_PERIOD_MS);

                t_apogee_ticks = xTaskGetTickCount();
                consec_near_pad = 0;
                state = STATE_DESCENT;
                break;
            }

            case (STATE_DESCENT):
            {
                SET_PID(1);
                SET_SDLOG(1);

                float dh_m = fabsf(data.ms.altitude - pad_altitude_m);
                consec_near_pad = (dh_m < STATE_DESCENT_MAIN_DEPLOY_M) ? (consec_near_pad + 1) : (0);
                if (consec_near_pad >= STATE_DESCENT_MAIN_DEPLOY_COUNT)
                    state = STATE_RECOVERY;
            
                uint32_t max_desc_ms = 600000U;
                if (((xTaskGetTickCount() - t_apogee_ticks) * portTICK_PERIOD_MS) > max_desc_ms)
                    state = STATE_RECOVERY;

                break;
            }

            case (STATE_RECOVERY):
            {
                SET_PID(0);
                SET_SDLOG(0);
                set_servos_neutral();

                if (!buzzer_started)
                {
                    buzzer_started = 1;
                    enable_buzzer();
                }
                break;
            }

            default:
                state = STATE_INIT;
                break;
        }
    }
}