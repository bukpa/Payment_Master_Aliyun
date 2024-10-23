#ifndef __CRC16_H
#define __CRC16_H


#include <stdint.h>



uint16_t CRC16Calculation(uint8_t *CRC16Data, uint32_t CRC16Datalength);
uint16_t CRC16CalculationData2(uint8_t *CRC16Data, uint32_t CRC16Datalength, uint8_t *CRC16Data2, uint32_t CRC16Datalength2);

#endif /* __CRC16_H */