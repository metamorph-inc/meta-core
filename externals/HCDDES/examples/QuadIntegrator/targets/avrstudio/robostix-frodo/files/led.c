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
 
#include "led.h"

/*-----------------------------------------------------------*/

void led_init(void)
{
	RED_LED_DDR |= RED_LED_MASK;
	BLUE_LED_DDR |= BLUE_LED_MASK;
	YELLOW_LED_DDR |= YELLOW_LED_MASK;
	DBG0_LED_DDR |= DBG0_LED_MASK;
	DBG1_LED_DDR |= DBG1_LED_MASK;
	DBG2_LED_DDR |= DBG2_LED_MASK;

	LED_OFF(RED);
	LED_OFF(BLUE);
	LED_OFF(YELLOW);
	LED_OFF(DBG0);
	LED_OFF(DBG1);
	LED_OFF(DBG2);
}
