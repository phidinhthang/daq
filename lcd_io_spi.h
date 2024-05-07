#ifndef LCD_IO_SPI_H
#define LCD_IO_SPI_H

#include <stdlib.h>
#include "main.h"

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
