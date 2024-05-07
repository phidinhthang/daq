#include "ili9488.h"

static uint16_t yStart, yEnd;

#define  LCD_IO_WriteData16_to_2x8(dt)    {LCD_IO_WriteData8((dt) >> 8); LCD_IO_WriteData8(dt); }

void ILI9488_Init(void) {
  LCD_IO_Init();

  LCD_Delay(105);
  LCD_IO_WriteCmd8(ILI9488_SWRESET);
  LCD_Delay(5);
  // positive gamma control
  LCD_IO_WriteCmd8MultipleData8(ILI9488_GMCTRP1, (uint8_t *)"\x00\x03\x09\x08\x16\x0A\x3F\x78\x4C\x09\x0A\x08\x16\x1A\x0F", 15);
  // negative gamma control
  LCD_IO_WriteCmd8MultipleData8(ILI9488_GMCTRN1, (uint8_t *)"\x00\x16\x19\x03\x0F\x05\x32\x45\x46\x04\x0E\x0D\x35\x37\x0F", 15);
  // Power Control 1 (Vreg1out, Verg2out)
  LCD_IO_WriteCmd8MultipleData8(ILI9488_PWCTR1, (uint8_t *)"\x17\x15", 2);
  LCD_Delay(5);
  // Power Control 2 (VGH,VGL)
  LCD_IO_WriteCmd8(ILI9488_PWCTR2); LCD_IO_WriteData8(0x41);
  LCD_Delay(5);
  // Power Control 3 (Vcom)
  LCD_IO_WriteCmd8MultipleData8(ILI9488_VMCTR1, (uint8_t *)"\x00\x12\x80", 3);
  LCD_Delay(5);

  LCD_IO_WriteCmd8(ILI9488_PIXFMT); LCD_IO_WriteData8(0x66); // Interface Pixel Format (24 bit)

  // LCD_IO_WriteCmd8(0xFB); LCD_IO_WriteData8(0x80);
  LCD_IO_WriteCmd8(ILI9488_IMCTR); LCD_IO_WriteData8(0x80); // Interface Mode Control (SDO NOT USE)
  
  LCD_IO_WriteCmd8(ILI9488_FRMCTR1); LCD_IO_WriteData8(0xA0); // Frame rate (60Hz)
  LCD_IO_WriteCmd8(ILI9488_INVCTR); LCD_IO_WriteData8(0x02); // Display Inversion Control (2-dot)
  LCD_IO_WriteCmd8MultipleData8(ILI9488_DFUNCTR, (uint8_t *)"\x02\x02", 2); // Display Function Control RGB/MCU Interface Control
  LCD_IO_WriteCmd8(ILI9488_IMGFUNCT); LCD_IO_WriteData8(0x00); // Set Image Functio (Disable 24 bit data)
  LCD_IO_WriteCmd8MultipleData8(ILI9488_ADJCTR3, (uint8_t *)"\xA9\x51\x2C\x82", 4); // Adjust Control (D7 stream, loose)
  LCD_Delay(5);
  LCD_IO_WriteCmd8(ILI9488_SLPOUT);      // Exit Sleep
  LCD_Delay(120);
  LCD_IO_WriteCmd8(ILI9488_DISPON);      // Display on
  LCD_Delay(5);
  LCD_IO_WriteCmd8(ILI9488_MADCTL); LCD_IO_WriteData8(ILI9488_MAD_DATA_RIGHT_THEN_DOWN);
}

void ILI9488_Display_On(void) {
  LCD_IO_WriteCmd8(ILI9488_SLPOUT);      // Display on
}

void ILI9488_Display_Off(void) {
  LCD_IO_WriteCmd8(ILI9488_SLPIN);       // Display off
}

void ILI9488_Set_Cursor(uint16_t Xpos, uint16_t Ypos) {
  ILI9488_SETCURSOR(Xpos, Ypos);
}

extern inline void ili9488_write16to24(uint16_t RGBCode);
inline void ili9488_write16to24(uint16_t RGBCode) {
  LCD_IO_WriteData8((RGBCode & 0xF800) >> 8);
  LCD_IO_WriteData8((RGBCode & 0x07E0) >> 3);
  LCD_IO_WriteData8((RGBCode & 0x001F) << 3);
}

void ILI9488_Write_Pixel(uint16_t Xpos, uint16_t Ypos, uint16_t RGBCode) {
  ILI9488_SETCURSOR(Xpos, Ypos)
  
  LCD_IO_WriteCmd8(ILI9488_RAMWR);
  ili9488_write16to24(RGBCode);
  
}

void ILI9488_Set_Display_Window(uint16_t Xpos, uint16_t Ypos, uint16_t Width, uint16_t Height) {
	yStart = Ypos; yEnd = Ypos + Height - 1;
  LCD_IO_WriteCmd8(ILI9488_CASET); LCD_IO_WriteData16_to_2x8(Xpos) LCD_IO_WriteData16_to_2x8(Xpos + Width - 1)
  LCD_IO_WriteCmd8(ILI9488_PASET); LCD_IO_WriteData16_to_2x8(Ypos) LCD_IO_WriteData16_to_2x8(Ypos + Height - 1)
}

void ILI9488_Fill_Rect(uint16_t Xpos, uint16_t Ypos, uint16_t Xsize, uint16_t Ysize, uint16_t RGBCode) {
  ILI9488_Set_Display_Window(Xpos, Ypos, Xsize, Ysize);

  LCD_IO_WriteCmd8(ILI9488_RAMWR);
  uint32_t XYsize = Xsize * Ysize;
  while(XYsize--)
    ili9488_write16to24(RGBCode);
}


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