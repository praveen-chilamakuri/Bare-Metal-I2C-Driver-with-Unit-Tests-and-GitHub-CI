#include "stm32f411xe.h"

/* Enter STOP mode (regulator ON) — wake on EXTI */
void Enter_STOP_Mode(void) {
	PWR->CR |= PWR_CR_CWUF;          // Clear wakeup flag

	PWR->CR &= ~PWR_CR_PDDS;         // PDDS=0 → STOP mode
	PWR->CR &= ~PWR_CR_LPDS;         // LPDS=0 → main regulator ON

	SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;   // Deep sleep

	__WFI();                         // Enter STOP mode

	SCB->SCR &= ~SCB_SCR_SLEEPDEEP_Msk;  // Clear after wake
}

/* Restore HSI clock after STOP mode wake */
void SystemClock_Restore(void) {
	RCC->CR |= RCC_CR_HSION;
	while (!(RCC->CR & RCC_CR_HSIRDY))
		;

	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI)
		;

	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN |
	RCC_AHB1ENR_GPIOBEN |
	RCC_AHB1ENR_GPIOCEN;

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
}
