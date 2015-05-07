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
 
#if !defined( _UART_H_ )
#define _UART_H_

// ---- Include Files -------------------------------------------------------

#include <inttypes.h>
#include <avr/io.h>
#include "robostix-frodo.h"

#define UART_DATA_BIT_8  (( 1 << UCSZ1 ) | ( 1 << UCSZ0 ))
#define UART_PARITY_NONE (( 0 << UPM1 )  | ( 0 << UPM0 ))
#define UART_STOP_BIT_1  ( 1 << USBS )

#define UBRR1_INIT   (( configCPU_CLOCK_HZ / 16 / UART_BAUD_RATE ) - 1 )

#define UCSR1A_INIT  0
#define UCSR1B_INIT  (( 1 << RXCIE ) | ( 1 << RXEN ) | ( 1 << TXEN ))
#define UCSR1C_INIT  ( UART_DATA_BIT_8 | UART_PARITY_NONE | UART_STOP_BIT_1 )

//---------------------------------------------------------------------------

#if ( UART_MSG_SIZE > 128 )
typedef uint16_t uart_index_t;
#else
typedef uint8_t  uart_index_t;
#endif

typedef struct
{
    uart_index_t idx;
    uint8_t      data[ UART_MSG_SIZE ];
} uart_buffer_t;

void uart_init( void );
portBASE_TYPE  uart_send( uint8_t *msg );
void  uart_recv( uint8_t *msg );
void puthex(int num);


#endif // _UART_H_

