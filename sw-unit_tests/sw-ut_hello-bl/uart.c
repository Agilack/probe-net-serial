/**
 * NetSerial - Unit test "Hello world" with bootloader
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
#include "uart.h"

void uart_init(void)
{
	u32 val;

	reg_set(RCC_APB2ENR,   0x04); /* Activate GPIOA */

	/* Configure UART-TX pin */
	val  = reg_rd(GPIOA + 4);
	val &= 0xFFFFFF0F;
	val |= 0x000000B0;
	reg_wr(GPIOA + 4, val);
	
	/* Configure UART-RX pin */
	val  = reg_rd(GPIOA + 4);
	val &= 0xFFFFF0FF;
	val |= 0x00000400;
	reg_wr(GPIOA + 4, val);

	reg_set(RCC_APB2ENR, 0x4000); /* Activate USART1 */
	
	reg_wr(UART_BRR, 0x0045);  /* 115200 @ 8MHz */
//	reg_wr(UART_BRR, 0x0138);  /* 115200 @ 36MHz       */
	reg_set(UART_CR1, 0x0C);   /* Set TE & RE bits     */
	reg_set(UART_CR1, 0x2000); /* Set USART Enable bit */
	
	reg_set(UART_CR1,   0x20); /* Set RXNEIE bit                */
	reg_set(0xE000E104, 0x20); /* NVIC: Enable USART1 interrupt */
}

void USART1_IRQHandler(void)
{
	u32 c;
	c = uart_rd();
	uart_puts("<"); uart_puthex8(c); uart_puts(">");
}

void uart_putc(u8 c)
{
	while ( (reg_rd(UART_SR) & 0x80) != 0x80)
		;
	reg_wr(UART_DR, c);
}
int uart_isready(void)
{
	return (reg_rd(UART_SR) & 0x20);
}
int uart_rd(void)
{
	return (reg_rd(UART_DR) & 0xFF);
}
void uart_puts (char *s)
{
	while (*s)
	{
		uart_putc(*s);
		s++;
	}
}
void uart_crlf (void)
{
	uart_putc('\r');
	uart_putc('\n');
}
void uart_puthex (const u32 c)
{
	u8 hex[20] = "0123456789ABCDEF";
	
	uart_putc( hex[(c >> 28) & 0xF] );
	uart_putc( hex[(c >> 24) & 0xF] );
	uart_putc( hex[(c >> 20) & 0xF] );
	uart_putc( hex[(c >> 16) & 0xF] );
	uart_putc( hex[(c >> 12) & 0xF] );
	uart_putc( hex[(c >>  8) & 0xF] );
	uart_putc( hex[(c >>  4) & 0xF] );
	uart_putc( hex[(c      ) & 0xF] );
}
void uart_puthex8(const u32 c)
{
	u8 hex[20] = "0123456789ABCDEF";
	
	uart_putc( hex[(c >>  4) & 0xF] );
	uart_putc( hex[(c      ) & 0xF] );
}
void uart_puthex16(const u32 c)
{
	u8 hex[20] = "0123456789ABCDEF";
	
	uart_putc( hex[(c >> 12) & 0xF] );
	uart_putc( hex[(c >>  8) & 0xF] );
	uart_putc( hex[(c >>  4) & 0xF] );
	uart_putc( hex[(c      ) & 0xF] );
}
/* EOF */
