#include "gpio.h"
#include "robostix-frodo.h"
#include "peri.h"
/* GPIO_Move */
void GPIO_Move(per_entry *pper) {
  char i;
  char* PORTn=0,*PINn=0;
  if ((*(pper->buffer)!=0) && (*(pper->buffer)!=1)) {
    // error invalid data in buffer
    return;
  }

  /* select the port */
  switch (pper->port_name) {
  case GPIO_PORTA:
    PORTn = &PORTA;
    PINn  = &PINA;
    break;
  case GPIO_PORTB:
    PORTn = &PORTB;
    PINn  = &PINB;
    break;
  case GPIO_PORTC:
    PORTn = &PORTC;
    PINn  = &PINC;
    break;
  case GPIO_PORTD:
    PORTn = &PORTD;
    PINn  = &PIND;
    break;
  case GPIO_PORTE:
    PORTn = &PORTE;
    PINn  = &PINE;
    break;
  case GPIO_PORTF:
    PORTn = &PORTF;
    PINn  = &PINF;
    break;
  default:
    // error invalid port
    return;
  }
  
  if (pper->way == IN) {
    /* in */
    i = ((*PINn) && (1<<(pper->pin_number)));
    *(pper->buffer)= i >> (pper->pin_number);
  } else {
    /* out */
    *PORTn = ((*PORTn) & ~(1<<(pper->pin_number))) | (((*(pper->buffer)) && 1)<<(pper->pin_number));
  }
  return;
}
/* GPIO_Init*/
unsigned int GPIO_Init(per_entry *pper) 
{
  char _DDxn=0;
  char* PORTn=0,*DDRn=0;

  if (pper->way == IN) {
    /* in */
    _DDxn=0;
  } else {
    /* out */
    _DDxn=1;
  }
  
  switch (pper->port_name) {
  case GPIO_PORTA:
    PORTn = &PORTA;
    DDRn  = &DDRA;
    break;
  case GPIO_PORTB:
    PORTn = &PORTB;
    DDRn  = &DDRB;
    break;
  case GPIO_PORTC:
    PORTn = &PORTC;
    DDRn  = &DDRC;
    break;
  case GPIO_PORTD:
    PORTn = &PORTD;
    DDRn  = &DDRD;
    break;
  case GPIO_PORTE:
    PORTn = &PORTE;
    DDRn  = &DDRE;
    break;
  case GPIO_PORTF:
    PORTn = &PORTF;
    DDRn  = &DDRF;
    break;
  default:
    // error invalid port
    return 1;
  }
  
  *PORTn=0;
  *DDRn |=(_DDxn << (pper->pin_number));
  
  pper->work_function=&GPIO_Move; // set work function
  
  return 0;
}
