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
#ifndef SPI_H
#define SPI_H

void spi_init(void);
void spi_open(u32 port);
void spi_wr  (u32 port, u32 w);
void spi_wait(u32 port);

void spi1_cs(int en);
void spi1_wr(u32 data);
u8   spi1_rd(void);
void spi1_wait(void);
void spi1_flush(void);

void spi2_cs(int en);
void spi2_wr(u32 data);
u8   spi2_rd(void);
void spi2_wait(void);
void spi2_flush(void);

#endif
