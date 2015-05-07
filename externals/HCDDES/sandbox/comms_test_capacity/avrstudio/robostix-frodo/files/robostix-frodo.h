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


#ifndef _ROBOSTIX_FRODO_H_
#define _ROBOSTIX_FRODO_H_

/* Scheduler include files. */
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"

#define UART_BAUD_RATE 57600
#define UART_MSG_SIZE (1 + 4)
#define UART_START_SYNC_BYTE 147

typedef struct {
	pdTASK_CODE		task_fn;
	const portCHAR* const name;
	portTickType	offset;	// ms
	xSemaphoreHandle semaphore;
} task_entry;

#define MSG_DIR_SEND	0
#define MSG_DIR_RECV	1
#define MSG_MAX_LENGHT	256
#define MSG_ID_INVALID	-1

typedef struct {
	portCHAR	id;
	portCHAR	dir;
	portCHAR	length;
	portCHAR*	buffer;
	portCHAR*	cbuffer;
	portTickType offset; // ms
	portCHAR 	dirty;	
} msg_entry;

/* new */
typedef struct per_entry{
  portCHAR id;
  const portCHAR* const name; //periphery
  portCHAR way; // in out
  const portCHAR port_name; // A,B,C,D,E,F
  const portCHAR pin_number;
  portCHAR length;
  portCHAR* buffer;
  portCHAR* cbuffer;
  portTickType offset; // ms
  
  //portCHAR* dummy;
  void (*work_function)(struct per_entry*); // function pointer  
  struct per_entry * next;
  portCHAR header;
} per_entry;

#endif //_ROBOSTIX_FRODO_H_
