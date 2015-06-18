/**
 * NetSerial - Unit test "Telnet server" using bootloader API
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
#ifndef _SPI_H_
#define _SPI_H_

typedef struct _s_spi
{
	void (*cs)    (int en);
	u8   (*rd)    (void);
	void (*wr)    (u32 data);
	void (*wait)  (void);
	void (*flush) (void);
} s_spi;

void    spi_init(void);
s_spi * spi_open(u32 port);
void    spi_wr  (u32 port, u32 w);
void    spi_wait(u32 port);

#endif
