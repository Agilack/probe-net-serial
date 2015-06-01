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
#include "hardware.h"
#include "main.h"
#include "uart.h"

#include "ip.h"
#include "dhcp.h"
#include "tftp.h"

u32 host_ip;

#define NB_PROTO 3
ip_proto prot[NB_PROTO];

void ip_proto_register(u8 proto, u32 fct)
{
	int i;
	
	for (i = 0; i < NB_PROTO; i++)
	{
		if ((prot[i].proto == proto) ||
		    (prot[i].proto == 0) )
		{
			prot[i].proto = proto;
			prot[i].cb    = (void(*)())fct;
			break;
		}
	}
}

void ip_prepare(ip_hdr *pkt)
{
	pkt->vhl = 0x45;
	pkt->tos = 0x00;
	pkt->ttl = 0x40;
	pkt->ipoffset[0] = 0;
	pkt->ipoffset[1] = 0;
	pkt->srcipaddr = htonl(host_ip);
}

void ip_tx(int len)
{
	eth_hdr *frame;
	ip_hdr  *dgram;
	u16 sum;
	
	dgram = (ip_hdr *)(net_tx_buf + sizeof(eth_hdr));
	
	dgram->len   = htons(len);
	dgram->cksum = 0;
	sum = ip_cksum(0, (u8 *)dgram, 20);
	dgram->cksum = htons(~sum);
	
	frame = (eth_hdr *)net_tx_buf;
	frame->type = htons(0x800);
	ip_arp_out();
	
	net_tx_len = (len + sizeof(eth_hdr));
	eth_tx_packet();
}

u16 ip_cksum(u32 sum, const u8 *data, u16 len)
{
	u16 t;
	const u8 *dataptr;
	const u8 *last_byte;

	dataptr = data;
	last_byte = data + len - 1;

	/* At least two more bytes */
	while(dataptr < last_byte)
	{	
		t = (dataptr[0] << 8) + dataptr[1];
		sum += t;
//		if(sum < t)
//			sum++; /* carry */
		dataptr += 2;
	}
	
	if (len & 1)
	{
		t = (dataptr[0] << 8) + 0;
		sum += t;
//		if(sum < t)
//			sum++; /* carry */
      }

	while (sum & 0xffff0000)
	{
		u16 plop;
		plop = (sum >> 16);
		sum = (sum & 0xFFFF) + plop;
	}
	
	/* Return sum in host byte order. */
	return (u16)sum;
}

void ip_mac(eth_hdr *frame, mac_entry *mac)
{
	if (mac)
	{
		frame->mac_d[0] = mac->phy[0];
		frame->mac_d[1] = mac->phy[1];
		frame->mac_d[2] = mac->phy[2];
		frame->mac_d[3] = mac->phy[3];
		frame->mac_d[4] = mac->phy[4];
		frame->mac_d[5] = mac->phy[5];
	}
	else
	{
		frame->mac_d[0] = 0xFF;
		frame->mac_d[1] = 0xFF;
		frame->mac_d[2] = 0xFF;
		frame->mac_d[3] = 0xFF;
		frame->mac_d[4] = 0xFF;
		frame->mac_d[5] = 0xFF;
	}
}

void ip_rx(void)
{
	ip_hdr  *pkt_ip;
	int i;
	
	pkt_ip  = (ip_hdr *) ( net_rx_buf   + sizeof(eth_hdr) );
	
	ip_arp_in();
	
	for (i = 0; i < NB_PROTO; i++)
	{
		if (prot[i].proto == pkt_ip->proto)
		{
			prot[i].cb(pkt_ip);
			break;
		}
	}
	
	if (i == NB_PROTO)
	{
		uart_puts("  -> Unknown proto\r\n");
		dump((u8*)pkt_ip,  48);
	}
}

void udp_rx(ip_hdr *datagram)
{
	udp_hdr *pkt_udp;
	D_IP_RX(" UDP");
	pkt_udp = (udp_hdr *)( (u8 *)datagram + sizeof(ip_hdr) );

	if (pkt_udp->destport == htons(0x44))
	{
		D_IP_RX(" DHCP\r\n");
		dhcp_rx(datagram);
	}
	else if (pkt_udp->destport == htons(TFTP_PORT))
	{
		D_IP_RX(" TFTP\r\n");
		tftp_rx();
	}
}

u16 htons(u16 n)
{
	return (u16) ( (n << 8) | (n >> 8) );
}
u32 htonl(u32 n)
{
	union
	{
		u32 l;
		u8  s[4];
	} src, dest;
	
	src.l = n;
	dest.s[0] = src.s[3];
	dest.s[1] = src.s[2];
	dest.s[2] = src.s[1];
	dest.s[3] = src.s[0];
	
	return (dest.l);
}
/* EOF */
