#ifndef ILI9488_H
#define ILI9488_H

#include <stdlib.h>
#include "main.h"

#define ILI9488_INTERFACE			0

#define ILI9488_ORIENTATION		0

#define ILI9488_COLORMODE			0

#define ILI9488_TOUCH					0

#define  ILI9488_LCD_PIXEL_WIDTH   320
#define  ILI9488_LCD_PIXEL_HEIGHT  480

#define LCD_SPI		1
#define LCD_SPI_SPD_WRITE	1

/* Command/data pin set */
#define LCD_RS_CMD		GPIOB->ODR &= ~(1 << 11)
#define LCD_RS_DATA		GPIOB->ODR |= 1 << 11

/* Reset pin set */
#define LCD_RST_ON		GPIOB->ODR &= ~(1 << 10)
#define LCD_RST_OFF		GPIOB->ODR |= 1 << 10

/* Chip select pin set */
#define LCD_CS_ON		GPIOB->ODR &= ~(1 << 1)
#define LCD_CS_OFF	GPIOB->ODR |= 1 << 1

#define SPIX					SPI1

#define LCD_SPI_RCC_EN				RCC->APB2ENR |= 1 << 12

#define LcdSpiMode8() 				SPIX->CR1 &= ~(1 << 11)
#define LcdSpiMode16()				SPIX->CR1 |= 1 << 11


#define ILI9488_NOP           0x00
#define ILI9488_SWRESET       0x01
#define ILI9488_RDDID         0x04
#define ILI9488_RDDST         0x09

#define ILI9488_SLPIN         0x10
#define ILI9488_SLPOUT        0x11
#define ILI9488_PTLON         0x12
#define ILI9488_NORON         0x13

#define ILI9488_RDMODE        0x0A
#define ILI9488_RDMADCTL      0x0B
#define ILI9488_RDPIXFMT      0x0C
#define ILI9488_RDIMGFMT      0x0D
#define ILI9488_RDSELFDIAG    0x0F

#define ILI9488_INVOFF        0x20
#define ILI9488_INVON         0x21
#define ILI9488_GAMMASET      0x26
#define ILI9488_DISPOFF       0x28
#define ILI9488_DISPON        0x29

#define ILI9488_CASET         0x2A
#define ILI9488_PASET         0x2B
#define ILI9488_RAMWR         0x2C
#define ILI9488_RAMRD         0x2E

#define ILI9488_PTLAR         0x30
#define ILI9488_VSCRDEF       0x33
#define ILI9488_MADCTL        0x36
#define ILI9488_VSCRSADD      0x37
#define ILI9488_PIXFMT        0x3A
#define ILI9488_RAMWRCONT     0x3C
#define ILI9488_RAMRDCONT     0x3E

#define ILI9488_IMCTR         0xB0
#define ILI9488_FRMCTR1       0xB1
#define ILI9488_FRMCTR2       0xB2
#define ILI9488_FRMCTR3       0xB3
#define ILI9488_INVCTR        0xB4
#define ILI9488_DFUNCTR       0xB6

#define ILI9488_PWCTR1        0xC0
#define ILI9488_PWCTR2        0xC1
#define ILI9488_PWCTR3        0xC2
#define ILI9488_PWCTR4        0xC3
#define ILI9488_PWCTR5        0xC4
#define ILI9488_VMCTR1        0xC5
#define ILI9488_VMCTR2        0xC7

#define ILI9488_RDID1         0xDA
#define ILI9488_RDID2         0xDB
#define ILI9488_RDID3         0xDC
#define ILI9488_RDID4         0xDD

#define ILI9488_GMCTRP1       0xE0
#define ILI9488_GMCTRN1       0xE1
#define ILI9488_IMGFUNCT      0xE9

#define ILI9488_ADJCTR3       0xF7

#define ILI9488_MAD_RGB       0x08
#define ILI9488_MAD_BGR       0x00

#define ILI9488_MAD_VERTICAL  0x20
#define ILI9488_MAD_X_LEFT    0x00
#define ILI9488_MAD_X_RIGHT   0x40
#define ILI9488_MAD_Y_UP      0x80
#define ILI9488_MAD_Y_DOWN    0x00

#define ILI9488_MAD_COLORMODE	ILI9488_MAD_RGB

#define LCD_ORIENTATION				ILI9488_ORIENTATION

#define ILI9488_SETCURSOR(x, y)            {LCD_IO_WriteCmd8(ILI9488_CASET); LCD_IO_WriteData16_to_2x8(x); LCD_IO_WriteData16_to_2x8(x); LCD_IO_WriteCmd8(ILI9488_PASET); LCD_IO_WriteData16_to_2x8(y); LCD_IO_WriteData16_to_2x8(y);}

#define ILI9488_MAX_X                      (ILI9488_LCD_PIXEL_WIDTH - 1)
#define ILI9488_MAX_Y                      (ILI9488_LCD_PIXEL_HEIGHT - 1)
#define ILI9488_MAD_DATA_RIGHT_THEN_UP     ILI9488_MAD_COLORMODE | ILI9488_MAD_X_RIGHT | ILI9488_MAD_Y_UP
#define ILI9488_MAD_DATA_RIGHT_THEN_DOWN   ILI9488_MAD_COLORMODE | ILI9488_MAD_X_RIGHT | ILI9488_MAD_Y_DOWN

#define ILI9488_LCD_INITIALIZED    0x01
#define ILI9488_IO_INITIALIZED     0x02


void ILI9488_Init(void);
void ILI9488_Display_On(void);
void ILI9488_Display_Off(void);
void ILI9488_Set_Cursor(uint16_t Xpos, uint16_t YPos);
void ILI9488_Write_Pixel(uint16_t Xpos, uint16_t YPos, uint16_t RGB_Code);
void ILI9488_Set_Display_Window(uint16_t XPos, uint16_t YPos, uint16_t Width, uint16_t Height);
void ILI9488_Fill_Rect(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint16_t RGBCode);


/*
	LCD_RST			B10
	LCD_RS			B11
	LCD_CS			B1
	LCD_SCK			A5
	LCD_MOSI		A7
	LCD_MISO		A6
*/

void  LCD_Delay (uint32_t delay);
void  LCD_IO_Init(void);

void  LCD_IO_WriteCmd8(uint8_t Cmd);
void  LCD_IO_WriteCmd16(uint16_t Cmd);
void  LCD_IO_WriteData8(uint8_t Data);
void  LCD_IO_WriteData16(uint16_t Data);

void  LCD_IO_WriteCmd8DataFill16(uint8_t Cmd, uint16_t Data, uint32_t Size);
void  LCD_IO_WriteCmd8MultipleData8(uint8_t Cmd, uint8_t *pData, uint32_t Size);
void  LCD_IO_WriteCmd8MultipleData16(uint8_t Cmd, uint16_t *pData, uint32_t Size);
void  LCD_IO_WriteCmd16DataFill16(uint16_t Cmd, uint16_t Data, uint32_t Size);
void  LCD_IO_WriteCmd16MultipleData8(uint16_t Cmd, uint8_t *pData, uint32_t Size);
void  LCD_IO_WriteCmd16MultipleData16(uint16_t Cmd, uint16_t *pData, uint32_t Size);

void  LCD_IO_ReadCmd8MultipleData8(uint8_t Cmd, uint8_t *pData, uint32_t Size, uint32_t DummySize);
void  LCD_IO_ReadCmd8MultipleData16(uint8_t Cmd, uint16_t *pData, uint32_t Size, uint32_t DummySize);
void  LCD_IO_ReadCmd8MultipleData24to16(uint8_t Cmd, uint16_t *pData, uint32_t Size, uint32_t DummySize);
void  LCD_IO_ReadCmd16MultipleData8(uint16_t Cmd, uint8_t *pData, uint32_t Size, uint32_t DummySize);
void  LCD_IO_ReadCmd16MultipleData16(uint16_t Cmd, uint16_t *pData, uint32_t Size, uint32_t DummySize);
void  LCD_IO_ReadCmd16MultipleData24to16(uint16_t Cmd, uint16_t *pData, uint32_t Size, uint32_t DummySize);

void LCD_IO_WriteMultiData8(uint8_t * pData, uint32_t Size, uint32_t dinc);
void LCD_IO_WriteMultiData16(uint16_t * pData, uint32_t Size, uint32_t dinc);

void  LCD_IO_Delay(uint32_t c);
#endif
