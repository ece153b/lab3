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
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED5;	
	
	// GPIO Push-Pull: No pull-up, no pull-down
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD5); //Set pull-up/pull-down to no pull-up/pull-down
	
	//configure and select alternative function for PA5 
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL5_0);
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;
	
	
	
	

}
	

void TIM2_CH1_Init(void){
	// Enable the timer clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	// Counting direction: 0 = up-counting, 1 = down-counting
	TIM2->CR1 &= ~(TIM_CR1_DIR);
	
  // Prescaler should be 79
	TIM2->PSC = 79; 
	
  // Auto-reload
	TIM2->ARR = 999; 

	// Disable output compare mode

	TIM2->CCMR1 &= ~(TIM_CCMR1_OC1M); //clear the output compare mode bits

	
	// PWM mode 1
		TIM2->CCMR1 |= TIM_CCMR1_OC1M_1; //Set the output compare mode bits to PWM mode 1
		TIM2->CCMR1 |= TIM_CCMR1_OC1M_2; 
	// Output 1 preload enable
		TIM2->CCMR1 |= TIM_CCMR1_OC1PE; //Enable output preload
	// Select output polarity: active high
		TIM2->CCER &= ~(TIM_CCER_CC1P);
  // Enable output for ch1
		TIM2->CCER |= TIM_CCER_CC1E;
	// Output Compare Register for channel 1 
		TIM2->CCR1 = 500; 
	// Enable counter
		TIM2->CR1 |= TIM_CR1_CEN; 
}

void SERVO_Pin_Init(void){
		RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// Set mode as Alternative Function 1
		GPIOA->MODER &= ~(GPIO_MODER_MODE0); //Clear the mode register
		GPIOA->MODER  |= GPIO_MODER_MODE0_1; //Set to Alternative Function

	//Set I/O output speed value as very high speed
	//GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED0;
	//Set I/O as no pull-up pull-down 
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD0); //Set pull-up/pull-down to no pull-up/pull-down
	
	//configure and select alternative function for PA5 
	GPIOA->AFR[0] &= ~(GPIO_AFRL_AFSEL0_0);
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_1;
}

void TIM5_CH1_Init(void){
	// Enable the timer clock
		RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
	// Counting direction: 0 = up-counting, 1 = down-counting
		TIM5->CR1 &= ~(TIM_CR1_DIR);
  // Prescaler
		TIM5->PSC = 79; 
  // Auto-reload
		TIM5->ARR = 999; 
	// Disable output compare mode
		TIM5->CCMR1 &= ~(TIM_CCMR1_OC1M); //clear the output compare mode bits
	// PWM mode 1
		TIM5->CCMR1 |= TIM_CCMR1_OC1M_1; //Set the output compare mode bits to PWM mode 1
		TIM5->CCMR1 |= TIM_CCMR1_OC1M_2; 
	// Output 1 preload enable
		TIM5->CCMR1 |= TIM_CCMR1_OC1PE; //Enable output preload
	// Select output polarity: active high
		TIM5->CCER &= ~(TIM_CCER_CC1P);
  // Enable output for ch1
		TIM5->CCER |= TIM_CCER_CC1E;
	// Output Compare Register for channel 1 
		TIM5->CCR1 = 500; 
	// Enable counter
		TIM5->CR1 |= TIM_CR1_CEN;
}

int main(void) {
	int i;
	int led = 1;
	int dire = 1;
	
	LED_Pin_Init();
	SERVO_Pin_Init();
	TIM2_CH1_Init();
	TIM5_CH1_Init();
		
    while(1) {
		// TODO
			
		if(led)
		{
			TIM2->CCR1++;
			if(TIM2->CCR1 == TIM2->ARR)
			{
				led = 0; 
			}
		}
		else
		{
			TIM2->CCR1--;
			if(TIM2->CCR1 == 0)
			{
				led = 1; 
			}
		}
		
		if(dire)
		{
			TIM5->CCR1++;
			if(TIM5->CCR1 == TIM5->ARR)
			{
				dire = 0; 
			}
		}
		else
		{
			TIM5->CCR1--;
			if(TIM5->CCR1 == 0)
			{
				dire = 1; 
			}
		}
		
		for(i=0;i<300;i++);  		// delay
    }
}
