################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/led.c \
../Src/main.c \
../Src/timebase.c \
../Src/uart.c 

OBJS += \
./Src/led.o \
./Src/main.o \
./Src/timebase.o \
./Src/uart.o 

C_DEPS += \
./Src/led.d \
./Src/main.d \
./Src/timebase.d \
./Src/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/%.o Src/%.su: ../Src/%.c Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DSTM32 -DSTM32F4 -DSTM32F446RETx -DNUCLEO_F446RE -DSTM32F446xx -c -I../Inc -I"C:/Users/salil madhav/Desktop/Projects/NTOS/LedDriver0/Inc/CMSIS/Device/ST/STM32F4xx/Include" -I"C:/Users/salil madhav/Desktop/Projects/NTOS/LedDriver0/Inc/CMSIS/Include" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Src

clean-Src:
	-$(RM) ./Src/led.d ./Src/led.o ./Src/led.su ./Src/main.d ./Src/main.o ./Src/main.su ./Src/timebase.d ./Src/timebase.o ./Src/timebase.su ./Src/uart.d ./Src/uart.o ./Src/uart.su

.PHONY: clean-Src

