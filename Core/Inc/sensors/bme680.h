#ifndef BME680_H
#define BME680_H

#include "main.h"
#include "stm32_sw_i2c.h"
#include "dwt_stm32_delay.h"
#include "sensors/bme68x.h"

#define BME680_I2C_ADDRESS (0x76 << 1)

typedef struct {
    float temperature;
    float pressure;
    float humidity;
    float gas;
} BME680_Data_t;

void BME680_Init(void);
void BME680_Read_All(BME680_Data_t *data);

#endif // BME680_H