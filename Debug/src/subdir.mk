################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/ADC.c \
../src/BH1750.c \
../src/BUTTON.c \
../src/DELAY.c \
../src/EXTI.c \
../src/GPIO.c \
../src/GUI.c \
../src/HTU21D.c \
../src/I2C.c \
../src/LCD.c \
../src/RTC.c \
../src/SEVENSEGMENT.c \
../src/SSP0.c \
../src/SSP1.c \
../src/UART.c \
../src/XPT2046.c \
../src/cr_startup_lpc175x_6x.c \
../src/crp.c \
../src/main.c 

OBJS += \
./src/ADC.o \
./src/BH1750.o \
./src/BUTTON.o \
./src/DELAY.o \
./src/EXTI.o \
./src/GPIO.o \
./src/GUI.o \
./src/HTU21D.o \
./src/I2C.o \
./src/LCD.o \
./src/RTC.o \
./src/SEVENSEGMENT.o \
./src/SSP0.o \
./src/SSP1.o \
./src/UART.o \
./src/XPT2046.o \
./src/cr_startup_lpc175x_6x.o \
./src/crp.o \
./src/main.o 

C_DEPS += \
./src/ADC.d \
./src/BH1750.d \
./src/BUTTON.d \
./src/DELAY.d \
./src/EXTI.d \
./src/GPIO.d \
./src/GUI.d \
./src/HTU21D.d \
./src/I2C.d \
./src/LCD.d \
./src/RTC.d \
./src/SEVENSEGMENT.d \
./src/SSP0.d \
./src/SSP1.d \
./src/UART.d \
./src/XPT2046.d \
./src/cr_startup_lpc175x_6x.d \
./src/crp.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"C:\Users\Vergil\Documents\LPCXpresso_8.1.4_606\workspace\CMSISv2p00_LPC17xx\inc" -I"C:\Users\Vergil\Documents\LPCXpresso_8.1.4_606\workspace\LPC1769_NYP\Drivers\Inc" -I"C:\Users\Vergil\Documents\LPCXpresso_8.1.4_606\workspace\LPC1769_NYP\Inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


