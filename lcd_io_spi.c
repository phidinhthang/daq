#include "main.h"
#include "lcd_io_spi.h"

extern inline void LcdWrite8(uint8_t d8);
inline void LcdWrite8(uint8_t d8) {
  SPIX->DR = d8;
  LCD_IO_Delay(2);
  while (SPIX->SR & SPI_SR_BSY);
}

extern inline void LcdCmdWrite8(uint8_t cmd8);
inline void LcdCmdWrite8(uint8_t cmd8) {
  LCD_RS_CMD;
  SPIX->DR = cmd8;
  LCD_IO_Delay(2);
	while (SPIX->SR & SPI_SR_BSY);
  LCD_RS_DATA;
}

extern inline void LcdWrite16(uint16_t d16);
inline void LcdWrite16(uint16_t d16) {
  SPIX->DR = d16;
  LCD_IO_Delay(1);
	while (SPIX->SR & SPI_SR_BSY);
}

extern inline void LcdCmdWrite16(uint16_t cmd16);
inline void LcdCmdWrite16(uint16_t cmd16) {
  LCD_RS_CMD;
  SPIX->DR = cmd16;
  LCD_IO_Delay(2);
	while (SPIX->SR & SPI_SR_BSY);
  LCD_RS_DATA;
}

void LCD_IO_WriteMultiData8(uint8_t * pData, uint32_t Size, uint32_t dinc) {
  while(Size--) {
    LcdWrite8(*pData);
    if(dinc)
      pData++;
  }
  LCD_CS_OFF;
}

//-----------------------------------------------------------------------------
void LCD_IO_WriteMultiData16(uint16_t * pData, uint32_t Size, uint32_t dinc) {
  while(Size--) {
    LcdWrite16(*pData);
    if(dinc)
      pData++;
  }
  LCD_CS_OFF;
}

void LCD_IO_Delay(uint32_t c) {
  while(c--);
}

void LCD_Delay(uint32_t Delay) {
  Delay_ms(Delay);
}

void LCD_IO_Init(void) {
	GPIOB->CRH &= ~(0xF << 8);
	GPIOB->CRH |= 2 << 8; // RST PB10 output push pull 2Mhz
	
  LCD_RST_OFF;
	LCD_RS_DATA;
  LCD_CS_OFF;
  
	GPIOB->CRH &= ~(0xF << 12);
	GPIOB->CRH |= 3 << 12; // RS PB11 output push pull 2Mhz
  
	GPIOB->CRL &= ~(0xF << 4);
	GPIOB->CRL |= 3 << 4; // CS PB1 output push pull 50Mhz

	GPIOA->CRL |= 1 << 5; // SCK = 1
	
	RCC->APB2ENR |= 1 << 12; // enable SPI1

	GPIOA->CRL &= ~(0xF << 20);
	GPIOA->CRL |= 11 << 20; // SPI1 SCK PA5 output af push pull 50Mhz
  
	GPIOA->CRL &= ~(0xF << 28);
	GPIOA->CRL |= 11 << 28; // SPI1 MOSI PA7 output af push pull 50Mhz
	
	SPIX->CR1 = SPI_CR1_CPHA | SPI_CR1_CPOL | SPI_CR1_MSTR | SPI_CR1_SPE | SPI_CR1_SSM | SPI_CR1_SSI | (LCD_SPI_SPD_WRITE << 3);
	 
	SPIX->CR1 |= SPI_CR1_SPE;
	 
	 LCD_Delay(10);
  LCD_RST_ON;
  LCD_Delay(10);
  LCD_RST_OFF;
	
	LCD_Delay(10);
}

void LCD_IO_WriteCmd8(uint8_t Cmd) {
  LcdSpiMode8();
  LCD_CS_ON;
  LcdCmdWrite8(Cmd);
  LCD_CS_OFF;
}

void LCD_IO_WriteCmd16(uint16_t Cmd) {
  LcdSpiMode16();
  LCD_CS_ON;
  LcdCmdWrite16(Cmd);
  LCD_CS_OFF;
}

void LCD_IO_WriteData8(uint8_t Data) {
  LcdSpiMode8();
  LCD_CS_ON;
  LcdWrite8(Data);
  LCD_CS_OFF;
}

void LCD_IO_WriteData16(uint16_t Data) {
  LcdSpiMode16();
  LCD_CS_ON;
  LcdWrite16(Data);
  LCD_CS_OFF;
}

void LCD_IO_WriteCmd8DataFill16(uint8_t Cmd, uint16_t Data, uint32_t Size) {
  LcdSpiMode8();
  LCD_CS_ON;
  LcdCmdWrite8(Cmd);
  LcdSpiMode16();
  LCD_IO_WriteMultiData16(&Data, Size, 0);
}

void LCD_IO_WriteCmd8MultipleData8(uint8_t Cmd, uint8_t *pData, uint32_t Size) {
  LcdSpiMode8();
  LCD_CS_ON;
  LcdCmdWrite8(Cmd);
  LCD_IO_WriteMultiData8(pData, Size, 1);
}

void LCD_IO_WriteCmd8MultipleData16(uint8_t Cmd, uint16_t *pData, uint32_t Size) {
  LcdSpiMode8();
  LCD_CS_ON;
  LcdCmdWrite8(Cmd);
  LcdSpiMode16();
  LCD_IO_WriteMultiData16(pData, Size, 1);
}

void LCD_IO_WriteCmd16DataFill16(uint16_t Cmd, uint16_t Data, uint32_t Size) {
  LcdSpiMode16();
  LCD_CS_ON;
  LcdCmdWrite16(Cmd);
  LCD_IO_WriteMultiData16(&Data, Size, 0);
}

void LCD_IO_WriteCmd16MultipleData8(uint16_t Cmd, uint8_t *pData, uint32_t Size) {
  LcdSpiMode16();
  LCD_CS_ON;
  LcdCmdWrite16(Cmd);
  LcdSpiMode8();
  LCD_IO_WriteMultiData8(pData, Size, 1);
}

void LCD_IO_WriteCmd16MultipleData16(uint16_t Cmd, uint16_t *pData, uint32_t Size) {
  LcdSpiMode16();
  LCD_CS_ON;
  LcdCmdWrite16(Cmd);
  LCD_IO_WriteMultiData16(pData, Size, 1);
}

