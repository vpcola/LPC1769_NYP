#include "BUTTON.h"
#include "LCD.h"


//BUTTON_Type Buttons[BUTTON_MAX_BUTTONS];


int8_t Button_Add(BUTTON_Type * Buttons, BUTTON_Type * button) {
	uint8_t id = 0;
	while ((Buttons[id].flags & BUTTON_FLAG_USED) && (id < BUTTON_MAX_BUTTONS)) {
		id++;
	}
	
	if (id == BUTTON_MAX_BUTTONS) {
		//Max button reached
		return -1;
	}
	
	// Copy the details of the button
	// if one is available
	Buttons[id].x = button->x;
	Buttons[id].y = button->y;
	Buttons[id].width = button->width;
	Buttons[id].height = button->height;
	Buttons[id].background = button->background;
	Buttons[id].borderColor = button->borderColor;
	Buttons[id].flags = button->flags | BUTTON_FLAG_USED | BUTTON_FLAG_ENABLED;
	Buttons[id].label = button->label;
	Buttons[id].color = button->color;
	Buttons[id].font = button->font;
	Buttons[id].image = button->image;

	return id;
}

void Button_DrawAll(BUTTON_Type * Buttons) {
	uint8_t id = 0;
	for (id = 0; id < BUTTON_MAX_BUTTONS; id++) {
		if ((Buttons[id].flags & BUTTON_FLAG_USED)) {
			//Button enabled, draw it to screen
			Button_Draw(Buttons, id);
		}
	}
}

int8_t Button_Draw(BUTTON_Type * Buttons, uint8_t id) {
	int fontWidth, fontHeight, x, y, i, j;
	if ((Buttons[id].flags & BUTTON_FLAG_USED) == 0) {
		//Button not enabled
		return ERROR;
	}
	//Draw label
	if (Buttons[id].flags & BUTTON_FLAG_IMAGE) {
		//Draw picture
		for (i = 0; i < Buttons[id].width; i++) {
			for (j = 0; j < Buttons[id].height; j++) {
				LCD_Pixel(	Buttons[id].x + i,
							Buttons[id].y + j,
							*(uint16_t *)(Buttons[id].image + j * Buttons[id].width + i)
				);
			}
		}
	} else {
		//Background
		LCD_FillRect(	Buttons[id].x,
						Buttons[id].y,
						Buttons[id].x + Buttons[id].width,
						Buttons[id].y + Buttons[id].height,
						Buttons[id].background );
		
	}
	
	//Display label
	if ((Buttons[id].flags & BUTTON_FLAG_NOLABEL) == 0)
	{
		LCD_GetStringExtent(Buttons[id].label, Buttons[id].font, &fontWidth, &fontHeight);
		x = Buttons[id].x + Buttons[id].width / 2 - (fontWidth / 2);
		y = Buttons[id].y + Buttons[id].height / 2 - (fontHeight / 2);
		LCD_Locate(x, y);
		LCD_Foreground(Buttons[id].color);
		LCD_Background(Buttons[id].background);
		LCD_SetFont(Buttons[id].font);
		LCD_PutString(Buttons[id].label);
	}
	
	//Border
	if ((Buttons[id].flags & BUTTON_FLAG_NOBORDER) == 0) {
		//Border enabled
		LCD_Rect(	Buttons[id].x,
					Buttons[id].y,
					Buttons[id].x + Buttons[id].width,
					Buttons[id].y + Buttons[id].height,
					Buttons[id].borderColor );
	}
	
	return SUCCESS;
}

void Button_Select(BUTTON_Type * Buttons, uint8_t id,  int color)
{
	//Border enabled
	LCD_Rect(	Buttons[id].x - 10,
			Buttons[id].y - 10,
			Buttons[id].x + Buttons[id].width + 10,
			Buttons[id].y + Buttons[id].height + 10,
			color );
}

int8_t Button_Touch(BUTTON_Type * Buttons, TOUCHDATA_Type * TouchData)
{
	uint8_t id;
	for (id = 0; id < BUTTON_MAX_BUTTONS; id++) {
		//If button not enabled, ignore it
		if ((Buttons[id].flags & BUTTON_FLAG_ENABLED) == 0) {
			continue;
		}
		//If touch data is inside button somewhere
		if (
			(TouchData->x > Buttons[id].x && TouchData->x < (Buttons[id].x + Buttons[id].width)) &&
			(TouchData->y > Buttons[id].y && TouchData->y < (Buttons[id].y + Buttons[id].height))
		)
		{
			//Return its id
			return id;
		}
	}
	
	//No one was pressed
	return -1;
}


void Button_Enable(BUTTON_Type * Buttons, uint8_t id) {
	//Add enabled flag
	Buttons[id].flags |= BUTTON_FLAG_ENABLED;
}

void Button_Disable(BUTTON_Type * Buttons, uint8_t id) {
	//Remove enabled flag
	Buttons[id].flags &= ~BUTTON_FLAG_ENABLED;
}

void Button_DeleteAll(BUTTON_Type * Buttons) {
	uint8_t i;
	for (i = 0; i < BUTTON_MAX_BUTTONS; i++) {
		Button_Delete(Buttons, i);
	}
}

void Button_Delete(BUTTON_Type * Buttons, uint8_t id) {
	//Just remove USED flag from button
	Buttons[id].flags &= ~BUTTON_FLAG_USED;
}

