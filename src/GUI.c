/*
 * GUI.c
 *
 *  Created on: 30 May, 2016
 *      Author: tkcov
 */

#include "GUI.h"
#include "ADC.h"
#include "LCD.h"
#include "BUTTON.h"
#include "RTC.h"
#include "EXTI.h"
#include "GPIO.h"
#include "SEVENSEGMENT.h"
#include "XPT2046.h"
#include "HTU21D.h"
#include "BH1750.h"
#include "UART.h"
#include "DELAY.h"
#include "Arial12x12.h"
#include "Arial24x23.h"
#include "Arial28x28.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define HOME_SCREEN 0
#define MAIN_SCREEN 1
#define BLACK_SCREEN 2

static const char * EnterLabel = "Enter";
static const char * OnLabel = "On";
static const char * OffLabel = "Off";

BUTTON_Type HomeButtons[BUTTON_MAX_BUTTONS];
BUTTON_Type MainButtons[BUTTON_MAX_BUTTONS];

int8_t MainButtonLayout[2][2];

__IO uint32_t UpdateTime;
__IO uint32_t UpdateTemp;
__IO uint32_t AlarmHit;
__IO uint32_t KeyboardHit;

typedef enum {
	KEY_NONE, // Nothing pressed
	KEY_LEFT,
	KEY_RIGHT,
	KEY_UP,
	KEY_DOWN
} KEY_PRESS;

// Main button ids
uint8_t light_on, light_off, fan_on, fan_off;

// Callback functions when an RTC interrupt
// occurs.
// OnRTCUpdate - periodic update scheduled at 1 sec
// interval. This is used to update the time display,
// sensor readout - temp, humidity, and lux. This is
// set by RTC_SetUpdateCallback(OnRTCUpdate).
//
// OnRTCAlarm - When a scheduled alarm goes off. Alarm
// is set by calling RTC_SetAlarmSeconds(OnRTCAlarm, 10).
//
// NOTE: Since these are called by interrupts, we do not
// do any extra processing here except to update flags
void OnRTCUpdate(uint32_t secval)
{
	UpdateTime = 1;
	// Update the humid, lux and temp every 2 seconds
	if ((secval % 2 ) == 0)
		UpdateTemp = 1;
}

void OnRTCAlarm(void)
{
	AlarmHit = 1;
}
// This function is called by the INT3 interrupt when
// the joystick button is pressed.
void Keyboard_Update(void)
{
	KeyboardHit = 1;
}

void GUI_UpdateTime()
{
	int x;
	char datestr[100];
	RTC_TIME_Type RTCFullTime;

	RTC_GetFullTime (LPC_RTC, &RTCFullTime);

    LCD_SetFont(Arial12x12);
    LCD_Foreground(White);
    LCD_Background(Blue);

	sprintf(datestr, "%02d/%02d/%04d", RTCFullTime.DOM, RTCFullTime.MONTH, RTCFullTime.YEAR );
    x = LCD_GetStringExtentX(datestr);
    x = (LCD_Width() - x) / 2;
    // LCD_PutStr(65, 45, title);
    LCD_PutStr(x, 40, datestr);

    sprintf(datestr, "%02d:%02d:%02d", RTCFullTime.HOUR, RTCFullTime.MIN, RTCFullTime.SEC);
    x = LCD_GetStringExtentX(datestr);
    x = (LCD_Width() - x) / 2;
    // LCD_PutStr(65, 45, title);
    LCD_PutStr(x, 55, datestr);

}

void GUI_UpdateTemp()
{
	char buffer[100];
	int turnLightOn, turnFanOn;
	float luminance, humidity, temperature;

    LCD_SetFont(Arial24x23);
    LCD_Foreground(Yellow);
    LCD_Background(Black);

    if (BH1750_GetLux(&luminance) == 0)
    {
    	sprintf(buffer, "%.1f",luminance);
    }else
    	sprintf(buffer, "Error");
    // Erase the line first to make a clean
    // output
    LCD_PutStr(145, 35, "     ");
    LCD_PutStr(145, 35, buffer);

    if (HTU21D_ReadHumidity(&humidity) == 0)
    	sprintf(buffer, "%.2f", humidity);
    else
    	sprintf(buffer, "Error");
    LCD_PutStr(145, 75, buffer);

    if (HTU21D_ReadTemperature(&temperature) == 0)
    {
    	sprintf(buffer, "%.2f", temperature);
    }else
    	sprintf(buffer, "Error");
    LCD_PutStr(145, 115, buffer);

    if (luminance < 10.0 )
    {
    	UART3_XBEESend(XBEE_BROADCAST_ADDR, "D3", 1);
    	// Turn on Blue Led
    	GPIO_BlueLed(1);
    }
    else
    {
    	UART3_XBEESend(XBEE_BROADCAST_ADDR, "D3", 0);
    	GPIO_BlueLed(0);
    }

    if (temperature > 30.0 )
    {
		UART3_XBEESend(XBEE_BROADCAST_ADDR, "D4", 1);
		// Turn on Red Led
		GPIO_RedLed(1);
    }
    else
    {
		UART3_XBEESend(XBEE_BROADCAST_ADDR, "D4", 0);
		GPIO_RedLed(0);
    }


}


int GUI_ProcessMessages()
{
	int nextScreen = 0;

	// Start screen updates to display time/temp/lux
	RTC_SetUpdateCallback(OnRTCUpdate);

	// Initialize Flags
	UpdateTime = 0;
	AlarmHit = 0;
	KeyboardHit = 0;

	// Disable all indicator LEDs
	GPIO_GreenLed(0);
	GPIO_RedLed(0);
	GPIO_BlueLed(0);


	// Enable INT2/INT3 here?
	EXTI3_Enable();

	// Enable RTC interrupts to update Clock label
	RTC_Start();
	nextScreen = HOME_SCREEN;

	while(1)
	{
		switch(nextScreen)
		{
		case HOME_SCREEN : GUI_DisplayHome();
			nextScreen = GUI_ProcessHome();
			break;
		case MAIN_SCREEN : GUI_DisplayMain();
			nextScreen = GUI_ProcessMain();
			break;
		case BLACK_SCREEN: GUI_DisplayNone();
			nextScreen = GUI_ProcessNone();
			break;
		}
	}
}

int GUI_ProcessHome()
{
	int num, x, y; // to display on seven segment
	int countstart;

	// Set the alarm update callback
	RTC_SetAlarmSeconds(OnRTCAlarm, 10);

	// Reset Alarms
	AlarmHit = 0;

	// All off
	SEVENSEG_Set(10);

	num = 9;
	countstart = 0;
	// Home message processing
	while(1) // while button is not pressed
	{
		if (AlarmHit)
		{
			// Todo: When an alarm hits, we should
			// countdown before coming to a BLACK_SCREEN
			AlarmHit = 0; // Reset Flag
			countstart = 1;
		}

		// Every 1 second the updatetime is
		// set. We use it to update the time and date
		// and the sevenseg if countstart is set.
		if (UpdateTime)
		{
			// Reset the flag
			UpdateTime = 0;
			// Update the time
			GUI_UpdateTime();
			// If countstart is set, update the sevensegment
			if (countstart)
			{
				SEVENSEG_Set(num);
				if (num > 0)
					num--;
				else
				{
					// We reach the end of count, and we now
					// go to the OFF Screen (BLACK_SCREEN)
					countstart = 0;
					SEVENSEG_Set(10); // All off
					return BLACK_SCREEN;
				}
			}
		}

		if (KeyboardHit)
		{
			// Reset the flag
			KeyboardHit = 0;
			// Go to main screen
			return MAIN_SCREEN;
		}


		// TODO: Make the touch controller working!!!
		// Get if we have touch data
        if(XPT2046_Wait(&x, &y, 1000))
        {
        	//printf("x = %d y = %d\n", x, y);

        	// Turn off Green LED
        	GPIO_GreenLed(1);

        	// Reset counter
        	countstart = 0; num = 9;

        	// If we have a screen activity, such as touch
        	// panel hit, we reset the alarm to the next
        	// 10 seconds.
        	RTC_SetAlarmSeconds(OnRTCAlarm, 10);
        }
	}

	// We should not get here ...

	return 0;
}

void GUI_DisplayHome()
{
	int x, btn1;

    LCD_SetOrientation(2);

    // Set screen colors
    LCD_ClearScreen(Black);
    LCD_FillRect(0, 0, LCD_Width(), 100, Blue);
    LCD_FillRect(0, 101, LCD_Width(), LCD_Height(), White);
    LCD_SetFont(Arial12x12);
    LCD_Foreground(White);
    LCD_Background(Blue);

    // Draw Borders
    LCD_Rect(5,5,LCD_Width()-5,95, Yellow);
    LCD_Rect(5,106, LCD_Width()-5, LCD_Height()-5, Red);

    // Draw the button
    BUTTON_Type Button1;
    Button1.background = Blue;
    Button1.color = White;
    Button1.borderColor = Red;
    Button1.height = 30;
    Button1.font = Arial12x12;
    Button1.flags = BUTTON_FLAG_ENABLED;
    x = LCD_GetStringExtentX(EnterLabel) + 20;
    Button1.width = x;
    Button1.label = EnterLabel;
    // Center the button
    x = (LCD_Width() - Button1.width) / 2;
    Button1.x = x;
    Button1.y = 200;

    btn1 = Button_Add(&HomeButtons[0], &Button1);
    Button_DrawAll(&HomeButtons[0]);
}

void GUI_DisplayMain()
{
	int x, btn1,btn2,btn3,btn4;
    LCD_SetOrientation(2);

    // Set screen colors
    LCD_ClearScreen(Black);
    LCD_FillRect(0, 0, LCD_Width(), 150, Black);
    LCD_FillRect(0, 151, LCD_Width(), LCD_Height(), White);
    LCD_SetFont(Arial12x12);

    // Draw Borders
    LCD_Rect(5,5,LCD_Width()-5,145, Blue);
    LCD_Rect(5,156, LCD_Width()-5, LCD_Height()-5, Blue);

    LCD_Foreground(White);
    LCD_Background(Black);

    // Draw Label for the Light Sensor
    LCD_PutStr(120, 20, "Light Sensor");
    LCD_PutStr(120, 60, "Humidity");
    LCD_PutStr(120, 100, "Temperature");

    LCD_Foreground(Black);
    LCD_Background(White);

    // Draw Label for the light
    LCD_PutStr(20, 200, "Light");
    LCD_PutStr(20, 270, "Fan" );

    // Draw the button
    BUTTON_Type LightOn;
    LightOn.background = Blue;
    LightOn.color = White;
    LightOn.borderColor = Red;
    LightOn.height = 30;
    LightOn.font = Arial12x12;
    LightOn.flags = BUTTON_FLAG_ENABLED;
    x = LCD_GetStringExtentX(OnLabel) + 20;
    LightOn.width = x;
    LightOn.label = OnLabel;
    LightOn.x = 100;
    LightOn.y = 190;

    // Draw the button
    BUTTON_Type LightOff;
    LightOff.background = Blue;
    LightOff.color = White;
    LightOff.borderColor = Red;
    LightOff.height = 30;
    LightOff.font = Arial12x12;
    LightOff.flags = BUTTON_FLAG_ENABLED;
    x = LCD_GetStringExtentX(OffLabel) + 20;
    LightOff.width = x;
    LightOff.label = OffLabel;
    LightOff.x = 180;
    LightOff.y = 190;

    // Draw the button
    BUTTON_Type FanOn;
    FanOn.background = Blue;
    FanOn.color = White;
    FanOn.borderColor = Red;
    FanOn.height = 30;
    FanOn.font = Arial12x12;
    FanOn.flags = BUTTON_FLAG_ENABLED;
    x = LCD_GetStringExtentX(OnLabel) + 20;
    FanOn.width = x;
    FanOn.label = OnLabel;
    FanOn.x = 100;
    FanOn.y = 270;

    // Draw the button
    BUTTON_Type FanOff;
    FanOff.background = Blue;
    FanOff.color = White;
    FanOff.borderColor = Red;
    FanOff.height = 30;
    FanOff.font = Arial12x12;
    FanOff.flags = BUTTON_FLAG_ENABLED;
    x = LCD_GetStringExtentX(OffLabel) + 20;
    FanOff.width = x;
    FanOff.label = OffLabel;
    FanOff.x = 180;
    FanOff.y = 270;


    light_on = Button_Add(&MainButtons[0], &LightOn);
    MainButtonLayout[0][0] = light_on;	// Upper Left Button for Light On
    light_off = Button_Add(&MainButtons[0], &LightOff);
    MainButtonLayout[0][1] = light_off;  // Upper Right Button for Light Off
    fan_on = Button_Add(&MainButtons[0], &FanOn);
    MainButtonLayout[1][0] = fan_on; 	// Lower Left Button for Fan On
    fan_off = Button_Add(&MainButtons[0], &FanOff);
    MainButtonLayout[1][1] = fan_off;  // Lower Left Button for Fan Off

    Button_DrawAll(&MainButtons[0]);

}
// Returns the buttons in focus
KEY_PRESS GUI_GetKeys()
{
	int x, y;

	// Read the ADC for the current values
	x = ADC_ReadX();
	y = ADC_ReadY();

	if ((x > 1000 ) && ( x < 3000 ) &&
			( y > 1000 ) && ( y < 3000 ))
		return KEY_NONE;

	if (( x < 1000 ) && ( y > 1000 ) && ( y <  3000 ))
		return KEY_LEFT;
	else if (( x > 2000 ) && (y > 1000) && (y < 3000))
		return KEY_RIGHT;

	if (( y < 1000 ) && ( x > 1000 ) && ( x < 3000 ))
		return KEY_UP;
	else if (( y > 3000 ) && ( x > 1000 ) && ( x < 3000 ))
		return KEY_DOWN;

	return KEY_NONE;
}

void GUI_HandleButton(uint8_t buttonid)
{
	// Todo: Redraw button as pressed

	// Handle button press
	if (buttonid == light_on)
	{
		UART3_XBEESend(XBEE_BROADCAST_ADDR, "D3", 1);
    	// Turn on Blue Led
    	GPIO_BlueLed(1);
	}
	else if (buttonid == light_off)
	{
		UART3_XBEESend(XBEE_BROADCAST_ADDR, "D3", 0);
    	// Turn on Blue Led
    	GPIO_BlueLed(0);
	}
	else if (buttonid == fan_on)
	{
		UART3_XBEESend(XBEE_BROADCAST_ADDR, "D4", 1);
		GPIO_RedLed(1);
	}
	else if (buttonid == fan_off)
	{
		UART3_XBEESend(XBEE_BROADCAST_ADDR, "D4", 0);
		GPIO_RedLed(0);
	}
}

int GUI_ProcessMain()
{
	uint8_t curx, cury, x, y, num, countstart, curbutton;
	int lightstatus, fanstatus;
	KEY_PRESS key;

	// Clear flags
	UpdateTime = 0;
	UpdateTemp = 0;
	KeyboardHit = 0;
	AlarmHit = 0;

	num = 9;
	countstart = 0;

	SEVENSEG_Set(10); // Clear sevensegment

	// Set the alarm update callback for 60 seconds
	// before countdown (going to black screen)
	RTC_SetAlarmSeconds(OnRTCAlarm, 60);

	// Select the current button
	curx = 0; cury = 0;
	x = 0; y = 0;
	curbutton = MainButtonLayout[curx][cury];
	Button_Select(&MainButtons[0], curbutton, Red);
	while(1)
	{
		if (UpdateTemp)
		{
			GUI_UpdateTemp();
			UpdateTemp = 0;
		}
		if (AlarmHit)
		{
			AlarmHit = 0;
			countstart = 1;
		}
		// Every 1 sec trigger
		if (UpdateTime)
		{
			UpdateTime = 0;
			if (countstart)
			{
				SEVENSEG_Set(num);
				if (num > 0)
					num--;
				else
				{
					// We reach the end of count, and we now
					// go to the OFF Screen (BLACK_SCREEN)
					countstart = 0;
					SEVENSEG_Set(10); // All off
					return BLACK_SCREEN;
				}
			}
		}

		key = GUI_GetKeys();
		if (key != KEY_NONE )
		{
			switch(key)
			{
				case KEY_UP:
					if ( y == 1 ) y = 0;
					break;
				case KEY_DOWN:
					if ( y == 0 ) y = 1;
					break;
				case KEY_LEFT:
					if ( x == 1 ) x = 0;
					break;
				case KEY_RIGHT:
					if ( x == 0 ) x = 1;
					break;
				default:
					;// Nothing
			}

			// Check if we need to select the button
			if ((curx != x) || (cury != y))
			{
				AlarmHit = 0;
				// We have activity, reset alarms
	        	// If we have a screen activity, such as touch
	        	// panel hit, we reset the alarm to the next
	        	// 10 seconds.
	        	RTC_SetAlarmSeconds(OnRTCAlarm, 60);
				// Clear the border of the previous button
				curbutton = MainButtonLayout[curx][cury];
				// Draw white border on the current button to deselect
				Button_Select(&MainButtons[0], curbutton, White);
				// Draw border of the next selected button
				curbutton = MainButtonLayout[x][y];
				Button_Select(&MainButtons[0], curbutton, Red);

				curx = x;
				cury = y;

			}
		}
		if (KeyboardHit)
		{
			AlarmHit = 0;
			KeyboardHit = 0;
			// We have activity, reset alarms
        	// If we have a screen activity, such as touch
        	// panel hit, we reset the alarm to the next
        	// 10 seconds.
        	RTC_SetAlarmSeconds(OnRTCAlarm, 60);
			// Turn off/on devices
			GUI_HandleButton(curbutton);
		}

		// FixMe: Fix touch panel
        //if(XPT2046_Wait(&x, &y, 1000))
        //{
        	// Todo: Nothing here for the moment
        	//printf("x = %d y = %d\n", x, y);
        //}
		// Todo: process other messages here

	}
	return 0;
}
void GUI_DisplayNone()
{
    LCD_Background(Black);
    // Set screen colors
    LCD_ClearScreen();
}

int GUI_ProcessNone()
{
	while(1)
	{
		// We can return to the home screen
		// only when a keyboard is pressed.
		if (KeyboardHit)
		{
			// Clear the flags
			KeyboardHit = 0;
			return HOME_SCREEN;
		}

		// Todo:
		// Bacground processing should be done here while in
		// low power mode. We should read sensors albiet at
		// a much lower frequency. Pre-set the alarm to
		// 1 minute interval
	}

	return 0;
}
