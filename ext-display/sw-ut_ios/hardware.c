/**
 * NetSerial Display Extention - Unit test
 *
 * Copyright (c) 2015 Saint-Genest Gwenael <gwen@agilack.fr>
 *
 * This file may be distributed and/or modified under the terms of the
 * GNU General Public License version 2 as published by the Free Software
 * Foundation. (See COPYING.GPL for details.)
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */
#include <efm8bb1.h>
#include <efm8bb1_defs.h>
#include "hardware.h"

void hw_init(void)
{
	// Disable Watchdog
	WDTCN = 0xDE; // First key
	WDTCN = 0xAD; // Second key
	
	// Configure port 0
	P0MDOUT = P0MDOUT_B0__PUSH_PULL | P0MDOUT_B1__OPEN_DRAIN | P0MDOUT_B2__PUSH_PULL
		 | P0MDOUT_B3__PUSH_PULL | P0MDOUT_B4__PUSH_PULL | P0MDOUT_B5__OPEN_DRAIN
		 | P0MDOUT_B6__OPEN_DRAIN | P0MDOUT_B7__OPEN_DRAIN;

	// Configure port 1
	P1MDOUT = P1MDOUT_B0__PUSH_PULL | P1MDOUT_B1__PUSH_PULL | P1MDOUT_B2__OPEN_DRAIN
		 | P1MDOUT_B3__OPEN_DRAIN;

	// Configure crossbar
	XBR0 = XBR0_URT0E__ENABLED | XBR0_SPI0E__ENABLED | XBR0_SMB0E__DISABLED
		 | XBR0_CP0E__DISABLED | XBR0_CP0AE__DISABLED | XBR0_CP1E__DISABLED
		 | XBR0_CP1AE__DISABLED | XBR0_SYSCKE__DISABLED;
	XBR2 = XBR2_WEAKPUD__PULL_UPS_ENABLED | XBR2_XBARE__ENABLED;

	// Configure clocks
	CLKSEL = CLKSEL_CLKSL__HFOSC | CLKSEL_CLKDIV__SYSCLK_DIV_2;
	
	hw_init_timers();
	
	hw_init_spi();
	hw_init_uart();
}

void hw_init_timers(void)
{
	unsigned char TCON_save;
	
	TCON_save = TCON;
	TCON &= ~TCON_TR0__BMASK & ~TCON_TR1__BMASK;
	TH1 = (0xF2 << TH1_TH1__SHIFT);
	TL1 = (0x40 << TL1_TL1__SHIFT);
	TCON |= (TCON_save & TCON_TR0__BMASK) | (TCON_save & TCON_TR1__BMASK);

	CKCON0 = CKCON0_SCA__SYSCLK_DIV_4 | CKCON0_T0M__PRESCALE | CKCON0_T2MH__EXTERNAL_CLOCK
		 | CKCON0_T2ML__EXTERNAL_CLOCK | CKCON0_T3MH__EXTERNAL_CLOCK | CKCON0_T3ML__EXTERNAL_CLOCK
		 | CKCON0_T1M__PRESCALE;
	TMOD = TMOD_T0M__MODE0 | TMOD_T1M__MODE2 | TMOD_CT0__TIMER | TMOD_GATE0__DISABLED
		 | TMOD_CT1__TIMER | TMOD_GATE1__DISABLED;
	TCON |= TCON_TR1__RUN;
}

void hw_init_spi(void)
{
	SPI0CKR = (0x08 << SPI0CKR_SPI0CKR__SHIFT);
	SPI0CFG |= SPI0CFG_MSTEN__MASTER_ENABLED;
	SPI0CN0 |= SPI0CN0_SPIEN__ENABLED | SPI0CN0_NSSMD__4_WIRE_MASTER_NSS_HIGH;
}

void hw_init_uart(void)
{
	SCON0 |= SCON0_REN__RECEIVE_ENABLED;
}
