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

void spi_cs(int en)
{
	if (en)
	{
		/* Set CS low */
		SPI0CN0_NSSMD0 = 0;
	}
	else
	{
		/* Wait if SPI is still active */
		while(SPI0CFG & 0x80)
			;
		/* Set CS high */
		SPI0CN0_NSSMD0 = 1;
		SPI0CN0_SPIF = 0;
	}
}

void spi_wr(unsigned char c)
{
	/* Wait if the TX buffer is full */
	while(SPI0CFG & 0x80)
		;
	/* Write the byte to TX buffer */
	SPI0DAT = c;
}

