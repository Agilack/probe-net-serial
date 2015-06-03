/**
 * NetSerial - Unit test for UART2 and UART3
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

#include "api.h"

#define API_BASE 0x08000100
#define API_BASE_SPI  (API_BASE + 0x10)
#define API_BASE_ETH  (API_BASE_SPI + 0x18)
#define API_BASE_IP   (API_BASE_ETH + 0x1C)
#define API_BASE_DHCP (API_BASE_IP  + 0x18)

#define API_UART(reg) *(u32*)(API_BASE+reg)
#define API_SPI(reg)  *(u32*)(API_BASE_SPI + reg)
#define API_ETH(reg)  *(u32*)(API_BASE_ETH + reg)
#define API_IP(reg)   *(u32*)(API_BASE_IP  + reg)
#define API_DHCP(reg) *(u32*)(API_BASE_DHCP+ reg)

void api_init(void)
{
	uart_putc   = (void (*)(u8 c)        ) API_UART(0x04);
	uart_puts   = (void (*)(char *)      ) API_UART(0x08);
	uart_puthex = (void (*)(const u32 c) ) API_UART(0x0C);
	
	eth_init         = (int  (*)(void)   ) API_ETH(0x00);
	eth_periodic     = (void (*)(void)   ) API_ETH(0x04);
	eth_status       = (int  (*)(void)   ) API_ETH(0x08);
	eth_set_rx       = (void (*)(u8 *)   ) API_ETH(0x0C);
	eth_set_tx       = (void (*)(u8 *)   ) API_ETH(0x10);
	eth_set_callback = (void (*)(u32)    ) API_ETH(0x14);
	eth_stack_init   = (void (*)(void)   ) API_ETH(0x18);
	
	ip_tx             = (void (*)(int)    ) API_IP(0x00);
	ip_prepare        = (void (*)(ip_hdr*)) API_IP(0x04);
	ip_proto_register = (void (*)(u8,u32) ) API_IP(0x08);
	ip_cksum = (u16 (*)(u32,const u8*,u16)) API_IP(0x0C);
	htons             = (u16  (*)(u16)    ) API_IP(0x10);
	htonl             = (u32  (*)(u32)    ) API_IP(0x14);
	
	dhcp_periodic     = (void (*)(void)   ) API_DHCP(0x00);
}
/* EOF */
