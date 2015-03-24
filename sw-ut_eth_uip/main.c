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

#include "uip.h"

static int net_tx_pend;
extern u8 *net_tx_buf;
static int net_tx_len;

int main(void)
{
	u32 tm_notify;
	
	hw_init();
	uart_init();
	spi_init();

	uart_puts("\r\n --=={ Ethernet + uIP Unit Test }==--\r\n");

	tm_notify    = 0;
	net_tx_pend  = 0;

	uart_puts("  - Init Ethernet: ");
	eth_init();
	uart_puts("ok\r\n");

	uart_puts("  - Init uIP: ");
	eth_stack_init();
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

void net_wr(u8 *data, int len)
{
	int i;
	for (i = 0; i < len; i++)
		net_tx_buf[i] = data[i];
	net_tx_len = len;
	net_tx_pend = 1;
}

void net_app_callback(void)
{
	struct app_state *s = (struct app_state *)&(uip_conn->appstate);
	if( uip_connected() )
	{
		uart_puts("APP: connected\r\n");
		PSOCK_INIT(&s->sin,  s->inputbuf, sizeof(s->inputbuf) - 1);
		PSOCK_INIT(&s->sout, s->inputbuf, sizeof(s->inputbuf) - 1);
		PT_INIT(&s->outputpt);
		
		net_wr((u8 *)"Hello World!\r\n", 14);
	}
	if ( uip_newdata() && (uip_datalen() > 0) )
	{
		uart_puts("APP: newdata\r\n");
		
		uip_close();
	}
	if(uip_closed() || uip_aborted() || uip_timedout())
	{
		uart_puts("APP: closed\r\n");
	}
	
	if( uip_poll() )
	{
		if (net_tx_pend)
			uip_send(net_tx_buf, net_tx_len);
		net_tx_pend = 0;
	}
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
