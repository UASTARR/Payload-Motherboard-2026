#include "sensors/sgp41.h"

void SGP41_Init(void) {
    uint8_t conditioning[8] = {
        0x26, 0x12,        // command
        0x80, 0x00, 0xA2,  // default humidity 0x8000 + CRC
        0x66, 0x66, 0x93   // default temp 0x6666 (abt 25°C) + CRC
    };
    // conditioning command
    I2C_transmit(SGP41_I2C_ADDRESS, conditioning, 8);
    // for 50ms
    DWT_Delay_us(50000);
}

void SGP41_Read_All(SGP41_Data_t *data) {
    uint8_t measure_cmd[8] = {
        0x26, 0x19,
        0x80, 0x00, 0xA2,  // default humidity
        0x66, 0x66, 0x93   // default temp
    };
    uint8_t cmd[1] = {0};
    uint8_t raw[6] = {0};
    uint8_t dummy[1] = {0x00};
    uint16_t sraw_voc;
    uint16_t sraw_nox;

    // send the measure command
    I2C_transmit(SGP41_I2C_ADDRESS, measure_cmd, 8);
    // wait 55ms
    DWT_Delay_us(55000);

    // read: SRAW_VOC MSB, LSB, CRC | SRAW_NOx MSB, LSB, CRC
    I2C_receive(SGP41_I2C_ADDRESS, dummy, raw, 0, 6);
    sraw_voc = ((uint16_t)raw[0] << 8) | raw[1];
    sraw_nox = ((uint16_t)raw[3] << 8) | raw[4];
    // ignore raw[2] and raw[5] (crc) lols

    // need to use GasIndexAlgorithm from Sensiron to get actual values
    data->TVOC = (float)sraw_voc;
    data->NOx  = (float)sraw_nox;
}
