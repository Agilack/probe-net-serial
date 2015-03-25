/*
 * NetSerial - Ethernet Unit Test
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
#include "spi.h"
#include "uart.h"
#include "eth.h"

int main(void)
{
	u32 tm_notify;
	
	hw_init();
	uart_init();
	spi_init();

	uart_puts("\r\n --=={ Ethernet Unit Test }==--\r\n");

	tm_notify    = 0;

	uart_puts("  - Init Ethernet: ");
	eth_init();
	uart_puts("ok\r\n");

	hw_tim2_init();
	
	tm_notify = get_time();

	while(1)
	{
		eth_periodic();
		
		if ( (get_time() > (tm_notify + 1000)) )
		{
			tm_notify = get_time();
			uart_puts("["); uart_puthex(tm_notify);
			uart_puts("] Wait for events... \r");
			
		}
	}

	while(1)
		;
}

extern u32 glb_tim2;
u32 get_time(void)
{
	return glb_tim2;
}

void EXTI9_5_IRQHandler(void)
{
	u32 pr;
	
	uart_putc('%');
	// Read the pendding register
	pr = reg_rd(EXTI_PR);
	
	if (pr & 0x00000100)
		eth_interrupt();
	
	// Clear the event
	reg_wr(EXTI_PR, pr);
	uart_putc('#');
}

void Global_Handler(u8 n)
{
	while ( (*(volatile u32 *)UART_SR & 0x80) != 0x80)
		;
	*(volatile u32 *)UART_DR = '!';
	while ( (*(volatile u32 *)UART_SR & 0x80) != 0x80)
		;
	*(volatile u32 *)UART_DR = n;
	while(1)
		;	
}

void EXTI15_10_IRQHandler(void)
{
	u32 pr;

	// Read the pendding register
	pr = reg_rd(EXTI_PR);
	
	// Clear the event
	reg_wr(EXTI_PR, pr);
}

/* EOF */
