/**
 * NetSerial - Unit test "Telnet server" using bootloader API
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
#include "main.h"
#include "uart.h"

u32 reg_rd(u32 reg)
{
	return ( *(volatile u32 *)reg );
}
void reg_wr(u32 reg, u32 value)
{
	*(volatile u32 *)reg = value;
}
void reg_set(u32 reg, u32 value)
{
	*(volatile u32 *)reg = ( *(volatile u32 *)reg | value );
}
void reg_clr(u32 reg, u32 value)
{
	*(volatile u32 *)reg = ( *(volatile u32 *)reg & ~value );
}

u32 glb_tim2;

void hw_tim2_init(void)
{
	glb_tim2 = 0;
	
	reg_set(RCC_APB1ENR, 0x01); /* Activate Timer2 */
	/* Configure Timer2 */
	reg_wr (TIM2_CR1, 0x0200); /* Input clock divided by 4 */
//	reg_wr (TIM2_ARR, 0x3680); /* Value used when reloaded */
	reg_wr (TIM2_ARR, 0x8000); /* Value used when reloaded */
	reg_wr (TIM2_PSC,   0x80); /* Prescaler       */
	reg_set(TIM2_DIER,  0x01); /* Set UIE bit     */
	/* NVIC: Enable interrupt 28 (TIM2) */
	reg_wr(0xE000E100, (1 << 28));
	/* Activate Timer2 (CEN=1) */
	reg_set(TIM2_CR1,     1);
}

void TIM2_IRQHandler(void)
{
	/* Increment counter */
	glb_tim2++;
	
//	uart_putc('*');
	
	/* Clear event */
	reg_clr(TIM2_SR, 1);
}
/* EOF */
