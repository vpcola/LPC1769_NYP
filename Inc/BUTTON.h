/*
 * BUTTON.h
 *
 *  Created on: 27 May, 2016
 *      Author: tkcov
 */

#ifndef INC_BUTTON_H_
#define INC_BUTTON_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include "lpc17xx_pinsel.h"

#define BUTTON_MAX_BUTTONS	10

//Button flags
#define BUTTON_FLAG_ENABLED		0x0001	// Button is enabled - internal flag
#define BUTTON_FLAG_NOBORDER	0x0002	// No border used in
#define BUTTON_FLAG_IMAGE		0x0004	// Use image for background
#define BUTTON_FLAG_NOLABEL		0x0008	// Do not display button label
#define BUTTON_FLAG_USED		0x8000	// Button is used - internal flag

//Button status
#define BUTTON_STATUS_ON	1
#define BUTTON_STATUS_OFF	0

/**
 * Button options struct
 *
 * Parameters:
 *	- uint16_t x: X coordinate of top left button corner
 *	- uint16_t y: Y coordinate of top left button corner
 *	- uint16_t width: button width in pixels
 *	- uint16_t height: button height in pixels
 *	- uint16_t background: 16bit background color
 *	- uint16_t borderColor: 16bit border color
 *	- uint16_t flags: Button flags
 *	- char *label: pointer to first character of label
 *	- unsigned char * font: pointer to font structure
 *	- uint16_t color: label color
 *	- uint16_t *image: pointer to location at first color for pixel
 */
typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t width;
	uint16_t height;
	uint16_t background;
	uint16_t borderColor;
	uint16_t flags;
	char* label;
	const unsigned char * font;
	uint16_t color;
	uint16_t status;
	uint16_t* image;
} BUTTON_Type;

typedef struct{
	uint16_t x;
	uint16_t y;
} TOUCHDATA_Type;


int8_t Button_Add(BUTTON_Type * Buttons, BUTTON_Type * button);
int8_t Button_Draw(BUTTON_Type * Buttons, uint8_t button_id);
void Button_DrawAll(BUTTON_Type * Buttons);
void Button_Enable(BUTTON_Type * Buttons, uint8_t id);
void Button_Select(BUTTON_Type * Buttons, uint8_t id, int color);
void Button_Disable(BUTTON_Type * Buttons, uint8_t id);
void Button_DeleteAll(BUTTON_Type * Buttons);
void Button_Delete(BUTTON_Type * Buttons, uint8_t id);
int8_t Button_Touch(BUTTON_Type * Buttons, TOUCHDATA_Type * TouchData);

#ifdef __cplusplus
}
#endif

#endif /* INC_BUTTON_H_ */
