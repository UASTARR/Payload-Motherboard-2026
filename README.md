## Payload Motherboard STM32 Codebase

This repo contains the code for Payload's motherboard. The main execution path is defined by the main state machine, which detects changes in acceleration and altitude to determine different phases of flight and activate phase-specific control.  

Currently, the code enables sensor data collection from 5 onboard sensors - a BME680 (temp + pressure + humidity + gas resistance), SGP41 (TVOC + NOx), MS5611 (baro pressure + altitude), ICM40609-D (gyro + accelerometer), and a MMC5983MA (magnetometer). Software I2C is used due to a mistake in the hardware design. It also enables the use of an SD card for data logging purposes (note that a hardware change is necessary to use the SD card slot). A fusion algorithm is used to determine the absolute orientation of the payload during flight, and this data is used in a PID algorithm to control the angle of servo motors attached to external gridfins.  

The state machine is disabled in the code as of 2026-05-18. Absolute orientation data can be output over USB CDC to be read by the attached computer, which can be used to simulate a model of the board/payload and its orientation, for testing purposes.
