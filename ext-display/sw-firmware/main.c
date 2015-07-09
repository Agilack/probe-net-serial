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

	hw_init();

	disp_init();

#ifdef USE_SPLASH
	disp_splash();
#else
	disp_clear();
#endif

	SCON0_RI = 0;
	
	while (1)
	{
		/* If the UART Receive Interrupt Flag is set */
		if (SCON0_RI)
		{
			/* Read the received byte */
			c = SBUF0;
			/* Clear the RI flag */
			SCON0_RI = 0;
			
			/* Send the byte to character generator */
			disp_wr(c);
		}
	}
}
