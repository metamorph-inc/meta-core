#ifndef _ADC_H_
#define _ADC_H_

#include "robostix-frodo.h"

void ADC_Move(per_entry *pper);
unsigned int ADC_Init(per_entry *pper, unsigned char div);

#endif // _ADC_H_
