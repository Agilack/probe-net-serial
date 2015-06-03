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
#include "hardware.h"
#include "main.h"
#include "time.h"
#include "uart.h"
#include "api.h"

int main(void)
{
	api_init();
	
	uart_puts("UARTs 2/3 Unit Test\r\n");
	
	uart_init();
	
	time_init();
	hw_tim2_init();
	
	uart_puts(" To change speed, type 'a' or 'b' (port), then '0' to '9' (speed)\r\n");
	uart_puts("  Speeds are :\r\n");
	uart_puts("    0 :   1200 bps\r\n");
	uart_puts("    1 :   4800 bps\r\n");
	uart_puts("    2 :   9600 bps (port B default)\r\n");
	uart_puts("    3 :  19200 bps\r\n");
	uart_puts("    4 :  38400 bps\r\n");
	uart_puts("    5 :  57600 bps\r\n");
	uart_puts("    6 : 115200 bps\r\n");
	uart_puts("    7 : 230400 bps\r\n");
	uart_puts("    8 : 460800 bps\r\n");
	uart_puts("    9 : 921600 bps (port A default)\r\n");
	uart_puts("  Example :\r\n");
	uart_puts("    a2 : Set port A to 9600\r\n");
	uart_puts("    b6 : Set port B to 115200\r\n");

	uart2_config(BAUD_921600);
	uart3_config(BAUD_9600);
	while(1)
	{
		uart2_puts("Hello");
		uart3_puts("world");
	}
}
/* EOF */
