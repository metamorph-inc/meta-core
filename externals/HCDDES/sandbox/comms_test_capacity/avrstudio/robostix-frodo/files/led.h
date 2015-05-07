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

#ifndef LED_H
#define LED_H

#include "FreeRTOS.h"
#include "task.h"

// Visible LEDs

#define RED_LED_PIN     4
#define RED_LED_MASK    ( 1 << RED_LED_PIN )
#define RED_LED_DDR     DDRG
#define RED_LED_PORT    PORTG

#define BLUE_LED_PIN    3
#define BLUE_LED_MASK   ( 1 << BLUE_LED_PIN )
#define BLUE_LED_DDR    DDRG
#define BLUE_LED_PORT   PORTG

#define YELLOW_LED_PIN  4
#define YELLOW_LED_MASK ( 1 << YELLOW_LED_PIN )
#define YELLOW_LED_DDR  DDRB
#define YELLOW_LED_PORT PORTB

// "DEBUG" LEDs - used with the scope

#define DBG0_LED_PIN     0
#define DBG0_LED_MASK    ( 1 << DBG0_LED_PIN )
#define DBG0_LED_DDR     DDRG
#define DBG0_LED_PORT    PORTG

#define DBG1_LED_PIN    1
#define DBG1_LED_MASK   ( 1 << DBG1_LED_PIN )
#define DBG1_LED_DDR    DDRG
#define DBG1_LED_PORT   PORTG

#define DBG2_LED_PIN  2
#define DBG2_LED_MASK ( 1 << DBG2_LED_PIN )
#define DBG2_LED_DDR  DDRG
#define DBG2_LED_PORT PORTG

//#define LED_ON( color )     do { vTaskSuspendAll(); color ## _LED_PORT &= ~color ## _LED_MASK; xTaskResumeAll(); } while (0)
//#define LED_OFF( color )    do { vTaskSuspendAll(); color ## _LED_PORT |= color ## _LED_MASK; xTaskResumeAll();} while (0)
//#define LED_TOGGLE( color ) do { vTaskSuspendAll(); color ## _LED_PORT ^= color ## _LED_MASK; xTaskResumeAll();} while (0)

#define LED_ON( color )     do { color ## _LED_PORT &= ~color ## _LED_MASK; } while (0)
#define LED_OFF( color )    do { color ## _LED_PORT |= color ## _LED_MASK; } while (0)
#define LED_TOGGLE( color ) do { color ## _LED_PORT ^= color ## _LED_MASK; } while (0)

void led_init(void);

#endif

