/*
 * DELAY.c
 *
 *  Created on: 26 May, 2016
 *      Author: tkcov
 */

#include "DELAY.h"
#include "lpc17xx_systick.h"

__IO uint32_t TimingDelay;

/*
 * Our Systick Handler configured from
 * interrupt scheduled every 1 ms
 * Decrements the timing delay
 */
void SysTick_Handler(void)
{
  if(TimingDelay !=0)
  {
    TimingDelay --;
  }
}

void Init_Delay()
{
	//Initialize System Tick with 1ms time interval
	SYSTICK_InternalInit(1);
	//Enable System Tick interrupt
	SYSTICK_IntCmd(ENABLE);
	//Enable System Tick Counter
	SYSTICK_Cmd(ENABLE);
}

void Delay(unsigned int milliseconds)
{
	TimingDelay = milliseconds;
	while(TimingDelay > 0);
}
