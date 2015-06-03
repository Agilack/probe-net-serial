/**
 * NetSerial - Unit test for UART2 and UART3
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

#define USE_DMA 1

#define BAUD_1200   0x6C00
#define BAUD_2400   0x3600
#define BAUD_4800   0x1B00
#define BAUD_9600   0x0D80
#define BAUD_19200  0x06C0
#define BAUD_38400  0x0360
#define BAUD_57600  0x0240
#define BAUD_115200 0x0120
#define BAUD_230400 0x0090
#define BAUD_460800 0x0048
#define BAUD_921600 0x0024

void uart_init   (void);
int  uart_isready(void);
int  uart_rd     (void);

void uart_crlf    (void);

void uart2_config(u16 speed);
void uart2_putc(u8 c);
void uart2_puts(char *s);
void uart2_putm(char *s, int len);
inline int uart2_isready(void);
inline int uart2_rd(void);

void uart3_config(u16 speed);
void uart3_putc(u8 c);
void uart3_puts(char *s);
void uart3_putm(char *s, int len);
inline int uart3_isready(void);
inline int uart3_rd(void);

#endif
