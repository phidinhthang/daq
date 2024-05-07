#ifndef LCD_H
#define LCD_H

#include <stdint.h>

#define LCD_REVERSE16	0


typedef struct {
	void (*ILI9488_Init)(void);
	void (*ILI9488_Display_On)(void);
	void (*ILI9488_Display_Off)(void);
	void (*ILI9488_Set_Cursor)(uint16_t, uint16_t);
	void (*ILI9488_Write_Pixel)(uint16_t, uint16_t, uint16_t);
	void (*ILI9488_Set_Display_Window)(uint16_t, uint16_t, uint16_t, uint16_t);
	void (*ILI9488_Fill_Rect)(uint16_t, uint16_t, uint16_t, uint16_t, uint16_t);
} LCD_DrvTypeDef;

#endif

