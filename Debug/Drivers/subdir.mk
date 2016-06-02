################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/lpc17xx_adc.c \
../Drivers/lpc17xx_can.c \
../Drivers/lpc17xx_clkpwr.c \
../Drivers/lpc17xx_dac.c \
../Drivers/lpc17xx_emac.c \
../Drivers/lpc17xx_exti.c \
../Drivers/lpc17xx_gpdma.c \
../Drivers/lpc17xx_gpio.c \
../Drivers/lpc17xx_i2c.c \
../Drivers/lpc17xx_i2s.c \
../Drivers/lpc17xx_iap.c \
../Drivers/lpc17xx_libcfg_default.c \
../Drivers/lpc17xx_mcpwm.c \
../Drivers/lpc17xx_nvic.c \
../Drivers/lpc17xx_pinsel.c \
../Drivers/lpc17xx_pwm.c \
../Drivers/lpc17xx_qei.c \
../Drivers/lpc17xx_rit.c \
../Drivers/lpc17xx_rtc.c \
../Drivers/lpc17xx_spi.c \
../Drivers/lpc17xx_ssp.c \
../Drivers/lpc17xx_systick.c \
../Drivers/lpc17xx_timer.c \
../Drivers/lpc17xx_uart.c \
../Drivers/lpc17xx_wdt.c 

OBJS += \
./Drivers/lpc17xx_adc.o \
./Drivers/lpc17xx_can.o \
./Drivers/lpc17xx_clkpwr.o \
./Drivers/lpc17xx_dac.o \
./Drivers/lpc17xx_emac.o \
./Drivers/lpc17xx_exti.o \
./Drivers/lpc17xx_gpdma.o \
./Drivers/lpc17xx_gpio.o \
./Drivers/lpc17xx_i2c.o \
./Drivers/lpc17xx_i2s.o \
./Drivers/lpc17xx_iap.o \
./Drivers/lpc17xx_libcfg_default.o \
./Drivers/lpc17xx_mcpwm.o \
./Drivers/lpc17xx_nvic.o \
./Drivers/lpc17xx_pinsel.o \
./Drivers/lpc17xx_pwm.o \
./Drivers/lpc17xx_qei.o \
./Drivers/lpc17xx_rit.o \
./Drivers/lpc17xx_rtc.o \
./Drivers/lpc17xx_spi.o \
./Drivers/lpc17xx_ssp.o \
./Drivers/lpc17xx_systick.o \
./Drivers/lpc17xx_timer.o \
./Drivers/lpc17xx_uart.o \
./Drivers/lpc17xx_wdt.o 

C_DEPS += \
./Drivers/lpc17xx_adc.d \
./Drivers/lpc17xx_can.d \
./Drivers/lpc17xx_clkpwr.d \
./Drivers/lpc17xx_dac.d \
./Drivers/lpc17xx_emac.d \
./Drivers/lpc17xx_exti.d \
./Drivers/lpc17xx_gpdma.d \
./Drivers/lpc17xx_gpio.d \
./Drivers/lpc17xx_i2c.d \
./Drivers/lpc17xx_i2s.d \
./Drivers/lpc17xx_iap.d \
./Drivers/lpc17xx_libcfg_default.d \
./Drivers/lpc17xx_mcpwm.d \
./Drivers/lpc17xx_nvic.d \
./Drivers/lpc17xx_pinsel.d \
./Drivers/lpc17xx_pwm.d \
./Drivers/lpc17xx_qei.d \
./Drivers/lpc17xx_rit.d \
./Drivers/lpc17xx_rtc.d \
./Drivers/lpc17xx_spi.d \
./Drivers/lpc17xx_ssp.d \
./Drivers/lpc17xx_systick.d \
./Drivers/lpc17xx_timer.d \
./Drivers/lpc17xx_uart.d \
./Drivers/lpc17xx_wdt.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/%.o: ../Drivers/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"C:\Users\Vergil\Documents\LPCXpresso_8.1.4_606\workspace\CMSISv2p00_LPC17xx\inc" -I"C:\Users\Vergil\Documents\LPCXpresso_8.1.4_606\workspace\LPC1769_NYP\Inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


