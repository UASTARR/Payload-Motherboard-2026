################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/sensors/bme680.c \
../Core/Src/sensors/bme68x.c \
../Core/Src/sensors/icm40609d.c \
../Core/Src/sensors/mmc5983ma.c \
../Core/Src/sensors/ms5611.c \
../Core/Src/sensors/sgp41.c 

OBJS += \
./Core/Src/sensors/bme680.o \
./Core/Src/sensors/bme68x.o \
./Core/Src/sensors/icm40609d.o \
./Core/Src/sensors/mmc5983ma.o \
./Core/Src/sensors/ms5611.o \
./Core/Src/sensors/sgp41.o 

C_DEPS += \
./Core/Src/sensors/bme680.d \
./Core/Src/sensors/bme68x.d \
./Core/Src/sensors/icm40609d.d \
./Core/Src/sensors/mmc5983ma.d \
./Core/Src/sensors/ms5611.d \
./Core/Src/sensors/sgp41.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/sensors/%.o Core/Src/sensors/%.su Core/Src/sensors/%.cyclo: ../Core/Src/sensors/%.c Core/Src/sensors/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F732xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-sensors

clean-Core-2f-Src-2f-sensors:
	-$(RM) ./Core/Src/sensors/bme680.cyclo ./Core/Src/sensors/bme680.d ./Core/Src/sensors/bme680.o ./Core/Src/sensors/bme680.su ./Core/Src/sensors/bme68x.cyclo ./Core/Src/sensors/bme68x.d ./Core/Src/sensors/bme68x.o ./Core/Src/sensors/bme68x.su ./Core/Src/sensors/icm40609d.cyclo ./Core/Src/sensors/icm40609d.d ./Core/Src/sensors/icm40609d.o ./Core/Src/sensors/icm40609d.su ./Core/Src/sensors/mmc5983ma.cyclo ./Core/Src/sensors/mmc5983ma.d ./Core/Src/sensors/mmc5983ma.o ./Core/Src/sensors/mmc5983ma.su ./Core/Src/sensors/ms5611.cyclo ./Core/Src/sensors/ms5611.d ./Core/Src/sensors/ms5611.o ./Core/Src/sensors/ms5611.su ./Core/Src/sensors/sgp41.cyclo ./Core/Src/sensors/sgp41.d ./Core/Src/sensors/sgp41.o ./Core/Src/sensors/sgp41.su

.PHONY: clean-Core-2f-Src-2f-sensors

