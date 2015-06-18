/**
 * NetSerial - OLED unit-test
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

#include "img.h"

int main(void)
{
	u32 val;
	int i;
	
	uart_puts("OLED Unit Test\r\n");
	
	/* Configure OLED-RES (GPIO A 11) */
	val  = reg_rd(GPIO_CRH(GPIOA));
	val &= 0xFFFF0FFF;
	val |= 0x00001000;
	reg_wr(GPIO_CRH(GPIOA), val);
	/* Configure OLED-C/D (GPIO A 12) */
	val  = reg_rd(GPIO_CRH(GPIOA));
	val &= 0xFFF0FFFF;
	val |= 0x00010000;
	reg_wr(GPIO_CRH(GPIOA), val);
	/* Set RES to zero (active) */
	reg_wr(GPIO_BSRR(GPIOA), (0x0800 << 16));
	for (i = 0; i < 0x2000; i++)
		;
	spi_open(SPI1);
	/* Set RES to 1 (exit reset) */
	reg_wr(GPIO_BSRR(GPIOA), 0x0800);
	
	uart_puts(" - Reset.\r\n");
	
	/* Set C/D to Command */
	reg_wr(GPIO_BSRR(GPIOA), (0x1000 << 16));
	/* Set Display Off */
	spi1_cs(1);
	spi1_wr(0xAE);
	spi1_cs(0);
	/* Set Clock */
	spi1_cs(1);
	spi1_wr(0xD5);
	spi1_wr(0x80);
	spi1_cs(0);
	/* Set Multiplex ratio */
	spi1_cs(1);
	spi1_wr(0xA8);
	spi1_wr(0x1F);
	spi1_cs(0);
	/* Set Display Offset */
	spi1_cs(1);
	spi1_wr(0xD3);
	spi1_wr(0x00);
	spi1_cs(0);
	/* Set Display start line */
	spi1_cs(1);
	spi1_wr(0x40);
	spi1_cs(0);
	/* Configure charge pump */
	spi1_cs(1);
	spi1_wr(0x8D);
	spi1_wr(0x14);
	spi1_cs(0);
	/* Set Segment remap */
	spi1_cs(1);
	spi1_wr(0xA1);
	spi1_cs(0);
	/* Configure COM pins hardware configuration */
	spi1_cs(1);
	spi1_wr(0xDA);
	spi1_wr(0x00);
	spi1_cs(0);
	/* Set contrast control */
	spi1_cs(1);
	spi1_wr(0x81);
	spi1_wr(0xCF);
	spi1_cs(0);
	/* Set pre-charge period */
	spi1_cs(1);
	spi1_wr(0xD9);
	spi1_wr(0x1F);
	spi1_cs(0);
	/* Set VCOMH deselect level */
	spi1_cs(1);
	spi1_wr(0xDB);
	spi1_wr(0x40);
	spi1_cs(0);
	/* Set Entire display On/Off */
	spi1_cs(1);
	spi1_wr(0xA4);
	spi1_cs(0);
	/* Set Horizontal adressing mode  */
	spi1_cs(1);
	spi1_wr(0x20);
	spi1_wr(0x00);
	spi1_cs(0);
	/* Delay */
	for(i = 0; i < 0x8000; i++);
	/* Set Display On */
	spi1_cs(1);
	spi1_wr(0xAF);
	spi1_cs(0);
	
	/* Set C/D to Data */
	reg_wr(GPIO_BSRR(GPIOA), 0x1000);
	uart_puts(" - Init complete\r\n");

	for (i = 0; i < 512; i++)
	{
		spi1_cs(1);
		spi1_wr(img[i]);
		spi1_cs(0);
	}

	while(1);
}
/* EOF */
