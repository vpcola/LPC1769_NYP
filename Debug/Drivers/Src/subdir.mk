################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/Src/debug_frmwrk.c \
../Drivers/Src/lpc17xx_adc.c \
../Drivers/Src/lpc17xx_can.c \
../Drivers/Src/lpc17xx_clkpwr.c \
../Drivers/Src/lpc17xx_dac.c \
../Drivers/Src/lpc17xx_emac.c \
../Drivers/Src/lpc17xx_exti.c \
../Drivers/Src/lpc17xx_gpdma.c \
../Drivers/Src/lpc17xx_gpio.c \
../Drivers/Src/lpc17xx_i2c.c \
../Drivers/Src/lpc17xx_i2s.c \
../Drivers/Src/lpc17xx_iap.c \
../Drivers/Src/lpc17xx_libcfg_default.c \
../Drivers/Src/lpc17xx_mcpwm.c \
../Drivers/Src/lpc17xx_nvic.c \
../Drivers/Src/lpc17xx_pinsel.c \
../Drivers/Src/lpc17xx_pwm.c \
../Drivers/Src/lpc17xx_qei.c \
../Drivers/Src/lpc17xx_rit.c \
../Drivers/Src/lpc17xx_rtc.c \
../Drivers/Src/lpc17xx_spi.c \
../Drivers/Src/lpc17xx_ssp.c \
../Drivers/Src/lpc17xx_systick.c \
../Drivers/Src/lpc17xx_timer.c \
../Drivers/Src/lpc17xx_uart.c \
../Drivers/Src/lpc17xx_wdt.c 

OBJS += \
./Drivers/Src/debug_frmwrk.o \
./Drivers/Src/lpc17xx_adc.o \
./Drivers/Src/lpc17xx_can.o \
./Drivers/Src/lpc17xx_clkpwr.o \
./Drivers/Src/lpc17xx_dac.o \
./Drivers/Src/lpc17xx_emac.o \
./Drivers/Src/lpc17xx_exti.o \
./Drivers/Src/lpc17xx_gpdma.o \
./Drivers/Src/lpc17xx_gpio.o \
./Drivers/Src/lpc17xx_i2c.o \
./Drivers/Src/lpc17xx_i2s.o \
./Drivers/Src/lpc17xx_iap.o \
./Drivers/Src/lpc17xx_libcfg_default.o \
./Drivers/Src/lpc17xx_mcpwm.o \
./Drivers/Src/lpc17xx_nvic.o \
./Drivers/Src/lpc17xx_pinsel.o \
./Drivers/Src/lpc17xx_pwm.o \
./Drivers/Src/lpc17xx_qei.o \
./Drivers/Src/lpc17xx_rit.o \
./Drivers/Src/lpc17xx_rtc.o \
./Drivers/Src/lpc17xx_spi.o \
./Drivers/Src/lpc17xx_ssp.o \
./Drivers/Src/lpc17xx_systick.o \
./Drivers/Src/lpc17xx_timer.o \
./Drivers/Src/lpc17xx_uart.o \
./Drivers/Src/lpc17xx_wdt.o 

C_DEPS += \
./Drivers/Src/debug_frmwrk.d \
./Drivers/Src/lpc17xx_adc.d \
./Drivers/Src/lpc17xx_can.d \
./Drivers/Src/lpc17xx_clkpwr.d \
./Drivers/Src/lpc17xx_dac.d \
./Drivers/Src/lpc17xx_emac.d \
./Drivers/Src/lpc17xx_exti.d \
./Drivers/Src/lpc17xx_gpdma.d \
./Drivers/Src/lpc17xx_gpio.d \
./Drivers/Src/lpc17xx_i2c.d \
./Drivers/Src/lpc17xx_i2s.d \
./Drivers/Src/lpc17xx_iap.d \
./Drivers/Src/lpc17xx_libcfg_default.d \
./Drivers/Src/lpc17xx_mcpwm.d \
./Drivers/Src/lpc17xx_nvic.d \
./Drivers/Src/lpc17xx_pinsel.d \
./Drivers/Src/lpc17xx_pwm.d \
./Drivers/Src/lpc17xx_qei.d \
./Drivers/Src/lpc17xx_rit.d \
./Drivers/Src/lpc17xx_rtc.d \
./Drivers/Src/lpc17xx_spi.d \
./Drivers/Src/lpc17xx_ssp.d \
./Drivers/Src/lpc17xx_systick.d \
./Drivers/Src/lpc17xx_timer.d \
./Drivers/Src/lpc17xx_uart.d \
./Drivers/Src/lpc17xx_wdt.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/Src/%.o: ../Drivers/Src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M3 -D__USE_CMSIS=CMSISv2p00_LPC17xx -D__LPC17XX__ -D__REDLIB__ -I"C:\Users\Vergil\Documents\LPCXpresso_8.1.4_606\workspace\CMSISv2p00_LPC17xx\inc" -I"C:\Users\Vergil\Documents\LPCXpresso_8.1.4_606\workspace\LPC1769_NYP\Drivers\Inc" -I"C:\Users\Vergil\Documents\LPCXpresso_8.1.4_606\workspace\LPC1769_NYP\Inc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


