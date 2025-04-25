/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 3C
 */
 
#include <stdio.h> 
 
#include "stm32l476xx.h"

#define MAX_TIMER 4294967295 //2^32 - 1

uint32_t volatile currentValue = 0;
uint32_t volatile lastValue = 0;
uint32_t volatile overflowCount = 0;
uint32_t volatile timeInterval = 0;
uint32_t distance = 0;
void Input_Capture_Setup() {
	//Set up PB6
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN; //Enable the clock for GPIO Port B
	//Configure PB6 to be used as alternative function TIM4 CH1
	GPIOB->MODER &= ~(GPIO_MODER_MODE6); //Clear the mode register
	GPIOB->MODER |= GPIO_MODER_MODE6_1; //Set to Alternative Function
	GPIOB->AFR[0] &= ~(GPIO_AFRL_AFSEL6); //configure and select alternative function for PA5 
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1; 
	//Set PB6 to no pull-up, no pull-down
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6); //Set pull-up/pull-down to no pull-up/pull-down
	
	//Enable Timer 4 in RCC APB1ENRx
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN; //Enable clock for GPIO Pin A
	
	//Set the prescaler to 15
	TIM4->PSC = 15;
	
	//Enable auto reload preload in the control register and set the auto reload value to its
	//maximum value
	TIM4->CR1 |= TIM_CR1_ARPE; 
	TIM4->ARR = 0XFFFF; 
	
	//In the capture/compare mode register, set the input capture mode bits such that the
	//input capture is mapped to timer input 1
	TIM4->CCMR1 &= ~(TIM_CCMR1_CC1S); 
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0; 
	
	//In the capture/compare enable register, set bits to capture both rising/falling edges and
	//enable capturing
	TIM4->CCER |= TIM_CCER_CC1P; 
	TIM4->CCER |= TIM_CCER_CC1NP; 
	TIM4->CCER |= TIM_CCER_CC1E; 
	
	//In the DMA/Interrupt enable register, enable both interrupt and DMA requests. In
	//addition, enable the update interrupt
	TIM4->DIER |= TIM_DIER_CC1IE; 
	TIM4->DIER |= TIM_DIER_CC1DE; //double check this
	TIM4->DIER |= TIM_DIER_UIE; 
	
	//Enable update generation in the event generation register
	TIM4->EGR |= TIM_EGR_UG; 
	
	//Clear the update interrupt flag
	TIM4->SR &= ~(TIM_SR_UIF); 
	
	//Set the direction of the counter and enable the counter in the control register
	TIM4->CR1 &= ~(TIM_CR1_DIR); //Upcounting, check with TA what direction should be
	TIM4->CR1 |= TIM_CR1_CEN; 
	
	//Enable the interrupt (TIM4 IRQn) in the NVIC and set its priority to 2
	NVIC_SetPriority(TIM4_IRQn, 2);   // Set priority
	NVIC_EnableIRQ(TIM4_IRQn);   // Enable interrupt
}



void TIM4_IRQHandler(void) {
	/*Compute difference between two consec CCR values 
	(1st from interrupt on rising edge and 2nd on falling edge) to obtain pulse width*/
	if(TIM4->SR & TIM_SR_UIF) { // UIF Interrupt
		TIM4->SR &= ~TIM_SR_UIF; // clear interrupt flag
		overflowCount++;
    } else if (TIM4->SR & TIM_SR_CC1IF) { // Channel #1
        TIM4->SR &= ~TIM_SR_CC1IF; // clear interrupt flag
        lastValue = currentValue;
        currentValue = TIM4->CCR1;
        timeInterval = currentValue - lastValue + overflowCount * (MAX_TIMER + 1);
        overflowCount = 0;
    }
	}


void Trigger_Setup() {
	//Setup PA9
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; //Enable clock for GPIO Pin A
	//Configure PA9 to be used as alternative function TIM1_CH2
	GPIOA->MODER &= ~(GPIO_MODER_MODE9); //Clear the mode register
	GPIOA->MODER |= GPIO_MODER_MODE9_1; //Set to Alternative Function
	GPIOA->AFR[1] &= ~(GPIO_AFRH_AFSEL9); //configure and select alternative function for PA5 
	GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_0; 
	//Set PA9 to no pull-up, no pull-down 
	GPIOA->PUPDR &= ~(GPIO_PUPDR_PUPD9); //Set pull-up/pull-down to no pull-up/pull-down
	//Set the output type of PA9 to push-pull
	GPIOA->OTYPER &= ~(GPIO_OTYPER_OT9); 
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
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_1; //Set the output compare mode bits to PWM mode 1
	TIM1->CCMR1 |= TIM_CCMR1_OC2M_2;  //Make sure this is the right mode register
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;  //Enable output compare preload
	
	//Enable the output in the capture/compare enable register. (Assuming CC1, check with TA)
	TIM1->CCER	|= TIM_CCER_CC2E;
	
	//In the break and dead-time register, set the bits (at the same time) for main output
	//enable, off-state selection for run mode, and off-state selection for idle mode
	TIM1->BDTR |= TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI; 
	
	//Enable update generation in the event generation register
	TIM1->EGR |= TIM_EGR_UG; 
	
	//Enable update interrupt in the DMA/Interrupt enable register and clear the update in-
	//terrupt flag in the status register
	TIM1->DIER |= TIM_DIER_UIE; 
	TIM1->SR &= ~(TIM_SR_UIF); 
	
	//Set the direction of the counter and enable the counter in the control register
	TIM1->CR1 |= (TIM_CR1_DIR); //Upcounting, check with TA what direction should be
	TIM1->CR1 |= TIM_CR1_CEN; 
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
		//Store your measurements on Stack in cm
		if(timeInterval > 5000)
		{
			distance = 0; 
		}
		else
		{
			distance = timeInterval/58;
		}

	}
}
