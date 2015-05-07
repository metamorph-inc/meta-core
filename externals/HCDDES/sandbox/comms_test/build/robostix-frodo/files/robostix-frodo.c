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
#include <stdio.h>
#include <string.h>

#include "FreeRTOSConfig.h"
#include "portmacro.h"
#include "robostix-frodo.h"
#include "led.h"
#include "i2c.h"

#include "gpio.h"
#include "adc.h"
#include "uart0.h"
/*#include "math.h"*/

char* error[3]={"init OK\n\r","wrong parameter in the table\n\r","wrong per_entry pointer\n\r"};

extern unsigned char countdown;
extern void app_tasks_init();
/* end new */

/* Here comes the application specific stuff */

#define BAUDRATE 57600
#define PARITY 0
#define STOPBITS 0
#define DATABITS 3

#define ADC_CLOCK_DIV 6


//extern volatile task_entry tasks[];
//extern volatile msg_entry msgs[];
//extern volatile per_entry pers[];
//extern portTickType hp_len;

//#define Debug(buff) putstr(buff)
#define Debug(buff)

#include "app/app.c"

/* Priority definitions for most of the tasks in the frodo application.  Some
tasks might just use the idle priority. */
#define APP_TASK_PRIORITY			( tskIDLE_PRIORITY + 1 )
#define FRODO_TASK_PRIORITY		( tskIDLE_PRIORITY + 2 )



/*
 * The task function for the "FRODO" task.
 */
void frodo_task( void *parameters );

/*-----------------------------------------------------------*/
int stringcompare(char* a,char* b) {
  while ((*a!='\0') && (*b!='\0') && (*a++==*b++));
  if ((*a=='\0') && (*b=='\0')) return 0;
  else return 1;
}

portSHORT main( void )
{
	unsigned int err_idx;
	per_entry *per=pers;
	task_entry *task = tasks;

        /* Zero out port C  and the LED */
                 DDRC|=0xFF;
		  PORTC&=0x00;
                 LED_OFF( YELLOW );

	/* Setup the LED's for output. */
	led_init();

	/* Setup I2C */
	i2c_init();
	
	/* Setup UART */
	
	UART1_Init(BAUDRATE,PARITY,DATABITS,STOPBITS);
	Debug("\n\rInit peripherals...\n\r");
	/* begin new code */
	/*InnerLoop_100000016_init(&innerloop);*/
	while ((per->name != NULL)) {
	  if (stringcompare("GPIO",(char*)per->name) == 0) {
	    err_idx = GPIO_Init(per);
	    Debug("GPIO_Init: ");
	    Debug(error[err_idx]);
	  }
	  if (stringcompare("ADC",(char*)per->name) == 0) {
	    err_idx = ADC_Init(per,ADC_CLOCK_DIV);
	    Debug("ADC_Init: ");
	    Debug(error[err_idx]);
	  }
	  if (stringcompare("UART",(char*)per->name) == 0) {
	    err_idx = UART_Init_WF(per);
	    //if (per->port_name == 1) UART1_Init(BAUDRATE,PARITY,STOPBITS,DATABITS);
	    //else UART0_Init(BAUDRATE,PARITY,STOPBITS,DATABITS);
	    Debug("UART_Init: ");
	    Debug(error[err_idx]);
	  }
          per++;
	}
	/*per=pers;
	while ((per->name != NULL)) {
	  }
	  per++; // next item
	  };*/
	
	/* end new code */


	/* Create the FRODO task. */
	xTaskCreate( frodo_task, ( signed portCHAR * ) "FRODO", configMINIMAL_STACK_SIZE, NULL, FRODO_TASK_PRIORITY, NULL );

	/* Create application tasks. */
        app_tasks_init();
	while (task->task_fn) {
		vSemaphoreCreateBinary( task->semaphore );
		xSemaphoreTake(task->semaphore, portMAX_DELAY);
		xTaskCreate( task->task_fn, ( signed portCHAR * ) task->name, configMINIMAL_STACK_SIZE, task, APP_TASK_PRIORITY, NULL );
		task++;
	}

	/* Start kernel */
	vTaskStartScheduler();

	return 0;
}

/*-----------------------------------------------------------*/
void frodo_task( void *parameters )
{
	
	portTickType hp_begin;
	per_entry *per=pers;
	task_entry *task = tasks;
	msg_entry  *msg  = msgs;
	
	/* The parameters are not used. */
	( void ) parameters;
       
	for( ;; )
	{
                 DDRC|=0x10;
		  PORTC|=0x10;
		i2c_wait_for_sync();
                  DDRC|=0x20;
                  PORTC|=0x20;
		hp_begin = xTaskGetTickCount();

		/*if (countdown == 0)
                    LED_OFF( YELLOW );
                else
                   countdown--;*/

		per=pers;
		task = tasks;
		msg  = msgs;
	
		while ((task->task_fn != NULL) || (msg->id != MSG_ID_INVALID) || (per->name != NULL) ) {
		  if (task->task_fn != NULL) {
		    if ((per->name == NULL) || (task->offset <= per->offset)) {
		      if ((msg->id == MSG_ID_INVALID) || (task->offset <= msg->offset)) {
			vTaskDelay(hp_begin + task->offset - xTaskGetTickCount());
			xSemaphoreGive( task->semaphore );
			task++;
			continue; 
		      }
		    }
		  }
		  if (per->name != NULL) {
		    if ((msg->id == MSG_ID_INVALID) || (per->offset <= msg->offset)) {
		      vTaskDelay(hp_begin + per->offset - xTaskGetTickCount());
		      per->work_function(per);
		      per++;
		      continue;
		    }
		  }
		  vTaskDelay(hp_begin + msg->offset - xTaskGetTickCount());
		  i2c_process_msg(msg);
		  msg++;
		}
		/* end new */
		/* original
		while ((task->task_fn != NULL) || (msg->id != MSG_ID_INVALID) ) {
			if (task->task_fn != NULL) {
				if ((msg->id == MSG_ID_INVALID) || (task->offset <= msg->offset)) {
					vTaskDelay(hp_begin + task->offset - xTaskGetTickCount());
					xSemaphoreGive( task->semaphore );
					task++;
					continue;
				}
			}
			vTaskDelay(hp_begin + msg->offset - xTaskGetTickCount());
			i2c_process_msg(msg);
			msg++;
		}
		*/
		LED_OFF(YELLOW);
		vTaskDelay(hp_begin + hp_len - xTaskGetTickCount());
                PORTC&=~0x30;
	}
}
