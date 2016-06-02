/*
===============================================================================
Name        : NYPProject.c
Author      : $(author)
Version     :
Copyright   : $(copyright)
Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif

#include <stdio.h>
#include <string.h>

#include "ADC.h"
#include "UART.h"
#include "GPIO.h"
#include "I2C.h"
#include "DELAY.h"
#include "EXTI.h"
#include "SSP0.h"
#include "SSP1.h"
#include "HTU21D.h"
#include "LCD.h"
#include "XPT2046.h"
#include "BUTTON.h"
#include "SEVENSEGMENT.h"
#include "BH1750.h"
#include "RTC.h"
#include "GUI.h"


void Init_Peripherals()
{
	// Initialize Systick and Delay
    Init_Delay();
    // Initialize GPIO Leds
    Init_GPIO();
    // Initialize UART3
    Init_UART3(9600);

    // Initialize I2C peripherals
    Init_I2C2();
    Init_HTU21D();
    Init_SevenSeg();
    Init_BH1750();

    // Initialize SPI peripherals
    // Initializes SSP0 and the LCD
    Init_SSP0();
    Init_SSP1();
    // Initialize the ILI9341 LCD
    Init_LCD();
    // Initialize the touch controller
    Init_XPT2046();

    // Initialize External Interrupts
    Init_EXTI();

    // Initialize RTC
    Init_RTC();

    // Initialize ADC
    Init_ADC();

}



int main(void)
{
    // Initialize peripherals used
    Init_Peripherals();

    // Start main message processing
    GUI_ProcessMessages();

    return 0 ;
}
