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
#include "spi.h"
#include "eth.h"
#include "eth_drv_enc.h"

#include "ip.h"
#include "dhcp.h"

static void eth_rx(void);
static void eth_rx_packet(eth_hdr *frame);
static void (*eth_rx_data)(eth_hdr *data);

static s_eth_status status;

static u32 eth_events;
static u16 enc_rxpnt;

u8  host_mac[6];
u8 *net_rx_buf;
u8 *net_tx_buf;
int net_tx_len;

int eth_init(void)
{
	u8 v_lsb, v_msb;
	int result;
	
	eth_rx_data   = eth_rx_packet;
	status.link   = 0;
	status.tx     = 0;
	eth_events  = 0;

	spi_open(SPI2);
	
	result = enc_reset();
	if (result)
	{
		uart_puts("eth_init(): Failed into enc_reset() ");
		uart_puthex(result);
		uart_puts("\r\n");
		return(-1);
	}
	
//	hw_setup_irq(8);

	/* Configure buffers */
	enc_bxsel(0);
	enc_wcr(0x05, 0x06); // ERXSTH
	enc_wcr(0x04, 0x00); // ERXSTL
	enc_rxpnt = 0x0600;
	
	enc_wcr(0x07, 0x5F); // ERXTAILH
	enc_wcr(0x06, 0xFE); // ERXTAILL

	enc_wcr(0x17, 0x60); // EUDASTH
	enc_wcr(0x16, 0x00); // EUDASTL

	enc_wcr(0x19, 0x60); // EUDANDH
	enc_wcr(0x18, 0x01); // EUDANDL
	
	/* Init the local MAC address */
	enc_bxsel(3);
	host_mac[0] = enc_rcr(0x04);
	host_mac[1] = enc_rcr(0x05);
	host_mac[2] = enc_rcr(0x02);
	host_mac[3] = enc_rcr(0x03);
	host_mac[4] = enc_rcr(0x00);
	host_mac[5] = enc_rcr(0x01);
	
	/* Enable interrupts */
	enc_bxsel(3);
	v_msb = 0x88;
	v_lsb = 0;
#ifdef ETH_IT_TX
	v_lsb |= 0x0C;
#endif
#ifdef ETH_IT_RX
	v_lsb |= 0x40;
#endif
	enc_wcr(0x12, v_lsb); // EIEL
	enc_wcr(0x13, v_msb); // EIEH
	//enc_sbi(0xEC); // SETEIE
	
	// Enable RX packet reception
	enc_sbi(0xE8);
	
	return(0);
}

int eth_stack_init(void)
{
	host_ip = 0;
	
	ip_proto_register(17, (u32)udp_rx);
	
	uart_puts("ARP ");
	ip_arp_init();
	
	uart_puts("DHCP ");
	dhcp_init();
	
	return(0);
}

void eth_periodic(void)
{
	u8  v;
	
	enc_bxsel(0);
	
	/* Get global status from ESTATH */
	v = enc_rcr(0x1B);
	
	/* Update LINK */
	if (v & 0x01)
		status.link = 1;
	else
		status.link = 0;

	/* Get Packet Counter from ESTATL */
	v = enc_rcr(0x1A); 
	
	if ( (eth_events & 1) || (v != 0x00) )
	{
		eth_events &= ~1;
		
		eth_rx();
		enc_bxsel(3);

		//enc_bxsel(3);
		// Clear the PKTIF bit
		//enc_bfsc(0x1C, 0x40, 0);
	}
	
	return;
}

void eth_set_rx(u8 *addr)
{
	net_rx_buf = addr;
}

void eth_set_tx(u8 *addr)
{
	net_tx_buf = addr;
}

void eth_set_callback(u32 addr)
{
	eth_rx_data = (void(*)())addr;
}

int eth_status(void)
{
	return status.link;
}

static void eth_rx(void)
{
	u8  enc_rx_head[6];
	u16 adr_lsb, adr_msb;
	u16 pkt_len;
	u16 adr_tail;
	int pkt_valid;
	int ii;

	D_ETH_RX("PKT_RX: ");
	enc_bxsel(0);
	enc_wrxrdpt(enc_rxpnt);
	
	/* Read and save next packet address */
	adr_lsb = enc_rrxdata(1, 0);
	adr_msb = enc_rrxdata(1, 0);
	enc_rxpnt = (adr_msb << 8) | adr_lsb;
		
	/* Read the Receive Status Vector */
	for (ii = 0; ii < 6; ii++)
		enc_rx_head[ii] = enc_rrxdata(1, 0);
	/* Get packet length from status vector */
	pkt_len = (enc_rx_head[1] << 8) | enc_rx_head[0];
	
	if (pkt_len < ETH_FRAME_SIZE)
	{
		pkt_valid = 1;
		/* Read datas */
		for (ii = 0; ii < pkt_len; ii++)
			net_rx_buf[ii] = enc_rrxdata(1, 0);
	}
	else
	{
		D_ETH_RX("ETH: error, packet too big.\r\n");
		pkt_valid = 0;
	}
	
	/* Packet decrement */
	enc_bfsc(0x1F, 0x01, 1); /* ECON1 - PKTDEC */
	
	/* Compute the new tail address */
	adr_tail = enc_rxpnt - 2;
	if (enc_rxpnt == 0x600)
		adr_tail = 0x5FFE;
	/* Update the RX tail register */
	enc_wcr(0x06, adr_tail &  0xFF); // ERXTAILL
	enc_wcr(0x07, adr_tail >> 8);    // ERXTAILH

	if (pkt_valid == 1)
	{
		eth_rx_data((eth_hdr *)net_rx_buf);
	}
	D_ETH_RX("\r\n");
}

static void eth_rx_packet(eth_hdr *frame)
{
	if (frame->type == htons(0x0800))
	{
		D_ETH_RX("IP");
		ip_rx();
	}
	else if (frame->type == htons(0x0806))
	{
		D_ETH_RX("ARP\r\n");
		ip_arp_rx();
	}
/*	else
	{
		int j = 0;
		D_ETH_RX("Unknown ");
		uart_puthex(pkt_len);
		uart_crlf();
		while (j < pkt_len)
		{
			uart_puthex(j);
			uart_puts(": ");
			for (ii = 0; ii < 16; ii++, j++)
			{
				uart_puthex8(net_rx_buf[j]);
				uart_putc(' ');
				if (j == pkt_len)
				  break;
			}
			uart_crlf();
		}
	}
*/
}

void eth_tx_packet(void)
{
	eth_hdr *frame;
	frame = (eth_hdr *)net_tx_buf;
	frame->mac_s[0] = host_mac[0];
	frame->mac_s[1] = host_mac[1];
	frame->mac_s[2] = host_mac[2];
	frame->mac_s[3] = host_mac[3];
	frame->mac_s[4] = host_mac[4];
	frame->mac_s[5] = host_mac[5];

//	uart_puts("eth_tx_packet()\r\n");
//	dump(net_tx_buf, net_tx_len);
	if (status.link == 0)
	{
		D_ETH_TX("TX ! LINK\r\n");
		return;
	}
	
	while (status.tx)
		uart_putc('.');
	
#ifdef DEBUG_ETH_TX
	D_ETH_TX("PKT_TX: len=");
	uart_puthex16(net_tx_len);
	D_ETH_TX("\r\n");
#endif
	
	/* Clear TXIF */
	enc_bxsel(0);
	enc_bfsc(0x1C, 0x08, 0);
				
	//enc_w_pt(ENC_PT_GPWR, 0x00);
	enc_wgpwrpt(0x0000);
	enc_wtxdata(net_tx_len, net_tx_buf);
	
	enc_bxsel(0);
	
	/* Update TX start */
	enc_wcr(0x01, 0x00); // ETXSTH
	enc_wcr(0x00, 0x00); // ETXSTL
	
	/* Update TX length */
	enc_wcr(0x03, net_tx_len >>   8); // ETXLENH
	enc_wcr(0x02, net_tx_len & 0xFF); // ETXLENL

	status.tx = 1;
	
	/* Set TXRTS to start transmit */
	enc_bfsc(0x1E, 0x02, 1);

#ifndef ETH_IT_TX
	enc_bxsel(3);
	while(1)
	{
		u8 v;
		/* Read back ECON1 to get TXRTS value */
		v = enc_rcr(0x1E);
		/* If TXRTS is cleared, transmit is finished */
		if ((v & 0x02) == 0)
			break;
	}
	status.tx = 0;
#endif
}

void eth_interrupt(void)
{
	u8 v_msb, v_lsb;

	/* Get active interrupts from EIR */
	enc_bxsel(3);
	v_msb = enc_rcr(0x1D);
	v_lsb = enc_rcr(0x1C);
	uart_puthex8(v_msb); uart_putc(':');
	uart_puthex8(v_lsb); uart_putc(' ');
	
#ifdef not_defined
	// If PKTIF
	if (v_lsb & 0x40)
	{
		uart_puts("[RX]");
		// Clear the PKTIE bit into EIE
		enc_bfsc(0x12, 0x40, 0);
		
		eth_events |= 1;
	}
#endif
	
#ifdef ETH_IT_TX
	// If TXIF
	if (v_lsb & 0x0C)
	{
		D_ETH_TX("[TXIF]");
		/* Clear TXIF flag */
		enc_bxsel(0);
		enc_bfsc(0x1C, 0x08, 0);
		/* Clear TX status flag */
		status.tx = 0;
	}
#endif
	
	// If LINK_IF
	if (v_msb & 0x08)
	{
		u32 v;
		
		enc_bxsel(0);
		/* Get PHYLNK status from ESTATH */
		v = enc_rcr(0x1B);
		if (v & 1)
		{
			uart_puts("LINK UP\r\n");
			status.link = 1;
		}
		else
		{
			uart_puts("LINK DOWN\r\n");
			status.link = 0;
		}
		
		enc_bxsel(3);
		// Clear the EIE bit
		//enc_bfsc(0x13, 0x08, 0);
		// Clear the linkif bit
		enc_bfsc(0x1D, 0x08, 0);
	}
}

u32 enc_phy_r(u8 radr)
{
	u32 value;
	
	/* Select Bank 2 */
	enc_bxsel(2);
	/* Select required PHY register (MIREGADR) */
	enc_wcr(0x14, radr);
	enc_wcr(0x15, 0x01);
	/* Start MII Read (MICMD=0x01) */
	enc_wcr(0x12, 0x01);
	/* Select Bank 3 */
	enc_bxsel(3);
	/* Wait read end */
	do
	{
		/* Read the MISTAT reg */
		value = enc_rcr(0x0A);
	} while(value & 1);
	/* Select Bank 2 */
	enc_bxsel(2);
	/* Stop MII Read */
	enc_wcr(0x12, 0x00);
	/* Select Bank 3 */
	enc_bxsel(3);
	/* Get result from MIRD */
	value = enc_rcr(0x09);
	value = value << 8;
	value = value | enc_rcr(0x08);
	
	return(value);
}
/* EOF */
