/**
 * NetSerial - Unit test "Hello world" with bootloader
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
#ifndef UART_H
#define UART_H

#include "hardware.h"

void uart_init   (void);
int  uart_isready(void);
int  uart_rd     (void);

void uart_putc    (u8 c);
void uart_puts    (char *s);
void uart_crlf    (void);
void uart_puthex8 (const u32 c);
void uart_puthex16(const u32 c);
void uart_puthex  (const u32 c);

#endif
