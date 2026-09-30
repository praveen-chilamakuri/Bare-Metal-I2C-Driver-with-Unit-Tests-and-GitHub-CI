#include "stm32f411xe.h"
#include "i2c_driver.h"

#define SHT31_ADDR 0x45

void delay_ms(uint32_t ms);

/* Send measurement command (High Repeatability, Clock Stretching Disabled) */
void SHT31_StartMeasurement(void) {
	I2C1_Start();
	I2C1_SendAddress((SHT31_ADDR << 1) | 0);   // Write
	I2C1_WriteByte(0x24);
	I2C1_WriteByte(0x0B);
	I2C1_Stop();
}

/* Read 6 raw bytes: Temp(2+CRC) + Hum(2+CRC) */
void SHT31_ReadRaw(uint8_t *buf) {
	I2C1->CR1 |= I2C_CR1_ACK;   // ACK enabled

	I2C1_Start();
	I2C1_SendAddress((SHT31_ADDR << 1) | 1);   // Read

	buf[0] = I2C1_ReadByte_ACK();
	buf[1] = I2C1_ReadByte_ACK();
	buf[2] = I2C1_ReadByte_ACK();
	buf[3] = I2C1_ReadByte_ACK();
	buf[4] = I2C1_ReadByte_ACK();
	buf[5] = I2C1_ReadByte_NACK();   // Last byte

	I2C1_Stop();
}

/* =================================================================== */
/*                           CRC8 (SHT31)                              */
/* =================================================================== */
uint8_t SHT31_CRC8(uint8_t *data) {
	uint8_t crc = 0xFF;

	for (int i = 0; i < 2; i++) {
		crc ^= data[i];
		for (int b = 0; b < 8; b++) {
			if (crc & 0x80)
				crc = (crc << 1) ^ 0x31;
			else
				crc <<= 1;
		}
	}
	return crc;
}

/* =================================================================== */
/*                     Read + CRC Validate Raw Data                    */
/* =================================================================== */
uint8_t SHT31_ReadRaw_CRC(uint8_t *raw) {
	SHT31_StartMeasurement();
	delay_ms(15);                  // SHT31 measurement time

	SHT31_ReadRaw(raw);            // Read 6 bytes

	/* CRC check: raw[0..1] temp, raw[3..4] humidity */
	if ((SHT31_CRC8(raw) != raw[2]) || (SHT31_CRC8(&raw[3]) != raw[5])) {
		return 1;   // CRC fail
	} else {
		return 0;  // CRC OK
	}
}

