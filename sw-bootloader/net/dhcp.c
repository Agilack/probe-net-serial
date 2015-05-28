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
#include "spi.h"
#include "uart.h"
#include "eth.h"

#include "ip.h"
#include "dhcp.h"
#include "tftp.h"

static const u8 xid[4] = {0xad, 0xde, 0x12, 0x23};
static const u8 magic_cookie[4] = {99, 130, 83, 99};

static u8 state = 0;

static void dhcp_send_req(dhcp_packet *pkt);

void dhcp_init(void)
{
	state = 0;
}

void dhcp_periodic(void)
{
	if (state == 0)
		dhcp_req();
}

int dhcp_status(void)
{
	return state;
}

void dhcp_req(void)
{
	int jj;
	dhcp_packet *pkt;
	
	uart_puts("DHCP: REQ...\r\n");
	state = 1;

	for (jj = 0; jj < 64; jj++)
		net_tx_buf[jj] = 0;
	pkt = (dhcp_packet *)net_tx_buf;
	ip_mac( (eth_hdr *)net_tx_buf, 0 );
	pkt->eth.type = htons(0x800);
	ip_prepare(&pkt->ip);
	pkt->ip.proto = 17;
	pkt->ip.len   = htons( sizeof(dhcp_packet) - sizeof(eth_hdr) );
	pkt->ip.destipaddr = 0xFFFFFFFF;
	pkt->udp.srcport = htons(68);
	pkt->udp.destport= htons(67);
	pkt->udp.len = htons( sizeof(dhcp_packet) - sizeof(eth_hdr) - sizeof(ip_hdr) );
	dhcp_send_req(pkt);
	ip_tx(sizeof(dhcp_packet));
}

static void dhcp_send_req(dhcp_packet *pkt)
{
	u32 nip;
	
	nip = htonl(host_ip);
	
	pkt->op    = DHCP_REQUEST;
	pkt->htype = DHCP_HTYPE_ETHERNET;
	pkt->hlen  = DHCP_HLEN_ETHERNET;
	pkt->hops  = 0;
	memcpy(&pkt->xid, xid, sizeof(pkt->xid));
	pkt->sec   = 0;
	pkt->flags = htons(BOOTP_BROADCAST); /* Broadcast bit. */
	memcpy(&pkt->ciaddr, &nip, sizeof(u32));
	memset(&pkt->yiaddr, 0, sizeof(pkt->yiaddr));
	memset(&pkt->siaddr, 0, sizeof(pkt->siaddr));
	memset(&pkt->giaddr, 0, sizeof(pkt->giaddr));
	
	memcpy(pkt->chaddr, host_mac, 6);
	memset(&pkt->chaddr[6], 0, sizeof(pkt->chaddr) - 6);
	memcpy(pkt->options, magic_cookie, sizeof(magic_cookie));
}

void dhcp_rx(ip_hdr *pkt)
{
	dhcp_packet *d = (dhcp_packet *)( (u8 *)pkt - sizeof(eth_hdr) );
	
	if (d->op != DHCP_REPLY)
	{
		uart_puts("  unknown op ");
		uart_puthex(d->op);
		uart_crlf();
		return;
	}
	host_ip = htonl( d->yiaddr );
	
	uart_puts("DHCP: Your IP ");
	uart_puthex(host_ip); uart_crlf();
	
	state = 2;
	
	if (d->file[0] != 0x00)
	{
		tftp_setfile(d->file);
		tftp_setserver(htonl(d->siaddr));
		tftp_req();
		state |= 0x80;
	}
}
/* EOF */
