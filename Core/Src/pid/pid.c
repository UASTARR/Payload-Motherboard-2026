#include "pid/pid.h"

// source: https://github.com/pms67/PID

void PIDController_Init(PIDController *pid) {
	pid->integrator = 0.0f;
	pid->prevError  = 0.0f;

	pid->differentiator  = 0.0f;
	pid->prevMeasurement = 0.0f;

	pid->out = 0.0f;
}

float PIDController_Update(PIDController *pid, float setpoint, float measurement) {
	/* error signal */	
    float error = setpoint - measurement;

	/* proportional */
    float proportional = pid->Kp * error;

	/* integral */
    pid->integrator = pid->integrator + 0.5f * pid->Ki * pid->T * (error + pid->prevError);

	/* anti-wind-up via integrator clamping */
    if (pid->integrator > pid->limMaxInt) {
        pid->integrator = pid->limMaxInt;
    } else if (pid->integrator < pid->limMinInt) {
        pid->integrator = pid->limMinInt;
    }

	/* derivative */
    pid->differentiator = -(2.0f * pid->Kd * (measurement - pid->prevMeasurement)
                        + (2.0f * pid->tau - pid->T) * pid->differentiator)
                        / (2.0f * pid->tau + pid->T);

	/* get output */
    pid->out = proportional + pid->integrator + pid->differentiator;
    /* apply limits */
    if (pid->out > pid->limMax) {
        pid->out = pid->limMax;
    } else if (pid->out < pid->limMin) {
        pid->out = pid->limMin;
    }

	/* store for next iteration */
    pid->prevError       = error;
    pid->prevMeasurement = measurement;

	/* Return controller output */
    return pid->out;
}
