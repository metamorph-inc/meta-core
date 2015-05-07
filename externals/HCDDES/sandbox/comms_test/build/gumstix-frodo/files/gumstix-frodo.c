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

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

#include "i2c-dev.h"
#include "i2c-api.h"
#include "i2c-io-api.h"
#include "Log.h"

#include "gumstix-frodo.h"

const char *i2c_dev_name = "/dev/i2c-0";
const int i2c_addr = 76;
int i2c_dev;

extern void app_tasks_init();

int main()
{
	// float test;
	// test = 12;
	// unsigned char* test2 = (unsigned char*)&test;
	// printf("%d %d %d %d\n", test2[0], test2[1], test2[2], test2[3]);
	// return 1;
	LogInit( stdout );

	Log("gumstix-frodo initializing...\n");

	int res;
	task_entry *task;
 
	/* Opening I2C device */
	if (( i2c_dev = open( i2c_dev_name, O_RDWR )) < 0 )
	{
		LogError( "Error  opening '%s': %s\n", i2c_dev_name, strerror( errno ));
		exit( 1 );
	}

	// Indicate which slave we wish to speak to
	I2cSetSlaveAddress( i2c_dev, i2c_addr, I2C_USE_CRC );


	/* Create application tasks. */
	//for (task = tasks; task->task_fn; task++) {
	//	pthread_mutex_init(&(task->mutex), NULL);
	//	res = pthread_create(&(task->pthread), NULL, task->task_fn, task);
	//	if (res) {
	//		LogError( "Cannot create application task '%s': %s\n", task->name, strerror(errno));
	//		exit(1);
	//	}
	//}

	/* Create the FRODO task. */
	//pthread_t frodo_pthread;
	//res = pthread_create(&frodo_pthread, NULL, frodo_task, NULL);
	//if (res) {
	//	LogError( "Cannot create FRODO task: %s\n", strerror(errno));
	//	exit(1);
	//}
	// TODO: elevate priority of this thread

	Log("gumstix-frodo running...\n");
        app_tasks_init();
	frodo_task(NULL);
	//for (task = tasks; task->task_fn; task++) {
	//	pthread_join(task->pthread, NULL);
	//}

	close( i2c_dev );

	Log("gumstix-frodo finished.\n");
	
	return 0; 
}


void* frodo_task(void* param)
{
   int res;
   struct timeval hp_begin;
   task_entry *task;
   msg_entry *msg;
   uint8_t sync_msg[] = {0xFF};
   uint8_t cbuffer[MSG_MAX_LENGHT + 1];
   struct timeval dbg1, dbg2;

   for( ;; )
   {
      gettimeofday(&hp_begin, NULL);
      //Log("=======================\n"); 
		
      if (( res = I2cSendBytes( i2c_dev, sync_msg, sizeof(sync_msg) )) != 0 )
      {

         LogError( "I2cSendBytes failed (in HP sync): %d\n", res );
         return;
      }


      task_entry *task = tasks;
      msg_entry  *msg  = msgs;
      while ((task->task_fn != NULL) || (msg->id != (uint8_t)MSG_ID_INVALID) ) {

         if (task->task_fn != NULL) {
            
            if ((msg->id == (uint8_t)MSG_ID_INVALID) 
                || (task->offset <= msg->offset)) {

               offset_wait(&hp_begin, task->offset * 1000);
               task->task_fn(NULL);
               task++;
               continue;
            }
         }

            offset_wait(&hp_begin, msg->offset * 1000);

            if (msg->dir == MSG_DIR_SEND) {
	
               // TODO: protect the following lines:
               cbuffer[0] = msg->id;
               memcpy((cbuffer+1), msg->buffer, msg->length);

               if (( res = I2cSendBytes( i2c_dev, cbuffer, msg->length + 1 )) != 0 )
               {
                  LogError( "I2cSendBytes failed: %d\n", res );
               }

               //gettimeofday(&dbg2, NULL);
               //Log("=== TX: %d us\n", timediff(&dbg1, &dbg2)); 
            }

            if (msg->dir == MSG_DIR_RECV) {

               if (( res = I2cReceiveBytes( i2c_dev, cbuffer, msg->length + 1 )) != 0 )
               {
                  LogError( "I2cReceiveBytes failed: %d\n", res );
               }

               // TODO: protect the following lines:
               if (cbuffer[0] == msg->id) {
       
                  memcpy(msg->buffer, (cbuffer+1), msg->length);
               }
            }
	
            msg++;
         }	

      offset_wait(&hp_begin, hp_len * 1000);
   }
}

//
// Returns time difference in microseconds (might overflow, use with care)
//
int timediff(const struct timeval* time1, const struct timeval* time2)
{
	return (time2->tv_sec - time1->tv_sec) * 1000000 + (time2->tv_usec - time1->tv_usec);
}

// 
// Wait until a specified (offset) time.
// Parameters: start_time	- initial (hp) start time
//             offset - release time (relative to start_time) in us
//
static int counter = 0;
void offset_wait(const struct timeval* start_time, int offset)
{
	int res;
	struct timeval now, now2, now3;
	struct timespec wait_req, wait_res;
	int us_to_next;

	gettimeofday(&now, NULL);
	now2 = now;
	us_to_next = offset - timediff(start_time, &now);
        
	if ( counter == 0 )
        {
           Log( "First - offset %d - us_to_next %d\n", offset, us_to_next );
	}
        //else if ( (counter % 5) == 0 )
        //   Log( "Count %d", counter );
        counter++;
		
	if (us_to_next < 0) {
		Log("%d: Missed deadline at offset: %d us (by %d us)\n", now.tv_sec, offset, us_to_next);
		return;
	}
	
	if (us_to_next > 10000) {
		// Wake up 10ms earlier, hack
		// Cannot wait for more than a sec !
		wait_req.tv_sec = 0;
		wait_req.tv_nsec = (us_to_next - 10000) * 1000;
		res = nanosleep(&wait_req, &wait_res);
		if (res) {
			LogError( "Nanosleep failed: %s (%d ns)\n", strerror(errno), wait_req.tv_nsec);
			//exit(1);
		}
	}

	do  {
		gettimeofday(&now, NULL);
		us_to_next = offset - timediff(start_time, &now);
	} while (us_to_next > 0);
	
	
gettimeofday(&now3, NULL);
//Log("ENTER @%d us FOR WAIT OFFSET %d us\n", timediff(start_time, &now2), offset);
//Log("\tNeed to wait %d us\n", us_to_next);
//Log("\tStart to wait @%d us\n", timediff(start_time, &now2));
//Log("EXIT @%d us\n", timediff(start_time, &now3));
}
