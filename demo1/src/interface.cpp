#include "interface.h"

void IIC_SAND_DATE(uint16_t DEVICE_ADD,uint16_t IN_DEVICE_ADD,uint8_t *DATAS,uint16_t LONG)
{
	Wire.beginTransmission(DEVICE_ADD);
    Wire.write(IN_DEVICE_ADD);
    for(uint16_t a=0;a<LONG;a++)
    {
        Wire.write(DATAS[a]);
    }
    Wire.endTransmission();

}

void IIC_READ_DATE(uint16_t DEVICE_ADD,uint16_t IN_DEVICE_ADD,uint8_t *DATAS,uint16_t LONG)
{
	Wire.beginTransmission(DEVICE_ADD);
    Wire.write(IN_DEVICE_ADD);
    for(uint16_t a=0;a<LONG;a++)
    {
        DATAS[a]=Wire.read();
    }
    Wire.endTransmission();
}
