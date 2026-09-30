#ifndef I2C_DRIVER_H
#define I2C_DRIVER_H

void I2C1_Init(void);
void I2C1_Stop(void);
uint8_t I2C1_ReadByte_NACK(void);
uint8_t I2C1_ReadByte_ACK(void);
void I2C1_WriteByte(uint8_t data);
void I2C1_SendAddress(uint8_t addr);
void I2C1_Start(void);

#endif
