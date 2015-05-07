/*
 * Copyright (c) 2008, Vanderbilt University
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 *
 * IN NO EVENT SHALL THE VANDERBILT UNIVERSITY BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE VANDERBILT
 * UNIVERSITY HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * THE VANDERBILT UNIVERSITY SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE VANDERBILT UNIVERSITY HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.
 */

// ---- Include Files -------------------------------------------------------

#include "uart.h"

#include <avr/io.h>
#if defined( __AVR_LIBC_VERSION__ )
#   include <avr/interrupt.h>
#else
#include <avr/signal.h>
#endif
#include <string.h>

#include "led.h"

// ---- Public Variables ----------------------------------------------------
// ---- Private Constants and Types -----------------------------------------

// ---- Private Variables ---------------------------------------------------

volatile uart_buffer_t g_rx_buffs[2];
volatile uint8_t	c_rx_buff;		// Current receive buffer (used by the ISR)
uart_buffer_t g_tx_buff;

// ---- Private Function Prototypes -----------------------------------------
// ---- Functions -----------------------------------------------------------


SIGNAL( SIG_USART1_RECV )
{
	volatile uart_buffer_t *buff = &g_rx_buffs[c_rx_buff];
    uint8_t ch = UDR1;   // Read the character from the UART

	if ( (buff->idx > 0) || (ch == UART_START_SYNC_BYTE) ) {
		buff->data[buff->idx] = ch;
		buff->idx++;
	}
    
    if ( buff->idx == UART_MSG_SIZE)
    {
        // portENTER_CRITICAL();
        c_rx_buff = (c_rx_buff + 1) % 2;
        g_rx_buffs[c_rx_buff].idx = 0;
    	//LED_TOGGLE(YELLOW);
        // 	portEXIT_CRITICAL();
    }
}

//---------------------------------------------------------------------------

SIGNAL( SIG_USART1_DATA )
{
    if ( g_tx_buff.idx == UART_MSG_SIZE )
    {
        // Nothing left to transmit, disable the transmit interrupt
        UCSR1B &= ~( 1 << UDRIE1 );
    }
    else
    {
        // Otherwise, write the next character from the TX Buffer
        UDR1 = g_tx_buff.data[g_tx_buff.idx++];
    }
}

//---------------------------------------------------------------------------

void uart_init( void ) 
{
	UBRR1H = UBRR1_INIT >> 8;
    UBRR1L = UBRR1_INIT & 0xFF;

    UCSR1A = UCSR1A_INIT;
    UCSR1B = UCSR1B_INIT;
    UCSR1C = UCSR1C_INIT;
}

//---------------------------------------------------------------------------
portBASE_TYPE  uart_send( uint8_t *msg )
{
	if (UCSR1B & ( 1 << UDRIE1 )) {
		return pdFAIL;
	}
	
	portENTER_CRITICAL();
	memcpy(g_tx_buff.data, msg, UART_MSG_SIZE);
	g_tx_buff.idx = 0;
	portEXIT_CRITICAL();
		
    // Enable the transmit interrupt now that there's a character in the
    // buffer.
    UCSR1B |= ( 1 << UDRIE1 );

    return pdPASS;
}

//---------------------------------------------------------------------------

void  uart_recv( uint8_t *msg )
{
	portENTER_CRITICAL();
	volatile uart_buffer_t *buff = &g_rx_buffs[(c_rx_buff + 1) % 2];
	memcpy(msg, buff->data, UART_MSG_SIZE);
	portEXIT_CRITICAL();
}
void UART_SendByte(unsigned char data) {
  while(!(UCSR1A & (1 << UDRE)));
  //transmit
  UDR1=data;
}
void puthex(int num) {
  unsigned char c;
  int i;

  for(i=12;i>=0;i-=4) {
    if((c = ((num>>i)&0x0f))>9)
      c += 'A'-10;
    else
      c += '0';
    UART_SendByte(c);
  }
  

}




