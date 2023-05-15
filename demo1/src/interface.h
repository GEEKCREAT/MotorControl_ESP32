#ifndef HREAD_INTERFACE_H_
#define HREAD_INTERFACE_H_

#include "Arduino.h"
#include "Wire.h"


void IIC_SAND_DATE(uint16_t DEVICE_ADD,uint16_t IN_DEVICE_ADD,uint8_t *DATAS,uint16_t LONG);
void IIC_READ_DATE(uint16_t DEVICE_ADD,uint16_t IN_DEVICE_ADD,uint8_t *DATAS,uint16_t LONG);

#endif /* HREAD_INTERFACE_H_ */
