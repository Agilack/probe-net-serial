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
#include "hardware.h"
#include "main.h"
#include "uart.h"
#include "api_ip.h"
#include "api.h"
#include "ip_tcp.h"
#include "telnet.h"

#define TCP_NB_CONNS 2
tcp_conn conns[TCP_NB_CONNS];

void tcp_init(void)
{
	conns[0].state = 0;
	conns[1].state = 0;
	
}

void tcp_close(tcp_conn *c)
{
	tcp_tx(c, 0x01, 0);
}

u8 *tcp_buffer_tx(tcp_conn *c, int *len)
{
	u8 *buffer;

	buffer  = c->buffer_tx;
	buffer += sizeof(eth_hdr) + sizeof(ip_hdr) + sizeof(tcp_hdr);

	if (len)
		*len = 768;
	
	return(buffer);
}

void tcp_send(tcp_conn *c, u8 *d, int len)
{
	u8 *d_out;
	int max;
	int i;
	
	d_out = tcp_buffer_tx(c, &max);
	
	/* Test if tcp buffer is larger than input datas */
	if (len > max)
		/* Truncate input length to fit buffer size */
		len = max;
	
	/* Copy datas */
	for (i = 0; i < len; i++)
		d_out[i] = d[i];
	
	tcp_tx(c, 0, len);
}

void tcp_rx(ip_hdr *datagram)
{
	eth_hdr  *pout;
	tcp_hdr  *pkt_tcp;
	tcp_conn *p_conn;
	int ack_len;
	int event;
	int i;
	
	p_conn  = (tcp_conn *)0x00;
	ack_len = 0;
	event   = 0;
	
	pkt_tcp = (tcp_hdr *)( (u8 *)datagram + sizeof(ip_hdr) );
	
	/* If the SYN flag is set : new conn */
	if (pkt_tcp->flags & 0x02)
	{
		for (i = 0; i < TCP_NB_CONNS; i++)
		{
			if (conns[i].state == 0)
			{
				p_conn = &conns[i];
				break;
			}
		}
		if (p_conn == 0x00)
		{
			uart_puts("TCP conn: no more free socket\r\n");
			return;
		}
		p_conn->state = 1;
		p_conn->seq   = 1;
		p_conn->remote   = datagram->srcipaddr;
		p_conn->rem_port = pkt_tcp->srcport;
		p_conn->loc_port = pkt_tcp->destport;
		p_conn->buffer_tx = (u8 *)0x20004600;
		ack_len = 1;
		event = 1;
		goto send_ack;
	}
	
	for (i = 0; i < TCP_NB_CONNS; i++)
	{
		tcp_conn *c = &conns[i];
		if (c->state == 0)
			continue;
		if (c->remote != datagram->srcipaddr)
			continue;
		if (c->loc_port != pkt_tcp->destport)
			continue;
		if (c->rem_port != pkt_tcp->srcport)
			continue;
		p_conn = c;
		break;
	}
	if (p_conn == 0)
	{
		uart_puts("Unknown conn\r\n");
		return;
	}

	/* If the packet contains ACK */
	if (pkt_tcp->flags & 0x10)
	{
		if (p_conn->state == 10)
		{
			p_conn->state = 0;
			goto end;
		}
	}
	
	/* If the packet contains FIN */
	if (pkt_tcp->flags & 0x01)
	{
		ack_len = 1;
		event = 2;
		/* Mark connection as close */
		p_conn->state = 10;
		goto send_ack;
	}
	
	/* If the packet contains data */
	if (pkt_tcp->flags & 0x08)
	{
		int hlen;
		int len;
		
		hlen = (pkt_tcp->offset >> 2);
		len  = htons(datagram->len) - sizeof(ip_hdr) - hlen;
		
		ack_len = len;
		event = 3;
		goto send_ack;
	}
end:
	if (event == 1)
	{
		p_conn->seq++;
		if (htons(pkt_tcp->destport) == 23)
			telnet_accept(p_conn);
	}
	if (event == 2)
	{
		if (htons(pkt_tcp->destport) == 23)
			telnet_closed();
	}
	if (event == 3)
	{
		int hlen;
		int len;
		u8 *d;
		
		hlen = (pkt_tcp->offset >> 2);
		len  = htons(datagram->len) - sizeof(ip_hdr) - hlen;
		d = (u8 *)pkt_tcp + hlen;
		if (htons(pkt_tcp->destport) == 23)
			telnet_recv(d, len);
	}
	return;

send_ack:
	{
	ip_hdr  *ip;
	tcp_hdr *tcp;
	tcp_pseudo psd;
	u16 sum;
	
	pout = (eth_hdr *)0x20004600;
	ip  = (ip_hdr  *)( (u8*)pout + sizeof(eth_hdr));
	tcp = (tcp_hdr *)( (u8*)ip   + sizeof(ip_hdr) );
	ip_prepare(ip);
	ip->destipaddr = p_conn->remote;
	ip->proto      = 6;
	
	tcp->srcport  = p_conn->loc_port;
	tcp->destport = p_conn->rem_port;
	tcp->seq      = htonl( p_conn->seq );
	p_conn->ack = htonl(pkt_tcp->seq) + ack_len;
	tcp->ack      = htonl( p_conn->ack );
	tcp->offset   = (5 << 4);
	tcp->win      = htons(1500);
	tcp->urgent   = 0;
	tcp->flags    = 0x10;
	if (pkt_tcp->flags & 0x02)
		tcp->flags |= 0x02;
	if (pkt_tcp->flags & 0x01)
		tcp->flags |= 0x01;
	tcp->cksum = 0;
	/* Compute TCP checksum */
	psd.src   = ip->srcipaddr;
	psd.dst   = ip->destipaddr;
	psd.rsv   = 0;
	psd.proto = 6;
	psd.len   = htons( sizeof(tcp_hdr) );
	sum = ip_cksum(0, (u8 *)&psd, 12);
	sum = ip_cksum(sum, (u8 *)tcp, 20);
	tcp->cksum = htons(~sum);
	
	ip_tx(sizeof(ip_hdr) + sizeof(tcp_hdr));
	}
	goto end;
}


void tcp_tx (tcp_conn *c, u16 flags, int len)
{
	ip_hdr    *ip;
	tcp_hdr   *tcp;
	tcp_pseudo psd;
	u16        sum;
	
	ip  = (ip_hdr  *)( c->buffer_tx + sizeof(eth_hdr));
	tcp = (tcp_hdr *)( (u8*)ip   + sizeof(ip_hdr) );

	/* Config the IP datagram header */
	ip_prepare(ip);
	ip->destipaddr = c->remote;
	ip->proto      = 6;

	/* Configure the TCP packet header */
	tcp->srcport  = c->loc_port;
	tcp->destport = c->rem_port;
	tcp->seq      = htonl( c->seq );
	tcp->ack      = htonl( c->ack );
	tcp->offset   = (5 << 4);
	tcp->win      = htons(1500);
	tcp->urgent   = 0;
	tcp->flags    = 0x10 | flags;
	
	if (len)
	{
		/* Update flags (datas) */
		tcp->flags |= 0x08; 
		/* Update sequence counter */
		c->seq += len;
	}

	/* Compute TCP checksum */
	tcp->cksum = 0;
	psd.src   = ip->srcipaddr;
	psd.dst   = ip->destipaddr;
	psd.rsv   = 0;
	psd.proto = 6;
	psd.len   = htons( sizeof(tcp_hdr) + len );
	sum = ip_cksum(0, (u8 *)&psd, 12);
	sum = ip_cksum(sum, (u8 *)tcp, 20 + len);
	tcp->cksum = htons(~sum);
	
	/* Send datas */
	ip_tx(sizeof(ip_hdr) + sizeof(tcp_hdr) + len);
}

/* EOF */
