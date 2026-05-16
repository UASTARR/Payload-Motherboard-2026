#include <stdio.h>
#include <math.h>

#include "pid/pid.h"
#include "pid/pid_impl.h"
#include "fusion.h"
#include "sensors/sensor_master.h"
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "state_machine.h"

extern TIM_HandleTypeDef htim2;

static PIDController att_roll, att_pitch;
static PIDController rate_roll, rate_pitch;

static uint32_t pwm_clamp(float u)
{
    int32_t v = 1500 + (int32_t)(u * 500.0f);
    if (v > 2000) v = 2000;
    if (v < 1000) v = 1000;
    return (uint32_t)v;
}

void Control_Task(void *pvParameters)
{
    control_init(CONTROL_TASK_DELAY_MS / 1000.0f);
    TickType_t lastWakeTime = xTaskGetTickCount();
    while (1) {
        if (!StateMachine_PIDEnabled())
        {
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 1500);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 1500);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, 1500);
            __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, 1500);
            vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(CONTROL_TASK_DELAY_MS));
            continue;
        }


        FusionEuler euler;
        Fusion_GetEuler(&euler);
        Sensor_Master_Data_t data;
        Sensor_Master_Get(&data);
        float u_roll = 0.0f, u_pitch = 0.0f;
        control_step(euler.angle.roll,  euler.angle.pitch,
                     data.icm.gyro_x,  data.icm.gyro_y,
                     0.0f, 0.0f,
                     &u_roll, &u_pitch);
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, pwm_clamp(-u_pitch));
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, pwm_clamp( u_roll));
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_3, pwm_clamp( u_pitch));
        __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_4, pwm_clamp(-u_roll));
        vTaskDelayUntil(&lastWakeTime, pdMS_TO_TICKS(CONTROL_TASK_DELAY_MS));
    }
}

void control_init(float dt_seconds)
{
    /* constants for attitude control */
    att_roll.Kp = 1.0f;
    att_roll.Ki = 0.05f;
    att_roll.Kd = 0.0f;
    att_roll.tau = 0.02f;
    att_roll.limMin = -100.0f;
    att_roll.limMax = 100.0f;
    att_roll.limMinInt = -5.0f;
    att_roll.limMaxInt = 5.0f;
    att_roll.T = dt_seconds;
    PIDController_Init(&att_roll);

    att_pitch = att_roll;
    PIDController_Init(&att_pitch);

    /* constants for rate control */
    rate_roll.Kp = 0.10f;
    rate_roll.Ki = 0.03f;
    rate_roll.Kd = 0.05f;
    rate_roll.tau = 0.02f;
    rate_roll.limMin = -1.0f;
    rate_roll.limMax = 1.0f;
    rate_roll.limMinInt = -0.1f;
    rate_roll.limMaxInt = 0.1f;
    rate_roll.T = dt_seconds;
    PIDController_Init(&rate_roll);

    rate_pitch = rate_roll;
    PIDController_Init(&rate_pitch);
}

/* called on each iteration */
void control_step(float roll_deg, float pitch_deg,
                  float roll_rate_dps, float pitch_rate_dps,
                  float roll_sp_deg, float pitch_sp_deg,
                  float *u_roll, float *u_pitch)
{
    /* grab new roll rate */
    float roll_rate_sp  = PIDController_Update(&att_roll,  roll_sp_deg,  roll_deg);
    float pitch_rate_sp = PIDController_Update(&att_pitch, pitch_sp_deg, pitch_deg);

    /* grab controller output */
    float u_r = PIDController_Update(&rate_roll,  roll_rate_sp,  roll_rate_dps);
    float u_p = PIDController_Update(&rate_pitch, pitch_rate_sp, pitch_rate_dps);

    *u_roll  = u_r;
    *u_pitch = u_p;
}

/* Simulation */
// int main(void)
// {
//     // Simulation params
//     const double dt = 0.005;
//     const double sim_time = 20.0;

//     // kg·m^2
//     const double Ixx = 0.05;
//     const double Iyy = 0.08;

//     /* actuator authority and dynamics */
//     double Kroll  = 0.05;
//     double Kpitch = 0.05;
//     /* simulated lag */
//     const double servo_tau = 0.03;
//     /* aero damping (simulated drag forces) */
//     const double damp_roll  = 0.03;
//     const double damp_pitch = 0.03;

//     /* state (rad, rad/s) */
//     double phi = 30.0 * M_PI/180.0; /* roll */
//     double theta = -20.0 * M_PI/180.0; /* pitch */
//     double rr = 0.0; /* roll rate */
//     double pr = 0.0; /* pitch rate */

//     double u_r_eff = 0.0, u_p_eff = 0.0;

//     /* setpoints in degrees */
//     const float roll_sp_deg = 0.0f;
//     const float pitch_sp_deg = 0.0f;

//     control_init((float)dt);

//     for (int k = 0, steps = (int)(sim_time/dt); k <= steps; ++k) {
//         /* convert to controller units (degrees, degrees per second) */
//         float roll_deg       = (float)(phi * 180.0/M_PI);
//         float pitch_deg      = (float)(theta * 180.0/M_PI);
//         float roll_rate_dps  = (float)(rr * 180.0/M_PI);
//         float pitch_rate_dps = (float)(pr * 180.0/M_PI);

//         float u_r = 0.0f, u_p = 0.0f;

//         /* get commands from controller */
//         control_step(roll_deg, pitch_deg, roll_rate_dps, pitch_rate_dps,
//                      roll_sp_deg, pitch_sp_deg, &u_r, &u_p);

//         /* simulate lag of servos actuators */
//         u_r_eff += ((double)u_r - u_r_eff) * (dt/servo_tau);
//         u_p_eff += ((double)u_p - u_p_eff) * (dt/servo_tau);

//         /* simulate aerodynamic forces by damping the rates (idk this might be shitty) */
//         double tau_x = Kroll  * u_r_eff - damp_roll  * rr;
//         double tau_y = Kpitch * u_p_eff - damp_pitch * pr;

//         /* simulating a disturbance torque */

//         /*
//          * this is affecting the pitch rate;
//          * we might want to configure when this is applied
//          * and see how it responds (e.g. in momentary impulses)
//          *
//          * we also should consider doing this with roll rate too
//          */
//         // tau_x += 0.02;

//         /* use moments to update the rates */
//         double pdot = tau_x / Ixx;
//         double qdot = tau_y / Iyy;

//         /* integrate */
//         rr += pdot * dt;
//         pr += qdot * dt;
//         phi   += rr * dt;
//         theta += pr * dt;

//         if (k % (int)(1.0/dt) == 0) {
//             printf("t=%5.2fs  roll=%6.2f deg  pitch=%6.2f deg  u=(% .3f,% .3f)\n",
//                    k*dt, roll_deg, pitch_deg, (double)u_r, (double)u_p);
//         }
//     }
//     return 0;
// }