/*
 * ECE 153B
 * 
 * Name(s): Aden, Vishal
 * Section: Tuesday 6pm
 * Lab: 3B
 */

#include "stm32l476xx.h"

void LED_Pin_Init(void);
void TIM2_CH1_Init(void);
void SERVO_Pin_Init(void);
void TIM5_CH1_Init(void);

void LED_Pin_Init(void){
	 // Enable GPIO Clocks
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	
	// Initialize Green LED
	GPIOA->MODER &= ~(GPIO_MODER_MODE5); //Clear the mode register
	GPIOA->MODER  |= GPIO_MODER_MODE5_1; //Set to Alternative Function
	
	//Set I/O output speed value as very high speed
	GPIOA->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5);	
	
	// GPIO Push-Pull: No pull-up, no pull-down
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5); //Set pull-up/pull-down to no pull-up/pull-down
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT5); //Set output type to push-pull
	
	//configure and selct alternative function for PA5 (double check)
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL5_0);
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;
	
	
	
	

}
	

void TIM2_CH1_Init(void){
	// Enable the timer clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	// Counting direction: 0 = up-counting, 1 = down-counting
	TIM2->CR1 &= ~(TIM_CR1_DIR);
	
  // Prescaler
	
  // Auto-reload
	TIM2->CR1 |= TIM_CR1_ARPE;

	// Disable output compare mode
	TIM2->CCMR1
	
	// PWM mode 1
	// Output 1 preload enable
	// Select output polarity: active high
  // Enable output for ch1
	// Output Compare Register for channel 1 
	// Enable counter
}

void SERVO_Pin_Init(void){
	// Set mode as Alternative Function 1
	//Set I/O output speed value as very high speed
	//Set I/O as no pull-up pull-down 
}

void TIM5_CH1_Init(void){
	// Enable the timer clock
	// Counting direction: 0 = up-counting, 1 = down-counting
  // Prescaler
  // Auto-reload
	// Disable output compare mode
	// PWM mode 1
	// Output 1 preload enable
	// Select output polarity: active high
  // Enable output for ch1
	// Output Compare Register for channel 1 
	// Enable counter
}

int main(void) {
	int i;
	int led = 0;
	int dire = 1;
	
	LED_Pin_Init();
	SERVO_Pin_Init();
	TIM2_CH1_Init();
	TIM5_CH1_Init();
		
    while(1) {
		// TODO
		TIM2->CCR1 = 0; //TODO
		TIM5->CCR1 = 0;//TODO
		
		for(i=0;i<300;i++);  		// delay
    }
}
