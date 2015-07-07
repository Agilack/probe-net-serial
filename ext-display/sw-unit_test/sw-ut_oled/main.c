/**
 * NetSerial Display Extention - Unit test
 *
 * Copyright (c) 2015 Saint-Genest Gwenael <gwen@agilack.fr>
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation. (See COPYING.GPL for details.)
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <efm8bb1.h>
#include "hardware.h"
#include "img.h"

void spi_cs(int en);
void spi_wr(unsigned char c);

void disp_init(void);

int main (void)
{
	unsigned char c;
	int i;

	hw_init();

	disp_init();

	for (i = 0; i < 512; i++)
	{
		spi_cs(1);
		spi_wr( img_p1[i] );
		spi_cs(0);
	}

	while (1)
	{
		/* If the UART Receive Interrupt Flag is set */
		if (SCON0_RI)
		{
			/* Read the received byte */
			c = SBUF0;
			
			/* Send the byte as OLED data */
			spi_cs(1);
			spi_wr(c);
			spi_cs(0);
			
			/* Clear the RI flag */
			SCON0_RI = 0;
		}
	}
}

void disp_init(void)
{
	unsigned char i;

	// Set OLED mode to CMD
	P1_0 = 0;

	// Reset the OLED (pulse on RES#)
	P1_1 = 0;
	for (i = 0; i < 0xF0; i++)
		;
	P1_1 = 1;
	for (i = 0; i < 0xF0; i++)
		;

	/* Set Display Off */
	spi_cs(1);
	spi_wr(0xAE);
	spi_cs(0);
	/* Set Clock */
	spi_cs(1);
	spi_wr(0xD5);
	spi_wr(0x80);
	spi_cs(0);
	/* Set Multiplex Ratio */
	spi_cs(1);
	spi_wr(0xA8);
	spi_wr(0x1F);
	spi_cs(0);
	/* Set Display Offset */
	spi_cs(1);
	spi_wr(0xD3);
	spi_wr(0x00);
	spi_cs(0);
	/* Set Display Start Line */
	spi_cs(1);
	spi_wr(0x40);
	spi_cs(0);
	/* Configure Charge Pump  */
	spi_cs(1);
	spi_wr(0x8D);
	spi_wr(0x14);
	spi_cs(0);
	/* Set Segment Remap */
	spi_cs(1);
	spi_wr(0xA1);
	spi_cs(0);
	/* COM pins hardware configuration */
	spi_cs(1);
	spi_wr(0xDA);
	spi_wr(0x00);
	spi_cs(0);
	/* Set Contrast Control */
	spi_cs(1);
	spi_wr(0x81);
	spi_wr(0xCF);
	spi_cs(0);
	/* Set precharge period */
	spi_cs(1);
	spi_wr(0xD9);
	spi_wr(0x1F);
	spi_cs(0);
	/* Set VCOMH deselect level */
	spi_cs(1);
	spi_wr(0xDB);
	spi_wr(0x40);
	spi_cs(0);
	/* Set Entire Display On/Off */
	spi_cs(1);
	spi_wr(0xA4);
	spi_cs(0);
	/* Set Horizontal Adressing Mode */
	spi_cs(1);
	spi_wr(0x20);
	spi_wr(0x00);
	spi_cs(0);
	/* Delay ... */
	for (i = 0; i < 0xFF; i++)
		;
	for (i = 0; i < 0xFF; i++)
		;
	/* Set Display On */
	spi_cs(1);
	spi_wr(0xAF);
	spi_cs(0);

	// Set OLED mode to DATA
	P1_0 = 1;

	return;
}
