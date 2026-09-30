#include "main.h"
#include "stm32f411xe.h"
#include "stm32f4xx_it.h"
#include <stdint.h>

volatile uint32_t msTicks = 0;
volatile uint8_t exti_flag = 0;

void NMI_Handler(void)
{ while (1) {} }

void HardFault_Handler(void)
{ while (1) {} }

void MemManage_Handler(void)
{ while (1) {} }

void BusFault_Handler(void)
{ while (1) {} }

void UsageFault_Handler(void)
{ while (1) {} }

void SVC_Handler(void)
{}
void DebugMon_Handler(void)
{}
void PendSV_Handler(void)
{}

void SysTick_Handler(void)
{
	msTicks++;
}

void EXTI15_10_IRQHandler(void)
{
	GPIOA->BSRR = (1 << 6);               // PA6 High on ISR Entry
    if (EXTI->PR & (1 << 13))             // Check pending bit
    {
        EXTI->PR = (1 << 13);             // Clear pending bit
        exti_flag = 1;
    }
    GPIOA->BSRR = (1 << (6 + 16));        // PA6 Low on ISR Exit
}
