################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/pid/pid.c \
../Core/Src/pid/pid_impl.c 

OBJS += \
./Core/Src/pid/pid.o \
./Core/Src/pid/pid_impl.o 

C_DEPS += \
./Core/Src/pid/pid.d \
./Core/Src/pid/pid_impl.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/pid/%.o Core/Src/pid/%.su Core/Src/pid/%.cyclo: ../Core/Src/pid/%.c Core/Src/pid/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F732xx -c -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-pid

clean-Core-2f-Src-2f-pid:
	-$(RM) ./Core/Src/pid/pid.cyclo ./Core/Src/pid/pid.d ./Core/Src/pid/pid.o ./Core/Src/pid/pid.su ./Core/Src/pid/pid_impl.cyclo ./Core/Src/pid/pid_impl.d ./Core/Src/pid/pid_impl.o ./Core/Src/pid/pid_impl.su

.PHONY: clean-Core-2f-Src-2f-pid

