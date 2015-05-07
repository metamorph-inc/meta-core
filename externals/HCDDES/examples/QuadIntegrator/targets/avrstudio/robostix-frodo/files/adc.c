#include <avr/io.h>
#if defined( __AVR_LIBC_VERSION__ )
#   include <avr/interrupt.h>
#else
#   include <avr/signal.h>
#endif
#include "adc.h"
#include <string.h>
per_entry *current_per;
static uint8_t UARTmsg[UART_MSG_SIZE];

void ADC_Move(per_entry *pper) {
  if (ADCSRA & (1<<ADSC)) return; // if ad busy  
  memcpy(pper->buffer, pper->cbuffer, pper->length);
  ADMUX = (ADMUX & 0x3F) | ((pper->way)<<6); // reference voltage
  ADMUX = (ADMUX & 0xE0) | pper->pin_number; // select pin
  current_per=pper; // save the current row in the table for the interrupt
  ADCSRA |= 1<<ADSC; // start conversion 
  //puthex(*(int*)pper->buffer);
  //UART_SendByte('|');
  return;
}

unsigned int ADC_Init(per_entry *pper,unsigned char div) {
  ADCSRA |= (1<<ADEN);
  ADCSRA |= div; // 6:clk/64 
  DDRF = 0x00; 
  PORTF = 0x00; 
  ADCSRA |= (1<<ADIE);
  pper->work_function=&ADC_Move;
  return 0;
}
// ADC interrupt handler
SIGNAL(SIG_ADC){
  // store the ad result to the buffer
  *(int*)(current_per->cbuffer)=((ADCL)| ((ADCH)<<8));
}
