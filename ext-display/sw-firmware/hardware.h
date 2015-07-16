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
#ifndef HARDWARE_H
#define HARDWARE_H

void hw_init(void);
void hw_init_spi(void);
void hw_init_timers(void);
void hw_init_uart(void);

typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

#endif
