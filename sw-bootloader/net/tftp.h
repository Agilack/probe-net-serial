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
#ifndef NET_TFTP_H
#define NET_TFTP_H
#include "hardware.h"
#include "main.h"

#include "spi.h"
#include "uart.h"
#include "eth.h"
#include "ip.h"

#define TFTP_PORT 6969

struct _tftp_packet
{
	eth_hdr eth;
	ip_hdr  ip;
	udp_hdr udp;
	
	u16  opcode;
	u8  data[514];
} PACK;

typedef struct _tftp_packet tftp_packet;

struct _tftp_data
{
	u16 id;
	u8  data[512];
} PACK;

typedef struct _tftp_data tftp_data;

void tftp_setfile(u8 *name);
void tftp_setserver(u32 ip);
void tftp_req(void);
void tftp_rx (void);
#endif
