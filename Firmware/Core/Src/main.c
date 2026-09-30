#include "gpio_init.h"
#include "stm32f411xe.h"
#include "clock.h"
#include "i2c_driver.h"
#include "stop_mode.h"
#include "sht31.h"

// Global tick counter incremented by SysTick
extern volatile uint32_t msTicks;

// Set by EXTI interrupt when PC13 triggers
extern volatile uint8_t exti_flag;

void delay_ms(uint32_t ms);

/* =================================================================== */
/*                               MAIN                                  */
/* =================================================================== */
int main(void) {
	Clock_Init();        // Enable HSI + flash latency
	SysTick_Init();      // 1ms tick
	GPIO_Init();         // I2C pins + LED pins + EXTI13
	I2C1_Init();         // Configure I2C1 peripheral

	uint8_t raw[6];

	while (1) {
		/* Enter STOP mode — wake on EXTI13 falling edge */
		Enter_STOP_Mode();

		if (exti_flag == 1) {

			GPIOA->BSRR = (1 << 6);     // First instruction after ISR exit

			/* Restore system clock after STOP mode */
			SystemClock_Restore();

			GPIOA->BSRR = (1 << (6 + 16));     // Clock restored to HSI

			SysTick_Init();
			I2C1_Init();

			/* Read raw SHT31 data + CRC check */
			uint8_t status = SHT31_ReadRaw_CRC(raw);
			exti_flag = 0;

			delay_ms(1);

			/* If CRC OK → toggle LED on PA5 */
			if (status == 0) {
				GPIOA->BSRR = (1 << 5);   // PA5 HIGH
				delay_ms(1);
				GPIOA->BSRR = (1 << (5 + 16));   // PA5 LOW
			}
		}
	}
}

/* =================================================================== */
/*                           delay_ms                                  */
/* =================================================================== */
void delay_ms(uint32_t ms) {
	uint32_t start = msTicks;
	while ((msTicks - start) < ms)
		;
}
