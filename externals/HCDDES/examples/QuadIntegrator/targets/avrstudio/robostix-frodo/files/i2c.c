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
 
#include <avr/io.h>
#if defined( __AVR_LIBC_VERSION__ )
#   include <avr/interrupt.h>
#else
#   include <avr/signal.h>
#endif
#include <string.h>

#include "robostix-frodo.h"
#include "i2c.h"
#include "led.h"

#define I2C_ADDR 76

#define ST_IDLE 	0
#define ST_SYNC_ADDR	1
#define ST_SYNC_TYPE	2
#define ST_RCV_ADDR	3
#define ST_RCV_TYPE	4
#define ST_RCV_DATA	5
#define ST_SND_ADDR	6
#define ST_SND_TYPE	7
#define ST_SND_DATA	8

static volatile portBASE_TYPE	status;
static volatile xSemaphoreHandle sync_semaphore;

/*-----------------------------------------------------------*/

void i2c_init( void )
{
    TWAR = (I2C_ADDR << 1);	// Set slave address, no general calls
    TWCR = (0 << TWINT) | (0 << TWEA) | (0 << TWSTA) | (0 << TWSTO) | (0 << TWWC) | (1 << TWEN) | (1 << TWIE); // Setup as slave, enable irq
    status = ST_IDLE;
	 vSemaphoreCreateBinary( sync_semaphore );
    xSemaphoreTake(sync_semaphore, 0);
}

// TODO: relase
/*-----------------------------------------------------------*/
portBASE_TYPE i2c_wait_for_sync( void )
{
	portBASE_TYPE res = pdFAIL;
	portENTER_CRITICAL();
	if (status == ST_IDLE || status == ST_SYNC_ADDR || status == ST_SND_ADDR || status == ST_RCV_ADDR ) {
		TWCR |= (1 << TWEA);
		status = ST_SYNC_ADDR;
		res = pdPASS;
	}
	portEXIT_CRITICAL();
	
	if (res == pdPASS) {
		res = xSemaphoreTake(sync_semaphore, portMAX_DELAY);
	}
	
	return res;
}

/*-----------------------------------------------------------*/
static msg_entry* current_msg;

void i2c_process_msg(msg_entry *msg)
{
	portBASE_TYPE res = pdFAIL;
	portENTER_CRITICAL();
	if (status == ST_IDLE  || status == ST_SYNC_ADDR || status == ST_SND_ADDR || status == ST_RCV_ADDR) {
		current_msg = msg;
		if (msg->dir == MSG_DIR_SEND) {
			memcpy(msg->cbuffer, msg->buffer, msg->length);
			TWCR |= (1 << TWEA);
			status = ST_SND_ADDR;
			res = pdPASS;
		}
		if (msg->dir == MSG_DIR_RECV) {
			TWCR |= (1 << TWEA);
			status = ST_RCV_ADDR;
			res = pdPASS;
		}
	}
	portEXIT_CRITICAL();
}

/*-----------------------------------------------------------*/
//
// Communication protocol on top of I2C:
// Start of hyperperiod:    <slave_addr +  WR>[A] <0xFF>po
// I2C Write (master send): <slave_addr +  WR>[A] <msgid>[A] <msg0>[A] <msg1>[A] ... <msgN>[/A]
// I2C Read  (master recv): <slave_addr + /WR>[A] <msgid>[A] <msg0>[A] ...
//
static portBASE_TYPE	nBytes;

SIGNAL(SIG_2WIRE_SERIAL)
{
    static signed portBASE_TYPE xHigherPriorityTaskWoken; 
    xHigherPriorityTaskWoken = pdFALSE;
    

    switch(status) {
        case ST_IDLE:
                LED_TOGGLE( YELLOW );
                break;
	case ST_SYNC_ADDR:
		if (TWSR == 0x60) {
			status = ST_SYNC_TYPE;
		}
		break;
	case ST_SYNC_TYPE:
		if (TWSR == 0x80 && TWDR == 0xFF) {
                  DDRC|=0x02;
		  PORTC|=0x02;

			xSemaphoreGiveFromISR( sync_semaphore, &xHigherPriorityTaskWoken );
			TWCR &= ~(1 << TWEA);
			status = ST_IDLE;
		  PORTC&=~0x02;
		}
		else {
			status = ST_SYNC_ADDR;
		}
		break;
	case ST_RCV_ADDR:
		if (TWSR == 0x60) {
			status = ST_RCV_TYPE;
		}
		break;
	case ST_RCV_TYPE:
		if (TWSR == 0xA0) {
			status = ST_RCV_ADDR;
		}
		if (TWSR == 0x80) {
			if (TWDR == current_msg->id) {
				nBytes = 0;
				status = ST_RCV_DATA;
			}
			else {
				TWCR &= ~(1 << TWEA);
				status = ST_IDLE;
			}
		}
		break;
	case ST_RCV_DATA:
		if (TWSR == 0xA0) {
			status = ST_RCV_ADDR;
		}
		if (TWSR == 0x80) {
                  DDRC|=0x04;
		  PORTC|=0x04;

			current_msg->cbuffer[nBytes++] = TWDR;
			if (nBytes >= current_msg->length) {
                                DDRC|=0x01;
                                PORTC|=0x01;
				memcpy(current_msg->buffer, current_msg->cbuffer, current_msg->length);
				TWCR &= ~(1 << TWEA);
				status = ST_IDLE;
                                PORTC&=~0x01;
			}

			PORTC&=~0x04;
		}
		if (TWSR == 0x88) {	 // should not happen
			status = ST_IDLE;
		}
		break;
	case ST_SND_ADDR:
		if (TWSR == 0xA8) {
			status = ST_SND_TYPE;
			TWDR = current_msg -> id;
		}
		break;
	case ST_SND_TYPE:
		if (TWSR == 0xB8) {
			nBytes = 0;
			TWDR = current_msg->cbuffer[nBytes++];
			status = ST_SND_DATA;
		}
		else {
			TWCR &= ~(1 << TWEA);
			status = ST_IDLE;
		}
		break;
	case ST_SND_DATA:
		if (TWSR == 0xB8) {
                  DDRC|=0x08;
		  PORTC|=0x08;
			if (nBytes >= current_msg->length) {
				TWCR &= ~(1 << TWEA);
				status = ST_IDLE;
			}
			TWDR = current_msg->cbuffer[nBytes++];
			PORTC&=~0x08;
		}
		if (TWSR == 0xC0) {
			TWCR &= ~(1 << TWEA);
			status = ST_IDLE;
		}
		if (TWSR == 0xC8) {	 // should not happen
			status = ST_IDLE;
		}
		break;
    }


    // Now that we've finished dealing with the interrupt, set the TWINT flag
    // which will stop the clock stretching and clear the interrupt source
    TWCR |= ( 1 << TWINT );
    

    if (xHigherPriorityTaskWoken != pdFALSE) {
    	taskYIELD( );
    }
}

/*-----------------------------------------------------------*/

