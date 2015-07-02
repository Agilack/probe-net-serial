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

int main (void)
{
	unsigned char i;

	hw_init();

	while (1) 
	{
		/* Set P1.1 (display reset) */
		P1 = 0x02;
		for (i = 0; i < 0xFF; i++)
			;
		/* Clear P1.1 (display reset) */
		P1 = 0x00;
		for (i = 0; i < 0xFF; i++)
			;
	}
}
