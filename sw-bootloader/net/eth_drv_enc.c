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
#include "spi.h"
#include "eth.h"
#include "eth_drv_enc.h"

static s_spi *e_spi;
static int    enc_bank;

void enc_init(s_spi *spi)
{
	e_spi = spi;
	enc_bank = 0xFF;
}

void enc_bxsel(int n)
{
//	if (n == enc_bank)
//		return;
	
	/* Send the BxSEL command with bank number */
	enc_sbi(0xC0 | (n << 1) );
	
	/* Save the new bank id */
	enc_bank = n;
}

/**
 * Single Byte Instruction
 * 
 */
void enc_sbi(u32 cmd)
{
	e_spi->cs(1);
	
	/* Send command */
	e_spi->wr(cmd);
	
	e_spi->cs(0);
}

/**
 * Set or Clear into Bit Fields
 * 
 * @param address of the register
 * @param mask of the bits to change
 * @param set or clear access (1/0)
 */
void enc_bfsc(u8 adr, u8 mask, u8 set)
{
	e_spi->cs(1);
	if (set)
		e_spi->wr(0x80 | (adr & 0x1F) );
	else
		e_spi->wr(0xA0 | (adr & 0x1F) );
	/* The the bit mask */
	e_spi->wr(mask);
	e_spi->flush();
	e_spi->wait();
	e_spi->cs(0);
}

u8 enc_rcr(u8 radr)
{
	u32 value;
	
	e_spi->cs(1);
	
	/* Read from the specified reg (RCR spi command) */
	e_spi->wr( (0x00 << 5) | (radr & 0x1F) );
	e_spi->flush();

	/* Write a dummy byte */
	e_spi->wr(0x00);
	/* Wait for RX buffer filled */
	value = e_spi->rd();

	e_spi->cs(0);
	
	return(value & 0xFF);
}

u8 enc_rcru(u8 radr)
{
	u32 value;
	
	e_spi->flush();

	e_spi->cs(1);

	/* Write the unbanked read command (RCRU) */
	e_spi->wr(0x20);
	e_spi->flush();
	/* Write the register address */
	e_spi->wr(radr);
	e_spi->flush();

	/* Write a dummy byte */
	e_spi->wr(0x00);
	/* Wait for RX buffer filled */
	value = e_spi->rd();

	e_spi->wait();
	e_spi->cs(0);
	
	return(value & 0xFF);
}


void enc_wcr(u8 radr, u8 rval)
{
	e_spi->cs(1);
	if (radr & 0x80)
	{
		/* Write the unbanked write command (WCRU) */
		e_spi->wr(0x22);
		e_spi->flush();
		/* Write the register address */
		e_spi->wr(radr);
		e_spi->flush();
	}
	else
	{
		/* Write to the specified reg (WCR spi command) */
		e_spi->wr((0x02 << 5) | (radr & 0x1F));
		/* Wait until BSY is cleared */
		e_spi->wait();
	}
	/* Write the value */
	e_spi->wr( rval );
	e_spi->wait();
	
	/* Read the last input byte to flush RX */
	e_spi->flush();

	e_spi->cs(0);
}

/**
 * Get the value of one SRAM pointer
 * 
 * @param id of the pointer
 */
u32 enc_r_pt(u8 id)
{
	u8 v_lsb;
	u8 v_msb;
	
	e_spi->cs(1);
	e_spi->wr(0x60 | id);
	e_spi->flush();
	e_spi->wr(0x00);
	v_lsb = e_spi->rd();
	e_spi->wr(0x00);
	v_msb = e_spi->rd();
	e_spi->cs(0);
	
	return( (v_msb << 8) | v_lsb );
}

/**
 * Set the value of one SRAM pointer used for indirect buffer access
 * 
 * @param id of the pointer
 * @param address to set
 */
void enc_w_pt(u8 id, u32 addr)
{
	e_spi->cs(1);
	e_spi->wr(0x60 | id);
	e_spi->wr(addr &  0xFF);
	e_spi->wr(addr >> 8);
	e_spi->flush();
	e_spi->wait();
	e_spi->cs(0);
}

u32 enc_rrxrdpt(void)
{
	return enc_r_pt(0x06);
}

void enc_wrxrdpt(u32 addr)
{
	enc_w_pt(0x04, addr);
}

u32 enc_rgprdpt(void)
{
	return enc_r_pt(0x02);
}

void enc_wgprdpt(u32 addr)
{
	enc_w_pt(0x00, addr);
}

/**
 * Set General Purpose buffer Write Pointer
 * 
 */
void enc_wgpwrpt(u32 addr)
{
	enc_w_pt(ENC_PT_GPWR, addr);
}
u32 enc_rgpwrpt(void)
{
	return enc_r_pt(0x0E);
}

u8 enc_rrxdata(int len, char *buffer)
{
	u32 value;
	
	e_spi->cs(1);
	e_spi->wr(0x2C);
	e_spi->flush();
	while(len)
	{
		e_spi->wr(0x00);
		value = e_spi->rd();
		if (buffer)
			*buffer++ = value;
		len--;
	}
	e_spi->cs(0);
	return(value);
}

void enc_wtxdata(int len, u8 *buffer)
{
	e_spi->cs(1);
	
	e_spi->wr(0x2A); /* WGPDATA */
	while(len)
	{
		e_spi->wr( *buffer );
		buffer ++;
		len --;
	}
	e_spi->flush();
	e_spi->wait();
	e_spi->cs(0);
}

/**
 * 
 * Init procedure (39935b.pdf page 77)
 */
int enc_reset(void)
{
	u32 value;
	
	// Switch to bank 0
	enc_bxsel(0);
	
	// 1) Write arbitrate value to EUDAST
	enc_wcr(0x16, 0x34);
	enc_wcr(0x17, 0x12);
	
	// 2) Read back EUDAST, and verify value
	value  = (enc_rcr(0x17) << 8);
	value |=  enc_rcr(0x16);
	if (value != 0x1234)
		return(-1);
	
	// 3) Wait CLKRDY
	while(1)
	{
		/* Read ESTAT */
		value = enc_rcr(0x1B);
		/* Test CLKRDY bit */
		if (value & 0x10)
			break;
	}

	// 4) Soft-reset sequence
	enc_bxsel(3);
	enc_wcr(0x0E, 0x10);

	// 5) Wait at least 25us
//	for (value = 0; value < 0x80000; value++)
	for (value = 0; value < 0x8000; value++)
		asm volatile("nop");

	// 6) Read back EUDAST to confirm reset value
	enc_bxsel(0);
	value  = (enc_rcr(0x17) << 8);
	value |=  enc_rcr(0x16);
	if (value != 0x0000)
		return(-2);

	// 7) Wait at least 256us before PHY ready
//	for (value = 0; value < 0x800000; value++)
	for (value = 0; value < 0x80000; value++)
		asm volatile("nop");
	
	return(0);
}
/* EOF */
