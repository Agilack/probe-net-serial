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

int main (void)
{
	unsigned char c;
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
				state = 0;
				P0_6 = 0;
				continue;
			}
			if (state == 0)
			{
				state = 1;
				if ((c >= '0') && (c <= '9'))
				{
					disp_line(c - '0');
					P0_6 = 0;
					continue;
				}
			}
			
			/* Send the byte to character generator */
			disp_wr(c);
			P0_6 = 0;
		}
	}
}
