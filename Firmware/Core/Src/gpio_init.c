#include "stm32f411xe.h"

/* Configure I2C pins, LEDs, and EXTI13 */
void GPIO_Init(void) {
	/* ---------------------- I2C1: PB6 (SCL), PB7 (SDA) ---------------------- */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// Alternate function mode
	GPIOB->MODER &= ~(GPIO_MODER_MODER6 | GPIO_MODER_MODER7);
	GPIOB->MODER |= (GPIO_MODER_MODER6_1 | GPIO_MODER_MODER7_1);

	// Open-drain outputs (I2C requirement)
	GPIOB->OTYPER |= (GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7);

	// Medium speed
	GPIOB->OSPEEDR |= (GPIO_OSPEEDR_OSPEED6_0 | GPIO_OSPEEDR_OSPEED7_0);

	// No internal pull-ups (external pull-ups required)
	GPIOB->PUPDR &= ~(GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD7);

	// AF4 = I2C1
	GPIOB->AFR[0] &= ~((0xF << (6 * 4)) | (0xF << (7 * 4)));
	GPIOB->AFR[0] |= (0x4 << (6 * 4)) | (0x4 << (7 * 4));

	/* ---------------------- LEDs: PA5, PA6 ---------------------- */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

	GPIOA->MODER &= ~(3 << (5 * 2));
	GPIOA->MODER |= (1 << (5 * 2));   // PA5 output

	GPIOA->MODER &= ~(3 << (6 * 2));
	GPIOA->MODER |= (1 << (6 * 2));   // PA6 output

	/* ---------------------- EXTI13: PC13 button ---------------------- */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

	GPIOC->MODER &= ~(3 << (13 * 2));  // Input mode

	// Pull-up (PC13 is active-low)
	GPIOC->PUPDR &= ~(3 << (13 * 2));
	GPIOC->PUPDR |= (1 << (13 * 2));

	// Enable SYSCFG
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;

	// Route PC13 → EXTI13
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;

	// Configure EXTI13 falling-edge interrupt
	EXTI->IMR |= (1 << 13);
	EXTI->FTSR |= (1 << 13);
	EXTI->RTSR &= ~(1 << 13);

	NVIC_EnableIRQ(EXTI15_10_IRQn);
}
