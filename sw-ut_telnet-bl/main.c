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
#include "time.h"
#include "uart.h"
#include "api.h"
#include "api_ip.h"
#include "ip_tcp.h"

void eth_rx(u8 *frame);

int main(void)
{
	u32 tm_dhcp_next;
	int i;
	
	uart_puts("Telnet-server Unit Test\r\n");
	api_init();
	
	eth_init();
	eth_set_rx((u8 *)0x20004000);
	eth_set_tx((u8 *)0x20004600);
	eth_stack_init();
	
	tcp_init();
	ip_proto_register(0x06, (u32)tcp_rx);
	
	time_init();
	hw_tim2_init();

	tm_dhcp_next = 0;

	i = 0;
	while(1)
	{
		eth_periodic();
		
		if (get_time() > tm_dhcp_next)
		{
			if (eth_status())
			{
				dhcp_periodic();
				tm_dhcp_next = get_time() + 30;
			}
			else
				tm_dhcp_next = get_time() + 5;
		}
	}
}

void *memcpy(void *dst, const void *src, int n)
{
	u8 *s;
	u8 *d;
	s = (u8*)src;
	d = (u8*)dst;
	while(n)
	{
		*d = *s;
		s ++;
		d ++;
		n --;
	}
	return(dst);
}
/* EOF */
