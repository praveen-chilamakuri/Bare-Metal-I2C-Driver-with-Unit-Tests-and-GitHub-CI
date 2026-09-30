#include "stm32f411xe.h"

/* Configure I2C1 for 100 kHz standard mode */
void I2C1_Init(void) {
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

	I2C1->CR1 = 0;
	I2C1->CR2 = 16;      // PCLK1 = 16 MHz
	I2C1->CCR = 80;      // 100 kHz
	I2C1->TRISE = 17;    // TRISE = Fpclk(MHz) + 1

	I2C1->CR1 |= I2C_CR1_PE;
}

/* Generate START condition */
void I2C1_Start(void) {
	I2C1->CR1 |= I2C_CR1_START;
	while (!(I2C1->SR1 & I2C_SR1_SB))
		;
}

/* Send 7-bit address + R/W bit */
void I2C1_SendAddress(uint8_t addr) {
	I2C1->DR = addr;
	while (!(I2C1->SR1 & I2C_SR1_ADDR))
		;
	(void) I2C1->SR2;     // Clear ADDR flag
}

/* Write one byte */
void I2C1_WriteByte(uint8_t data) {
	while (!(I2C1->SR1 & I2C_SR1_TXE))
		;
	I2C1->DR = data;
	while (!(I2C1->SR1 & I2C_SR1_BTF))
		;
}

/* Read byte with ACK */
uint8_t I2C1_ReadByte_ACK(void) {
	I2C1->CR1 |= I2C_CR1_ACK;
	while (!(I2C1->SR1 & I2C_SR1_RXNE))
		;
	return I2C1->DR;
}

/* Read byte with NACK (last byte) */
uint8_t I2C1_ReadByte_NACK(void) {
	I2C1->CR1 &= ~I2C_CR1_ACK;
	while (!(I2C1->SR1 & I2C_SR1_RXNE))
		;
	return I2C1->DR;
}

/* Generate STOP condition */
void I2C1_Stop(void) {
	I2C1->CR1 |= I2C_CR1_STOP;
}
