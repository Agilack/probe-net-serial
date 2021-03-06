/**
 * NetSerial - Unit test "Hello world" with bootloader
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

int main(void)
{
	int i;
	
	while(1)
	{
		uart_puts("10\r\nHello World !\r\n");
		for (i = 0; i < 0x10000; i++);
	}

	while(1);
}
/* EOF */
