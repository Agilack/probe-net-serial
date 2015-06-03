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
#ifndef NET_IP_H
#define NET_IP_H

#include "hardware.h"
#include "main.h"

#define PACK __attribute__((packed))

struct _eth_hdr
{
	u8 mac_d[6];
	u8 mac_s[6];
	u16 type;
} PACK;

typedef struct _eth_hdr eth_hdr;

/* IPv4 header. */
struct _ip_hdr
{
	u8  vhl;
	u8  tos;
	u16 len;
	u8  ipid[2];
	u8  ipoffset[2];
	u8  ttl;
	u8  proto;
	u16 cksum;
	u32 srcipaddr;
	u32 destipaddr;
} PACK;

typedef struct _ip_hdr ip_hdr;

/* UDP header. */
struct _udp_hdr
{
	u16 srcport;
	u16 destport;
	u16 len;
	u16 udpchksum;
} PACK;

typedef struct _udp_hdr udp_hdr;

struct _arp_hdr
{
	u16 type;
	u16 proto;
	u8  hlen;
	u8  llen;
	u16 op;
	u8  sphy[6];
	u32 sip;
	u8  dphy[6];
	u32 dip;
} PACK;

typedef struct _arp_hdr arp_hdr;

struct _mac_entry
{
	u32 ip;
	u8 phy[6];
};

typedef struct _mac_entry mac_entry;

struct _ip_proto
{
	u8 proto;
	void (*cb)(ip_hdr *datagram);
};

typedef struct _ip_proto ip_proto;

#undef DEBUG_IP_TX
#undef DEBUG_IP_RX

#ifdef DEBUG_IP_RX
#define D_IP_RX(x) do { uart_puts(x); } while(0);
#else
#define D_IP_RX(x) do { } while(0);
#endif

#ifdef DEBUG_IP_TX
#define D_IP_TX(x) do { uart_puts(x); } while(0);
#else
#define D_IP_TX(x) do { } while(0);
#endif

#endif
