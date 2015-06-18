/*
 * NetSerial - Ethernet Unit Test
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

#include "uip.h"
#include "uip_arp.h"

static void eth_rx_packet(void);

s_eth_status eth_status;

static u32 eth_events;
static u16 enc_rxpnt;

u8 rx_buf0[ETH_FRAME_SIZE];
u8 *uip_buf;
u8 tx_buf0[128];
u8 *net_tx_buf;

u32 tm_last_uip;

int eth_init(void)
{
	s_spi *e_spi;
	u8 v_lsb, v_msb;
	int result;
	
	eth_status.link   = 0;
	eth_status.status = 0;
	eth_status.tx     = 0;
	eth_events  = 0;

	e_spi = spi_open(SPI2);
	
	enc_init(e_spi);
	result = enc_reset();
	if (result)
	{
		uart_puts("eth_init(): Failed into enc_reset() ");
		uart_puthex(result);
		uart_puts("\r\n");
		return(-1);
	}
	
	hw_setup_irq(8);
	
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
	uip_ipaddr_t xIPAddr;
	
	tm_last_uip = 0;
	uip_buf = rx_buf0;
	net_tx_buf = tx_buf0;
	
	uip_init();
	enc_bxsel(3);
	uip_ethaddr.addr[0] = enc_rcr(0x04);
	uip_ethaddr.addr[1] = enc_rcr(0x05);
	uip_ethaddr.addr[2] = enc_rcr(0x02);
	uip_ethaddr.addr[3] = enc_rcr(0x03);
	uip_ethaddr.addr[4] = enc_rcr(0x00);
	uip_ethaddr.addr[5] = enc_rcr(0x01);
	uip_ipaddr( xIPAddr, 192, 168,  44, 4 );
	uip_sethostaddr( xIPAddr );
	uip_ipaddr( xIPAddr, 255, 255, 255, 0 );
	uip_setnetmask( xIPAddr );
	uip_listen(HTONS(80));
	
	return(0);
}

void eth_periodic(void)
{
	u32 tm_elapsed;
	u8  v;
	int i;
	
	enc_bxsel(0);
	
	/* Get Packet Counter from ESTATL */
	v = enc_rcr(0x1A); 
	
	if ( (eth_events & 1) || (v != 0x00) )
	{
		eth_events &= ~1;
		
		eth_rx_packet();
		enc_bxsel(3);

		//enc_bxsel(3);
		// Clear the PKTIF bit
		//enc_bfsc(0x1C, 0x40, 0);
	}
	
	tm_elapsed = get_time() - tm_last_uip;
	if ( (tm_elapsed > 10) || (v != 0) )
	{
		for(i = 0; i < UIP_CONNS; ++i)
		{
			uip_periodic(i);
			if(uip_len > 0)
			{
				uip_arp_out();
				eth_tx_packet();
			}
		}
	}
	return;
}

static void eth_rx_packet(void)
{
	u8  enc_rx_head[6];
	u16 adr_lsb, adr_msb;
	u16 pkt_len;
	u16 adr_tail;
	int pkt_valid;
	int ii;

	DBG_RX("PKT_RX: ");
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
			rx_buf0[ii] = enc_rrxdata(1, 0);
	}
	else
	{
		DBG_RX("ETH: error, packet too big.\r\n");
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
		uip_len = pkt_len;
		struct uip_eth_hdr *pHead;
		pHead = (struct uip_eth_hdr *)(void *)uip_buf;
		if (htons(pHead->type) == 0x0800)
		{
			DBG_RX("IP\r\n");
			uip_arp_ipin();
			uip_input();
			if( uip_len > 0 )
				uip_arp_out();
		}
		else if (htons(pHead->type) == 0x0806)
		{
			DBG_RX("ARP\r\n");
			uip_arp_arpin();
		}
		else
		{
			int j = 0;
			DBG_RX("Unknown ");
			uart_puthex(pkt_len);
			uart_crlf();
			while (j < pkt_len)
			{
				uart_puthex(j);
				uart_puts(": ");
				for (ii = 0; ii < 16; ii++, j++)
				{
					uart_puthex8(rx_buf0[j]);
					uart_putc(' ');
					if (j == pkt_len)
					  break;
				}
				uart_crlf();
			}
			uip_len = 0;
		}
		
		if (uip_len)
			eth_tx_packet();
	}
	DBG_RX("\r\n");
}

void eth_tx_packet(void)
{
	if (eth_status.link == 0)
	{
		DBG_TX("TX ! LINK\r\n");
		return;
	}
	
	while (eth_status.tx)
		uart_putc('.');
	
#ifdef DEBUG_ETH_TX
	DBG_TX("PKT_TX: len=");
	uart_puthex16(uip_len);
	DBG_TX("\r\n");
#endif
	
	/* Clear TXIF */
	enc_bxsel(0);
	enc_bfsc(0x1C, 0x08, 0);
				
	//enc_w_pt(ENC_PT_GPWR, 0x00);
	enc_wgpwrpt(0x0000);
	enc_wtxdata(uip_len, uip_buf);
	
	enc_bxsel(0);
	
	/* Update TX start */
	enc_wcr(0x01, 0x00); // ETXSTH
	enc_wcr(0x00, 0x00); // ETXSTL
	
	/* Update TX length */
	enc_wcr(0x03, uip_len >>   8); // ETXLENH
	enc_wcr(0x02, uip_len & 0xFF); // ETXLENL

	eth_status.tx = 1;
	
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
	eth_status.tx = 0;
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
		DBG_TX("[TXIF]");
		/* Clear TXIF flag */
		enc_bxsel(0);
		enc_bfsc(0x1C, 0x08, 0);
		/* Clear TX status flag */
		eth_status.tx = 0;
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
			eth_status.link = 1;
		}
		else
		{
			uart_puts("LINK DOWN\r\n");
			eth_status.link = 0;
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
