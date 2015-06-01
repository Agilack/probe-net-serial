
#include "hardware.h"
#include "main.h"
#include "eth.h"
#include "uart.h"
#include "tftp.h"

static char tftp_filename[32];
static u32  tftp_server;
static u16  tftp_last_pos;
static u16  status = 0;

void tftp_setfile(u8 *name)
{
	uart_puts("TFTP: File    ");
	uart_puts((char *)name);
	strcpy(tftp_filename, (char *)name);
	uart_crlf();
}

void tftp_setserver(u32 ip)
{
	uart_puts("TFTP: SrvIp : ");
	tftp_server = ip;
	uart_puthex(ip);
	uart_crlf();
}

int tftp_status(void)
{
	return status;
}

void tftp_req(void)
{
	tftp_packet *pkt;
	u8  *pnt;
	int i;

	uart_puts("TFTP: REQ...\r\n");

	/* Reset the counter to a non-zero value */
	tftp_last_pos = 0xFFFF;
	status   = 0x0001;

	for (i = 0; i < ETH_FRAME_SIZE; i++)
		net_tx_buf[i] = 0;
	
	pkt = (tftp_packet *)net_tx_buf;
	ip_mac( (eth_hdr *)net_tx_buf, 0 );
	ip_prepare(&pkt->ip);
	pkt->ip.proto = 17;
	pkt->ip.destipaddr  = htonl(tftp_server);
	pkt->ip.len = htons( sizeof(tftp_packet) - sizeof(eth_hdr) );
	pkt->udp.srcport = htons(TFTP_PORT);
	pkt->udp.destport= htons(69);
	pkt->udp.len = htons( sizeof(tftp_packet) - sizeof(eth_hdr) - sizeof(ip_hdr) );

	pkt->opcode = htons(0x01);
	pnt = pkt->data;
	strcpy((char *)pnt, (char *)tftp_filename);
	pnt += strlen(tftp_filename) + 1;
	strcpy((char *)pnt, (char *)"octet");

	ip_tx(sizeof(tftp_packet) - sizeof(eth_hdr));
}

void tftp_rx(void)
{
	tftp_packet *pkt;
	tftp_packet *pout;
	int i;
	
	pkt = (tftp_packet *)net_rx_buf;
//	uart_puts("TFTP: op="); uart_puthex16(htons(pkt->opcode));
//	uart_crlf();
	
	if (pkt->opcode == htons(0x03))
	{
		tftp_data *d_pkt;
		u32  addr;
		d_pkt = (tftp_data *)pkt->data;
		if (d_pkt->id != tftp_last_pos)
		{
			uart_puts("TFTP: Data packet "); uart_puthex16(htons(d_pkt->id));
			addr = (0x08005000 - 512); /* Minus 512 because 1st packet is '1' */
			addr += htons(d_pkt->id) << 9;
			uart_puts("  address "); uart_puthex(addr);
			uart_puts("  len ");     uart_puthex16(htons(pkt->udp.len));
			uart_crlf();
			if ((addr & 0x3FF) == 0)
			{
				uart_puts("Erase page\r\n");
				hw_flash_erase(addr);
			}
			hw_flash_wr(d_pkt->data, addr, 512);
			tftp_last_pos = d_pkt->id;
		}
		
		pout = (tftp_packet *)net_tx_buf;
		ip_mac( (eth_hdr *)net_tx_buf, 0 );
		ip_prepare(&pout->ip);
		pout->ip.proto = 17;
		pout->ip.destipaddr  = htonl(tftp_server);
		pout->ip.len = htons( sizeof(ip_hdr) + sizeof(udp_hdr) + 8);
		pout->ip.ipid[0] = 0;
		pout->ip.ipid[1] = 0;
		pout->ip.ipoffset[0] = 0;
		pout->ip.ipoffset[1] = 0;
		pout->udp.srcport = htons(TFTP_PORT);
		pout->udp.destport= pkt->udp.srcport;
		pout->udp.len = htons( sizeof(udp_hdr) + 8);
		pout->udp.udpchksum = 0x0000;
		
		pout->opcode = htons(0x04);
		pout->data[0] = pkt->data[0];
		pout->data[1] = pkt->data[1];
		for (i = 2; i < 100; i++)
			pout->data[i] = 0x00;
		ip_tx(sizeof(ip_hdr) + sizeof(udp_hdr) + 8);
		
		if (htons(pkt->udp.len) != 0x020C)
		{
			status = 0x0002;
		}
	}
}
/* EOF */
