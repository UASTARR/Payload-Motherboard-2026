#ifndef SGP41_H
#define SGP41_H

#include "main.h"
#include "stm32_sw_i2c.h"
#include "dwt_stm32_delay.h"

#define SGP41_I2C_ADDRESS (0x59 << 1)

typedef struct {
    float NOx;
    float TVOC;
} SGP41_Data_t;

void SGP41_Init(void);
void SGP41_Read_All(SGP41_Data_t *data);

#endif // SGP41_H