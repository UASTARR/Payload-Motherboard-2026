#ifndef PID_IMPL_H
#define PID_IMPL_H

#define CONTROL_TASK_DELAY_MS   5
#define CONTROL_TASK_PRIORITY   (tskIDLE_PRIORITY + 4)
#define CONTROL_TASK_STACK_SIZE (1024 * 2)
#define CONTROL_TASK_NAME       "Servo Control"

void control_init(float dt_seconds);
void control_step(float roll_deg, float pitch_deg,
	float roll_rate_dps, float pitch_rate_dps,
	float roll_sp_deg, float pitch_sp_deg,
	float *u_roll, float *u_pitch);
void Control_Task(void *pvParameters);

#endif