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
#ifndef ETH_H
#define ETH_H

#define ETH_FRAME_SIZE 768
//#define ETH_IT_TX 1
#undef ETH_IT_TX

typedef struct _s_eth_status
{
	unsigned char link;
	unsigned char status;
	unsigned char tx;
} s_eth_status;

int  eth_init(void);
int  eth_stack_init(void);
void eth_interrupt(void);
void eth_periodic(void);
int  eth_status(void);

void eth_tx_packet(void);

u32 enc_phy_r(u8 radr);

extern u8 *net_rx_buf;
extern u8  host_mac[6];

#undef DEBUG_ETH_TX
#undef DEBUG_ETH_RX

#ifdef DEBUG_ETH_RX
#define D_ETH_RX(x) do { uart_puts(x); } while(0);
#else
#define D_ETH_RX(x) do { } while(0);
#endif

#ifdef DEBUG_ETH_TX
#define D_ETH_TX(x) do { uart_puts(x); } while(0);
#else
#define D_ETH_TX(x) do { } while(0);
#endif

#endif
