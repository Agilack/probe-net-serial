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
#ifndef _ETH_H_
#define _ETH_H_

#define ETH_FRAME_SIZE 768
#define ETH_IT_TX 1

typedef struct _s_eth_status
{
	unsigned char link;
	unsigned char status;
	unsigned char tx;
} s_eth_status;

int  eth_init(void);
void eth_interrupt(void);
void eth_periodic(void);

void eth_tx_packet(void);

u32 enc_phy_r(u8 radr);

#define DEBUG_ETH_TX 1
#define DEBUG_ETH_RX 1

#ifdef DEBUG_ETH_RX
#define DBG_RX(x) do { uart_puts(x); } while(0);
#else
#define DBG_RX(x) do { } while(0);
#endif

#ifdef DEBUG_ETH_TX
#define DBG_TX(x) do { uart_puts(x); } while(0);
#else
#define DBG_TX(x) do { } while(0);
#endif

#endif
