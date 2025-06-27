################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/SC16IS740/src/SC16IS740.c 

OBJS += \
./Drivers/SC16IS740/src/SC16IS740.o 

C_DEPS += \
./Drivers/SC16IS740/src/SC16IS740.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/SC16IS740/src/%.o Drivers/SC16IS740/src/%.su Drivers/SC16IS740/src/%.cyclo: ../Drivers/SC16IS740/src/%.c Drivers/SC16IS740/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../USB_HOST/App -I"D:/Documents/SC16IS740-STM32-Driver/SC16IS740_drivers/Drivers/SC16IS740/inc" -I../USB_HOST/Target -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Host_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Host_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-SC16IS740-2f-src

clean-Drivers-2f-SC16IS740-2f-src:
	-$(RM) ./Drivers/SC16IS740/src/SC16IS740.cyclo ./Drivers/SC16IS740/src/SC16IS740.d ./Drivers/SC16IS740/src/SC16IS740.o ./Drivers/SC16IS740/src/SC16IS740.su

.PHONY: clean-Drivers-2f-SC16IS740-2f-src

