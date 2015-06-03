/**
 * NetSerial - Unit test for UART2 and UART3
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
#include "api.h"
#include "uart.h"

static void uart2_init(void);
static void uart3_init(void);

static u8 key;

void uart_init(void)
{
	key = 0;
	
	uart2_init();
	uart3_init();

	reg_set(UART_CR1(UART1), 0x20); /* Set RXNEIE bit                */
	reg_set(0xE000E104, 0x20);      /* NVIC: Enable USART1 interrupt */
}

void USART1_IRQHandler(void)   
{
        u32 c;
        c = uart_rd();
        if ((key == 0) && ((c == 'a') || (c == 'A')) )
        {
        	key = 1;
        	uart_puts("Port A");
        	return;
	}
	if ((key == 0) && ((c == 'b') || (c == 'B')) )
	{
		key = 2;
		uart_puts("Port B");
		return;
	}
	if ((key > 0) && (c >= '0') && (c <= '9'))
	{
		u16 speed;
		uart_puts(" => ");
		switch(c)
		{
			case '0': speed = BAUD_1200;   uart_puts("1200");   break;
			case '1': speed = BAUD_4800;   uart_puts("4800");   break;
			case '2': speed = BAUD_9600;   uart_puts("9600");   break;
			case '3': speed = BAUD_19200;  uart_puts("19200");  break;
			case '4': speed = BAUD_38400;  uart_puts("38400");  break;
			case '5': speed = BAUD_57600;  uart_puts("57600");  break;
			case '6': speed = BAUD_115200; uart_puts("115200"); break;
			case '7': speed = BAUD_230400; uart_puts("230400"); break;
			case '8': speed = BAUD_460800; uart_puts("460800"); break;
			case '9': speed = BAUD_921600; uart_puts("921600"); break;
		}
		if (key == 1)
			uart2_config(speed);
		else
			uart3_config(speed);
		uart_puts("\r\n");
        }
        key = 0;
}

static void uart2_init(void)
{
	u32 val;
	
	/* Configure UART2-TX pin (PA2) */
	val  = reg_rd( GPIO_CRL(GPIOA) );
	val &= 0xFFFFF0FF;
	val |= 0x00000B00;
	reg_wr( GPIO_CRL(GPIOA), val);
	/* Configure UART2-RX pin (PA3) */
	val  = reg_rd( GPIO_CRL(GPIOA) );
	val &= 0xFFFF0FFF;
	val |= 0x00004000;
	reg_wr( GPIO_CRL(GPIOA), val);
	/* Activate USART2 */
	reg_set(RCC_APB1ENR, 0x00020000);
	
	reg_wr (UART_BRR(UART2), 0x0120); /* 115200 @ 33.1776MHz  */
	reg_set(UART_CR1(UART2), 0x0C);   /* Set TE & RE bits     */
	reg_set(UART_CR1(UART2), 0x2000); /* Set USART Enable bit */
}

static void uart3_init(void)
{
	u32 val;
	
	/* Enable GPIOB */
	reg_set(RCC_APB2ENR, 0x08);
	
	/* Configure UART3-TX pin (PB10) */
	val  = reg_rd( GPIO_CRH(GPIOB) );
	val &= 0xFFFFF0FF;
	val |= 0x00000B00;
	reg_wr( GPIO_CRH(GPIOB), val);
	/* Configure UART3-RX pin (PB11) */
	val  = reg_rd( GPIO_CRH(GPIOB) );
	val &= 0xFFFF0FFF;
	val |= 0x00004000;
	reg_wr( GPIO_CRH(GPIOB), val);
	/* Activate USART3 */
	reg_set(RCC_APB1ENR, 0x00040000);
	
	reg_wr (UART_BRR(UART3), 0x0120); /* 115200 @ 33.1776MHz  */
	reg_set(UART_CR1(UART3), 0x0C);   /* Set TE & RE bits     */
	reg_set(UART_CR1(UART3), 0x2000); /* Set USART Enable bit */
}

void uart2_config(u16 speed)
{
	/* baud = 33.1776MHz / (16 * BRR) */
	reg_wr (UART_BRR(UART2), speed);
}

void uart2_putc(u8 c)
{
	while ( (reg_rd(UART_SR(UART2)) & 0x80) != 0x80)
		;
	reg_wr(UART_DR(UART2), c);
}

void uart2_puts(char *s)
{
	while (*s)
	{
		uart2_putc(*s);
		s++;
	}
}

inline int uart2_isready(void)
{
	return (reg_rd(UART_SR(UART2)) & 0x20);
}

inline int uart2_rd(void)
{
	return (reg_rd(UART_DR(UART2)) & 0xFF);
}

void uart3_config(u16 speed)
{
	/* baud = 33.1776MHz / (16 * BRR) */
	reg_wr (UART_BRR(UART3), speed);
}

void uart3_putc(u8 c)
{
	while ( (reg_rd(UART_SR(UART3)) & 0x80) != 0x80)
		;
	reg_wr(UART_DR(UART3), c);
}

void uart3_puts(char *s)
{
	while (*s)
	{
		uart3_putc(*s);
		s++;
	}
}

inline int uart3_isready(void)
{
	return (reg_rd(UART_SR(UART3)) & 0x20);
}

inline int uart3_rd(void)
{
	return (reg_rd(UART_DR(UART3)) & 0xFF);
}

int uart_isready(void)
{
	return (reg_rd(UART_SR(UART1)) & 0x20);
}
int uart_rd(void)
{
	return (reg_rd(UART_DR(UART1)) & 0xFF);
}
void uart_crlf (void)
{
	uart_putc('\r');
	uart_putc('\n');
}
/* EOF */
