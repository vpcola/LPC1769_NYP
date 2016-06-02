/*
 * EXTI.c
 *
 *  Created on: May 29, 2016
 *      Author: vpcol
 */

#include "EXTI.h"
#include "lpc17xx_gpio.h"
#include "lpc17xx_exti.h"
#include "GPIO.h"

// Called internally by the EXTI2 (Touch Interrupt)
extern void XPT2046_Update(void);
// Called internally by the EXTI3 (Pushbutton Key)
extern void Keyboard_Update(void);

// The EIN2 external interrupt handler,
// Nothing here for the moment, for the time being
// lets have some fun, togle an LED.
void EINT2_IRQHandler()
{
	// Todo: Handle the interrupt here, for now
	// Set the green led to on
	GPIO_GreenLed(0);

	XPT2046_Update();

	// Clear the EXTI Flag so that further interrupts can
	// be generated.
	EXTI_ClearEXTIFlag(EXTI_EINT2);
}

// The EIN3 external interrupt handler,
// Nothing here for the moment
void EINT3_IRQHandler()
{
	// Todo: Handle the interrupt here
	Keyboard_Update();

	// Clear the EXTI Flag so that further interrupts can
	// be generated.
	EXTI_ClearEXTIFlag(EXTI_EINT3);
}

void Init_Exti_Pins(void)
{
    PINSEL_CFG_Type ExtIntPins;
    ExtIntPins.Portnum = PUSHBUTTON_INT_PORT;
    ExtIntPins.Pinnum = PUSHBUTTON_INT_PIN;
    ExtIntPins.Pinmode = PINSEL_PINMODE_PULLUP;
    ExtIntPins.OpenDrain = PINSEL_PINMODE_NORMAL;
    // External Interrupt pin - Alternate function 0x01
    ExtIntPins.Funcnum = PINSEL_FUNC_1; // EXTI3
    PINSEL_ConfigPin(&ExtIntPins);

    // External Interrupt pin - Alternate function 0x01
    ExtIntPins.Portnum = TOUCH_INT_PORT;
    ExtIntPins.Funcnum = PINSEL_FUNC_1; // EXTI2
    ExtIntPins.Pinnum = TOUCH_INT_PIN;
    PINSEL_ConfigPin(&ExtIntPins);

    // Configure PushButton INT as input
    GPIO_SetDir(PUSHBUTTON_INT_PORT, (1 << PUSHBUTTON_INT_PIN), GPIO_DIR_INPUT);
    // Configure TOUCH INT as input
    GPIO_SetDir(TOUCH_INT_PORT, (1 << TOUCH_INT_PIN), GPIO_DIR_INPUT);
}

void Init_EXTI(void)
{
	Init_Exti_Pins();

    // Initialize external interrupts
    EXTI_Init();
}

int EXTI2_ReadIntPin()
{
	return ((LPC_GPIO2->FIOPIN & (1 << TOUCH_INT_PIN))>> TOUCH_INT_PIN);
}

int EXTI3_ReadIntPin()
{
	return ((LPC_GPIO2->FIOPIN & (1 << PUSHBUTTON_INT_PIN))>> PUSHBUTTON_INT_PIN);
}

void EXTI2_Enable()
{
    // Setup NVIC for External interrupt EINT2
    // According to specs XPT2046 touch controller
    // goes down (negative edge trigger) when touched.
    EXTI_InitTypeDef Exti2;
    Exti2.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
    Exti2.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
    Exti2.EXTI_Line = EXTI_EINT2;
    EXTI_Config(&Exti2);

	/* Install interrupt for EINT2 interrupt */
	NVIC_SetPriority(EINT2_IRQn, TOUCH_INT_INTERRUPT_PRIORITY);
	/* Enable the EINT2 interrupt*/
	NVIC_EnableIRQ(EINT2_IRQn);
}

void EXTI2_Disable()
{
	NVIC_DisableIRQ(EINT2_IRQn);
}

void EXTI3_Enable()
{
    // Setup NVIC for External interrupt EINT2
    // According to specs XPT2046 touch controller
    // goes down (negative edge trigger) when touched.
    EXTI_InitTypeDef Exti3;
    Exti3.EXTI_Mode = EXTI_MODE_EDGE_SENSITIVE;
    Exti3.EXTI_polarity = EXTI_POLARITY_LOW_ACTIVE_OR_FALLING_EDGE;
    Exti3.EXTI_Line = EXTI_EINT3;
    EXTI_Config(&Exti3);

	/* Install interrupt for EINT2 interrupt */
	NVIC_SetPriority(EINT3_IRQn, PUSHBUTTON_INT_INTERRUPT_PRIORITY);
	/* Enable the EINT3 interrupt*/
	NVIC_EnableIRQ(EINT3_IRQn);
}

void EXTI3_Disable()
{
	NVIC_DisableIRQ(EINT3_IRQn);
}
