#ifndef PID_CONTROLLER_H
#define PID_CONTROLLER_H

// source: https://github.com/pms67/PID

typedef struct {
	// controller gains
	float Kp;
	float Ki;
	float Kd;

	// derivative low-pass filter time constant
	float tau;

	// output limits
	float limMin;
	float limMax;
	
	// integrator limits
	float limMinInt;
	float limMaxInt;

	// sample time (in seconds)
	float T;

	// controller "memory"
	float integrator;
	float prevError;			// required for integrator
	float differentiator;
	float prevMeasurement;		// required for differentiator

	// controller output
	float out;
} PIDController;

void  PIDController_Init(PIDController *pid);
float PIDController_Update(PIDController *pid, float setpoint, float measurement);

#endif
