################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/OSKernel.c \
../Src/main.c 

OBJS += \
./Src/OSKernel.o \
./Src/main.o 

C_DEPS += \
./Src/OSKernel.d \
./Src/main.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DSTM32F446xx -c -I../Inc -I"C:/Users/salil madhav/Desktop/Projects/NTOS/LedDriver0/Inc/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/salil madhav/Desktop/Projects/NTOS/LedDriver0/Inc/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/OSKernel.d ./Src/OSKernel.o ./Src/OSKernel.su ./Src/main.d ./Src/main.o ./Src/main.su

.PHONY: clean-Src

