#include <avr/io.h>
#if defined( __AVR_LIBC_VERSION__ )
#   include <avr/interrupt.h>
#else
#   include <avr/signal.h>
#endif
#include <string.h>
#include "uart0.h"
#include "led.h"
#include "peri.h"
int i;
int j=0;
int errflag = 0;
per_entry *trans_per;
per_entry *recv_per;

/* @todo: HACK - this should not be a constant */
/* Make this a variable, and sum the lengths into it during init. */
//unsigned char recv_buflength = 62;

void UART1_Move(per_entry *pper) {
  
  if (pper->way == IN) {
    float val = 0.0;   
    portENTER_CRITICAL();
    memcpy(pper->buffer, &(pper->cbuffer[pper->read_idx]), pper->length);
    /*val = (float) *((pper->buffer)+1);*/
    val = (float) *((&(pper->cbuffer[pper->read_idx]))+1);
    portEXIT_CRITICAL();
    if ( val > 1.0 )
    {
       DDRC|=0x80;
       PORTC|=0x80;
       LED_ON(RED); // One instruction just to give us some time
       PORTC&=~0x80;
    }
    return;
  }
  if (pper->way == OUT) {
    if (UCSR1B & ( 1 << UDRIE1 )) {
      // error on sending (uart busy)
      LED_ON(BLUE);
      return;
    }
    portENTER_CRITICAL();
    i=0;
    memcpy(pper->cbuffer, pper->buffer, pper->length);
    portEXIT_CRITICAL();
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
  static unsigned char first = 0;
  if (pper)  {
    pper->work_function=&UART1_Move;
    if ((pper->way == IN) && !first) 
    {
       pper->read_idx = 0;
       DDRC|=0x0f;
       PORTC&=~0x04;
       pper->write_idx = 1; 
       PORTC|=0x08;
       recv_per=pper; 
       first = 1; 
    }
    if (pper->way == OUT) 
    {
       trans_per=pper;
       pper->read_idx = 0;
       pper->write_idx = 0;
    }
    return 0;
  } else {
    return 2;
  }
}

SIGNAL( SIG_USART1_RECV ) {

  DDRC|=0x40;
  PORTC|=0x40;
  while ( !(UCSR1A & (1<<RXC)) )
	;  /* Wait for data to be available */

  if ( UCSR1A & ( (1<<FE) | (1<<DOR) | (1<<UPE) ) )
  {
    LED_ON( BLUE );
    errflag = 1;
  }

  if (j<recv_per->length) { 
    *(&(recv_per->cbuffer[recv_per->write_idx])+j)=UDR1;
    j++;
  } else {
    *(&(recv_per->cbuffer[recv_per->write_idx]))=UDR1;
    j=1;
  }
 
  if (j == recv_per->length )
  {
    portCHAR tmp_idx = recv_per->write_idx;
    recv_per->write_idx = recv_per->read_idx;
    recv_per->read_idx = tmp_idx;
    DDRC|=0x04; PORTC^=0x04; // toggle the bit
    DDRC|=0x08; PORTC^=0x08; 

    if ( (float) *( (&(recv_per->cbuffer[recv_per->write_idx]))+1 ) > 1.0 )
    {
       DDRC|=0x80;
       PORTC|=0x80;
       LED_ON(RED); // One instruction just to give us some time
       PORTC&=~0x80;
    }

    errflag = 0;
    //LED_OFF(BLUE);
  } 

  PORTC&=~0x40;
}

SIGNAL( SIG_USART1_DATA ) {
  DDRC|=0x40;
  PORTC|=0x40;
  UDR1=*(trans_per->cbuffer+i);
  i++;
  if (i>=trans_per->length) UCSR1B &= ~(1 << UDRIE1);
  PORTC&=~0x40;
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
