/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 3C
 */
 
#include <stdio.h> 
 
#include "stm32l476xx.h"

uint32_t volatile currentValue = 0;
uint32_t volatile lastValue = 0;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;

void Input_Capture_Setup() {
	// [TODO]
}

void TIM4_IRQHandler(void) {
	// [TODO]
}

void Trigger_Setup() {
	//Setup PA9
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //Enable clock for GPIO Pin A
	//Configure PA9 to be used as alternative function TIM1_CH2
	GPIOA->MODER &= ~(GPIO_MODER_MODE9); //Clear the mode register
	GPIOA->MODER |= GPIO_MODER_MODE9_1; //Set to Alternative Function
	GPIOA->AFR[0] &= ~(GPIO_AFRH_AFSEL9); //configure and select alternative function for PA5 
	GPIOA->AFR[0] |= GPIO_AFRH_AFSEL9_0; 
	//Set PA9 to no pull-up, no pull-down
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD9); //Set pull-up/pull-down to no pull-up/pull-down
	//Set the output type of PA9 to push-pull
	GPIOA->OTYPER &= GPIO_OTYPER_OT9; 
	//Set PA9 to very high output speed
	GPIOA->OSPEEDR |= GPIO_OSPEEDR_OSPEED9;
	
	//Enable Timer 1 in RCC APB2ENR
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
	//Set the prescaler to 15
	TIM1->PSC = 15;
	
	//Enable auto reload preload in the control register and set the auto reload value to its
	//maximum value.
	TIM1->CR1 |= TIM_CR1_ARPE; 
	TIM1->ARR = 0XFFFF; 

	//Set the CCR value that will trigger the sensor
	TIM1->CCR2 = 10; //Double check and come back to this
	
	//In the capture/compare mode register, set the output compare mode such that the timer
	//operates in PWM Mode 1 and enable output compare preload.
	TIM1->CCMR1 &= ~(TIM_CCMR1_OC1M); 
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_1; //Set the output compare mode bits to PWM mode 1
	TIM1->CCMR1 |= TIM_CCMR1_OC1M_2;  //Make sure this is the right mode register
	TIM1->CCMR1 &= TIM_CCMR1_OC1PE;  //Enable output compare preload
	
	//Enable the output in the capture/compare enable register. (Assuming CC1, check with TA)
	TIM1->CCER	|= TIM_CCER_CC1E;
	
	//In the break and dead-time register, set the bits (at the same time) for main output
	//enable, off-state selection for run mode, and off-state selection for idle mode.
	
	
	

}

int main(void) {	
	// Enable High Speed Internal Clock (HSI = 16 MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI is ready
	
	// Select HSI as system clock source 
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) == 0); // Wait until HSI is system clock source
  
	// Input Capture Setup
	Input_Capture_Setup();
	
	// Trigger Setup
	Trigger_Setup();

	
	while(1) {
		// [TODO] Store your measurements on Stack
		
	}
}
