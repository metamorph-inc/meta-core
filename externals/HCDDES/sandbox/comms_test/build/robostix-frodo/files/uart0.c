#include <avr/io.h>
#if defined( __AVR_LIBC_VERSION__ )
#   include <avr/interrupt.h>
#else
#   include <avr/signal.h>
#endif
#include <string.h>
#include "uart0.h"
#include "peri.h"
int i;
int j=0;
per_entry *trans_per;
per_entry *recv_per;
void UART1_Move(per_entry *pper) {
  //current_per=pper;
  if (pper->way == IN) {
    recv_per=pper;
    portENTER_CRITICAL();
    memcpy(pper->buffer, pper->cbuffer, pper->length);
    portEXIT_CRITICAL();
    return;
  }
  if (pper->way == OUT) {
    trans_per=pper;
    if (UCSR1B & ( 1 << UDRIE1 )) {
      // error on sending (uart busy)
      return;
    }
    portENTER_CRITICAL();
    i=0;
    memcpy(pper->cbuffer, pper->buffer, pper->length);
    portEXIT_CRITICAL();
    //UART_SendByte(*(pper->cbuffer));
    UCSR1B |= ( 1 << UDRIE1 );
    //}
    return;
  } else {
    // bad way in the table
    return;
  }
}
unsigned int UART1_Init( unsigned int baudratev, char parity, char databit, char stopbit) {
  //void UART1_Init(per_entry *pper) {
  int _baud_rate;
  _baud_rate = ((configCPU_CLOCK_HZ / 16 / baudratev ) - 1 );
  UBRR1H = _baud_rate >> 8;
  UBRR1L = _baud_rate & 0xFF;

  UCSR1B = (( 1 << RXCIE ) | ( 1 << RXEN ) | ( 1 << TXEN ));
  //UCSR1C = ( UART_DATA_BIT_8 | UART_PARITY_NONE | UART_STOP_BIT_1 );
  UCSR1C = ( (databit<<UCSZ0) | (parity<<UPM0) | (stopbit<<USBS) );
  //UCSR1C = ( databit | parity | stopbit );
  //if (pper)  pper->work_function=&UART1_Move;
  j=0;
  return 0;
}

unsigned int UART_Init_WF(per_entry *pper) {
  if (pper)  {
    pper->work_function=&UART1_Move;
    if (pper->way == IN)  recv_per=pper;
    if (pper->way == OUT) trans_per=pper;
    return 0;
  } else {
    return 2;
  }
}

SIGNAL( SIG_USART1_RECV ) {
  if (j<recv_per->length) {
    *(recv_per->cbuffer+j)=UDR1;
    j++;
  } else {
    *(recv_per->cbuffer)=UDR1;
    j=1;
  }
}

SIGNAL( SIG_USART1_DATA ) {
  UDR1=*(trans_per->cbuffer+i);
  i++;
  if (i>=trans_per->length) UCSR1B &= ~(1 << UDRIE1);
}

void UART_SendByte(unsigned char data) {
  while(!(UCSR1A & (1 << UDRE1)));
  //transmit
  UDR1=data;
}
void puthex(int num) {
  unsigned char c;
  int _i;

  for(_i=12;_i>=0;_i-=4) {
    if((c = ((num>>_i)&0x0f))>9)
      c += 'A'-10;
    else
      c += '0';
    UART_SendByte(c);
  }
}

void putstr( char *str ) {
  while (*str) {
    UART_SendByte((unsigned char) *str);
    str++;
  }
}
