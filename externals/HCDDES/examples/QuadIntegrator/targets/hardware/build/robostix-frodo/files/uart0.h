#ifndef _UART1_H_
#define _UART1_H_

#include "robostix-frodo.h"

#define UART_DATA_BIT_8  (( 1 << UCSZ1 ) | ( 1 << UCSZ0 ))
#define UART_PARITY_NONE (( 0 << UPM1 )  | ( 0 << UPM0 ))
#define UART_STOP_BIT_1  ( 1 << USBS )

#define BAUDRATE 57600

#define PUTC UART_SendByte

void UART_Move(per_entry *pper);
unsigned int UART1_Init( unsigned int baudratev, char parity, char databit, char stopbit);
unsigned int UART0_Init( unsigned int baudratev, char parity, char databit, char stopbit);
unsigned int UART_Init_WF(per_entry *pper);
//void UART1_Init(per_entry *pper);
void UART_SendByte(unsigned char data);
void puthex(int num);

#endif // _UART1_H_
