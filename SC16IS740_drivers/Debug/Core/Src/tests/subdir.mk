################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/tests/IS740_receiveStream_test.c 

OBJS += \
./Core/Src/tests/IS740_receiveStream_test.o 

C_DEPS += \
./Core/Src/tests/IS740_receiveStream_test.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/tests/%.o Core/Src/tests/%.su Core/Src/tests/%.cyclo: ../Core/Src/tests/%.c Core/Src/tests/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I"D:/Documents/SC16IS740-STM32-Driver/SC16IS740_drivers/Drivers/SC16IS740/inc" -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-tests

clean-Core-2f-Src-2f-tests:
	-$(RM) ./Core/Src/tests/IS740_receiveStream_test.cyclo ./Core/Src/tests/IS740_receiveStream_test.d ./Core/Src/tests/IS740_receiveStream_test.o ./Core/Src/tests/IS740_receiveStream_test.su

.PHONY: clean-Core-2f-Src-2f-tests

