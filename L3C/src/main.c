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
