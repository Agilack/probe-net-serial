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
#include "main.h"
#include "spi.h"
#include "uart.h"
#include "eth.h"

void __MSR_MSP(u32 v);
static void boot_normal(void);

int main(void)
{
	u32 tm_notify;
	u32 key;
	int i;
	
	hw_init();
	uart_init();

	uart_puts("\r\n --=={ NetSerial }==--\r\n");

	key = 2;
	while(1)
	{
		key = key << 1;
		if (reg_rd(GPIO_IDR(GPIOA)) & 0x00002000)
			key = key | 1;
		if ((key & 0xFFFF) == 0x0000)
			break;
		if ((key & 0xFFFF) == 0xFFFF)
			break;
		for (i = 0; i < 0x1000; i++)
			;
	}
	
	if (key & 1)
		boot_normal();
	
	uart_puts(" * Start bootloader mode...\r\n");

	tm_notify    = 0;

	hw_flash_unlock();
	spi_init();
	uart_puts("  - Init Ethernet: ");
	eth_init();
	uart_puts("ok\r\n");

	uart_puts("  - Init stack: ");
	eth_stack_init();
	uart_puts("ok\r\n");

	hw_tim2_init();
	
	tm_notify   = get_time();

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
}

static void boot_normal(void)
{
	void (*fpnt)(void);
	u32 ad;
	
	uart_puts(" * Start main firmware...\r\n");
	
	/* Get the address of the Reset vector */
	ad = *(u32 *)0x08005004;
	fpnt = (void (*)(void))ad;
	uart_puthex(ad); uart_crlf();
	
	/* Update MSP */
	__MSR_MSP(*(vu32 *)0x08005000);
	/* Call it ...*/
	fpnt();
	
	while(1);
}

void *memcpy(void *dst, const void *src, int n)
{
	u8 *s;
	u8 *d;
	s = (u8*)src;
	d = (u8*)dst;
	while(n)
	{
		*d = *s;
		s ++;
		d ++;
		n --;
	}
	return(dst);
}

void *memset(void *dst, int value, int n)
{
	u8 *d;
	d = (u8 *)dst;
	while(n)
	{
		*d  = value;
		d++;
		n --;
	}
	return(dst);
}

char *strcpy(char *dest, const char *src)
{
	char *dsave = dest;
	while(*src)
	{
		*dest = *src;
		src++;
		dest++;
	}
	*dest = 0;
	return dsave;
}

int strlen(char *str)
{
	int count;
	count = 0;
	while(*str)
	{
		count ++;
		str++;
	}
	return(count);
}

void dump (u8* adr, int len)
{
	int i, j;
	
	for (j = 0; j < len; )
	{
		uart_puthex((u32)adr);
		uart_putc(' ');
		for (i = 0; i < 16; i++)
		{
			uart_puthex8(*adr);
			adr++;
			uart_putc(' ');
			j++;
			if (j == len)
				break;
		}
		uart_crlf();
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
