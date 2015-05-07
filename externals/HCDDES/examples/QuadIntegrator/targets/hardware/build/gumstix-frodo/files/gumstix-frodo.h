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

#ifndef _GUMSTIX_FRODO_H_
#define _GUMSTIX_FRODO_H_

#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>
//#include "OuterLoop_sl.h"


typedef struct {
	void*	(*task_fn)(void*);
	const char* const name;
	int 	offset;	// ms
} task_entry;

extern task_entry tasks[];

#define MSG_DIR_SEND	0
#define MSG_DIR_RECV	1
#define MSG_MAX_LENGHT	256
#define MSG_ID_INVALID	-1

typedef struct {
	uint8_t	id;
	uint8_t	dir;
	uint8_t	length;
	uint8_t*	buffer;
	int	offset; // ms
} msg_entry;

extern msg_entry msgs[];

extern int hp_len;

#define portTickType int
#define portCHAR char
#define pdFALSE 0
#define pdTRUE 1

void* frodo_task(void* param);
int timediff(const struct timeval* time1, const struct timeval* time2);
void offset_wait(const struct timeval* start_time, int offset);

#endif // _GUMSTIX_FRODO_H_
