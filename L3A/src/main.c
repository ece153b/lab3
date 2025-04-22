/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 3A
 */

#include "stm32l476xx.h"
#include "LED.h"
#include "SysTimer.h"

void System_Clock_Init(void) {
	// Select MSI as system clock source
	//Replace with value that will make MSI the system clock source - 00 for MSI
	RCC->CFGR &= ~RCC_CFGR_SW; 
	
	
	// Set MSI clock range
	RCC->CR &= ~RCC_CR_MSIRANGE;
	 //Replace with value that will make range 8 MHz - 0111: range 7 around 8 MHz
	RCC->CR |= RCC_CR_MSIRANGE_7; 
	
	// Use the MSI clock range that is defined in RCC_CR
	//Replace with value that will select range in RCC->CR
	RCC->CR |= RCC_CR_MSIRGSEL; 
	
	// Enable MSI oscillator
	//Replace with value that will enable MSI
	RCC->CR |= RCC_CR_MSION; 
	
	// Wait until MSI is ready
	 /*Replace with value that checks whether MSI is ready */
	while((RCC->CR & RCC_CR_MSIRDY) == 0);
}

int main() {
	// Initialization 
	System_Clock_Init();
	SysTick_Init();
	LED_Init();		

	
	while(1) {
		delay(1000); //1000 seems way to long - check with TA
		Green_LED_Toggle(); 
	}
}
