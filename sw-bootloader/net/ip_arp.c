
#include "hardware.h"
#include "main.h"
#include "ip.h"
#include "uart.h"

mac_entry mac_cache[4];

void ip_arp_init(void)
{
	mac_cache[0].ip = 0xffffffff;
	mac_cache[0].phy[0] = 0xff;
	mac_cache[1].phy[1] = 0xff;
	mac_cache[2].phy[2] = 0xff;
	mac_cache[3].phy[3] = 0xff;
	mac_cache[4].phy[4] = 0xff;
	mac_cache[5].phy[5] = 0xff;
	
	mac_cache[1].ip = 0x00000000;
	mac_cache[2].ip = 0x00000000;
	mac_cache[3].ip = 0x00000000;
}

void ip_arp_in(void)
{
	eth_hdr *frame;
	ip_hdr  *dgram;
	int i, j;
	int id = -1;
	u32 ip_src;
	
	frame = (eth_hdr *)net_rx_buf;
	if (frame->type != 0x0008)
	{
		uart_puts("ip_arp_in() no ip\r\n");
		return;
	}
	
	dgram = (ip_hdr *)(net_rx_buf + sizeof(eth_hdr));
	ip_src = htonl(dgram->srcipaddr);
	
	for (i = 0; i < 4; i++)
	{
		if ( (id == -1) && (mac_cache[i].ip == 0) )
			id = i;
		
		if (mac_cache[i].ip != ip_src)
			continue;
		break;
	}
	
	if (i == 4)
	{
		mac_cache[id].ip = ip_src;
		for (j = 0; j < 6; j++)
			mac_cache[id].phy[j] = frame->mac_s[j];
	}
}

void ip_arp_out(void)
{
	eth_hdr *frame;
	ip_hdr  *dgram;
	int i, j;
	u32 ip_dst;
	
	frame = (eth_hdr *)net_tx_buf;
	
	if (frame->type != 0x0008)
	{
		uart_puts("ip_arp_out() no ip\r\n");
		return;
	}
	
	dgram = (ip_hdr *)(net_tx_buf + sizeof(eth_hdr));
	ip_dst = htonl(dgram->destipaddr);
	
	for (i = 0; i < 4; i++)
	{
		if (mac_cache[i].ip != ip_dst)
			continue;
//		uart_puts("ip_arp_out() found "); uart_puthex8(i);
		for (j = 0; j < 6; j++)
			frame->mac_d[j] = mac_cache[i].phy[j];
		break;
	}
	if (i == 4)
	{
		uart_puts("ip_arp_out() not found ");
		uart_puthex(ip_dst);
	}
//	uart_crlf();
}

void ip_arp_rx(void)
{
	arp_hdr *pkt, *pkt_out;
	eth_hdr *e_out;
	int i;
	
	pkt = (arp_hdr *)(net_rx_buf + sizeof(eth_hdr));
	if (pkt->dip != htonl(host_ip))
	{
		uart_puts("ip_arp_rx() unknown ");
		uart_puthex(htonl(pkt->dip));
		uart_crlf();
		dump( (u8 *)pkt, sizeof(arp_hdr) );
		uart_crlf();
		return;
	}
	
	e_out   = (eth_hdr *)net_tx_buf;
	e_out->type = htons(0x0806);
	pkt_out = (arp_hdr *)(net_tx_buf + sizeof(eth_hdr));
	pkt_out->type  = htons(0x0001);
	pkt_out->proto = htons(0x0800);
	pkt_out->hlen  = 6;
	pkt_out->llen  = 4;
	pkt_out->op    = htons(0x0002);
	pkt_out->sip   = htonl(host_ip);
	pkt_out->dip   = pkt->sip;
	for (i = 0; i < 6; i++)
		pkt_out->dphy[i] = pkt->sphy[i];
	for (i = 0; i < 6; i++)
		pkt_out->sphy[i] = host_mac[i];
		
	net_tx_len = sizeof(eth_hdr) + sizeof(arp_hdr);
	eth_tx_packet();
}
/* EOF */