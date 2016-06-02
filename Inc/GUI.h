/*
 * GUI.h
 *
 *  Created on: 30 May, 2016
 *      Author: tkcov
 */

#ifndef INC_GUI_H_
#define INC_GUI_H_

#ifdef __cplusplus
extern "C" {
#endif

int GUI_ProcessMessages();

void GUI_DisplayHome();
void GUI_DisplayMain();
void GUI_DisplayNone();
int GUI_ProcessHome();
int GUI_ProcessMain();
int GUI_ProcessNone();

#ifdef __cplusplus
}
#endif


#endif /* INC_GUI_H_ */
