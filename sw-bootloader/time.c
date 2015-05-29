/**
 * NetSerial - Bootloader
 *
 * Copyright (c) 2015 Saint-Genest Gwenael <gwen@agilack.fr>
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation. (See COPYING.GPL for details.)
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 */
#include "hardware.h"
#include "time.h"

static u32 time_count;

void time_init(void)
{
//	u32 val;
	time_count = 0;

	/* Configure Systick */
	reg_wr(SYSTICK_CTRL,  0x04); /* Set core clock as source */
	reg_wr(SYSTICK_LOAD, 80000); /* Approx 10ms              */
	reg_set(SYSTICK_CTRL, 0x01); /* Enable systick           */
	reg_set(SYSTICK_CTRL, 0x02); /* Activate tick interrupt  */

#ifdef DBG_TIME
	/* Configure debug pin PB3 */
	val  = reg_rd(GPIO_CRL(GPIOB));
	val &= 0xFFFF0FFF;
	val |= 0x00001000;
	reg_wr(GPIO_CRL(GPIOB), val);
#endif
}

void time_unload(void)
{
	/* Stop systick (and clear INT) */
	reg_clr(SYSTICK_CTRL, 0x03);
}

void SysTick_Handler(void)
{
	time_count ++;
#ifdef DBG_TIME
	if (time_count & 1)
		reg_wr(GPIO_BSRR(GPIOB), 0x00080000);
	else
		reg_wr(GPIO_BSRR(GPIOB), 0x00000008);
#endif
}

u32 get_time(void)
{
	return time_count;
}
/* EOF */
