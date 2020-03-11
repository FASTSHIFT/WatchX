
#ifndef __SOFTSERIAL_H__
#define __SOFTSERIAL_H__

#include "stdint.h"
#include "arduino.h"

void SoftSerial_Write(Pin_TypeDef pin, uint32_t frameDuration, uint8_t data);


#endif

