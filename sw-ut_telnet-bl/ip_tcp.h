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
#ifndef IP_TCP_H
#define IP_TCP_H

#include "hardware.h"
#include "main.h"
#include "api_ip.h"

#define PACK __attribute__((packed))

/* TCP header. */
struct _tcp_hdr
{
	u16 srcport;
	u16 destport;
	u32 seq;
	u32 ack;
	u8  offset;
	u8  flags;
	u16 win;
	u16 cksum;
	u16 urgent;
} PACK;

typedef struct _tcp_hdr tcp_hdr;

struct _tcp_pseudo
{
	u32 src;
	u32 dst;
	u8  rsv;
	u8  proto;
	u16 len;
} PACK;

typedef struct _tcp_pseudo tcp_pseudo;

struct _tcp_conn
{
	u8 state;
	u32 remote;
	u16 rem_port;
	u16 loc_port;
	u32 seq;
	u32 ack; /* Save the last ack */
	u8  *buffer_tx;
};
typedef struct _tcp_conn tcp_conn;

void tcp_init(void);
void tcp_close(tcp_conn *c);
u8  *tcp_buffer_tx(tcp_conn *c, int *len);
void tcp_rx(ip_hdr *datagram);
void tcp_tx(tcp_conn *c, u16 flags, int len);
void tcp_send(tcp_conn *c, u8 *d, int len);

#endif
