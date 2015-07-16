/**
 * NetSerial Display Extention - Character firmware
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
#include "spi.h"
#include "display.h"

static u8 h2b(u8 c);

int main (void)
{
	u8  c;
	u8  nibble;
	int state;
	
	hw_init();

	disp_init();

#ifdef USE_SPLASH
	disp_splash();
#else
	disp_clear();
#endif

	SCON0_RI = 0;
	state = 0;
	nibble = 0;
	P0_6;
	
	while (1)
	{
		/* If the UART Receive Interrupt Flag is set */
		if (SCON0_RI)
		{
			P0_6 = 1;
			/* Read the received byte */
			c = SBUF0;
			/* Clear the RI flag */
			SCON0_RI = 0;
			
			if ((c == '\r') || (c == '\n'))
			{
				if (state > 1)
					disp_line(0);
				state = 0;
				P0_6 = 0;
				continue;
			}
			if (state == 0)
			{
				state = 1;
				if ((c >= '0') && (c <= '3'))
				{
					disp_line(c - '0');
					P0_6 = 0;
					continue;
				}
				if (c == '8')
				{
					state = 2;
					disp_draw_front(1);
					P0_6 = 0;
					continue;
				}
				if (c == '9')
				{
					state = 0;
					disp_draw_front(0);
					P0_6 = 0;
					continue;
				}
				if (c == 0x1B)
				{
					disp_clear();
					state = 0;
					P0_6 = 0;
					continue;
				}
				state = 99;
				continue;
			}
			if (state == 2)
			{
				nibble = h2b(c) << 4;
				state = 3;
			}
			else if (state == 3)
			{
				nibble |= h2b(c);
				disp_wr(nibble);
				state = 2;
			}
			else if (state == 1)
			{
				/* Send the byte to character generator */
				disp_putc(c);
			}
			P0_6 = 0;
		}
	}
}

static u8 h2b(u8 c)
{
	if ((c >= '0') && (c <= '9'))
		return (c - '0');
	if ((c >= 'a') && (c <= 'f'))
		return (c - 'a' + 10);
	if ((c >= 'A') && (c <= 'A'))
		return (c - 'A' + 10);
	
	return(0);
}
