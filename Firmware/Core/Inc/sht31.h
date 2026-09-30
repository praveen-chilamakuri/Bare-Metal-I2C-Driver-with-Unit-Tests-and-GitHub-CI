#ifndef SHT31_H
#define SHT31_H

void SHT31_ReadRaw(uint8_t *buf);
void SHT31_StartMeasurement(void);
uint8_t SHT31_CRC8(uint8_t *data);
uint8_t SHT31_ReadRaw_CRC(uint8_t *raw);

#endif
