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
#ifndef API_H
#define API_H

#include "hardware.h"
#include "api_ip.h"

void (*uart_putc)(u8 c);
void (*uart_puts)(char *s);
void (*uart_puthex)(const u32 c);
int  (*eth_init)(void);
void (*eth_periodic)(void);
int  (*eth_status)(void);
void (*eth_set_rx)(u8 *addr);
void (*eth_set_tx)(u8 *addr);
void (*eth_set_callback)(u32 addr);
void (*eth_stack_init)(void);
void (*ip_tx)(int len);
void (*ip_prepare)(ip_hdr *pkt);
void (*ip_proto_register)(u8 proto, u32 fct);
u16  (*ip_cksum)(u32 sum, const u8 *data, u16 len);
u16  (*htons)(u16 n);  
u32  (*htonl)(u32 n);
void (*dhcp_periodic)(void);

void api_init(void);

#endif
