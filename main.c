#include "main.h"
#include "ili9488.h"

void Delay_ms(uint32_t ms) {
	for (uint32_t i = 0; i < ms; i++) {
		TIM2->EGR |= 1 << 0;
		while ((TIM2->CNT) < 1000);
	}
}

void TIM_Init(void) {
	RCC->APB1ENR |= 1 << 0;

	TIM2->CR1 &= ~(1 << 0);
	TIM2->PSC = 71;
	TIM2->ARR = 0xFFFF;
	TIM2->CR1 |= 1 << 0;

}

int main(void) {
	RCC->APB2ENR |= 1 << 2;
	RCC->APB2ENR |= 1 << 3;
	RCC->APB2ENR |= 1 << 4;
	RCC->APB2ENR |= 1 << 0;
	

	TIM_Init();
	
	ILI9488_Init();
	
	ILI9488_Display_On();
	
	ILI9488_Fill_Rect(0, 0, ILI9488_LCD_PIXEL_WIDTH / 2 , ILI9488_LCD_PIXEL_HEIGHT, 0xFF00);
	
	ILI9488_Fill_Rect(0, 0, ILI9488_LCD_PIXEL_WIDTH / 2 , ILI9488_LCD_PIXEL_HEIGHT, 0xA312);
	
	for (int i = 0; i < ILI9488_LCD_PIXEL_WIDTH; i++) {
		ILI9488_Write_Pixel(i, ILI9488_LCD_PIXEL_HEIGHT / 2, 0xC134);
	}
	while (1) {
		
	}
}