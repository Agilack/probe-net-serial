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
#ifndef NET_DHCP_H
#define NET_DHCP_H

#include "ip.h"
struct _dhcp_packet
{
	eth_hdr eth;
	ip_hdr  ip;
	udp_hdr udp;
	u8  op;
	u8  htype;
	u8  hlen;
	u8  hops;
	u32 xid;
	u16 sec;
	u16 flags;
	u32 ciaddr; /* Client IP (if known) */
	u32 yiaddr; /* Your IP address (filled by server) */
	u32 siaddr; /* Server IP address */
	u32 giaddr;
	u8  chaddr [16];
	u8  sname  [64];
	u8  file   [128];
	u8  options[120];
} PACK;

typedef struct _dhcp_packet dhcp_packet;

void dhcp_init(void);
void dhcp_periodic(void);
void dhcpc_appcall(void);
void dhcp_req(void);
void dhcp_rx(ip_hdr *pkt);

#define BOOTP_BROADCAST 0x8000
#define DHCP_REQUEST 1
#define DHCP_REPLY 2
#define DHCP_HTYPE_ETHERNET 1
#define DHCP_HLEN_ETHERNET 6
#define DHCP_MSG_LEN 236

#endif
