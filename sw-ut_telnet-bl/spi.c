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
#include "spi.h"

static void spi_init_ios(u32 port);
static u8   spi1_rd(void);
static u8   spi2_rd(void);
static void spi1_wr(u32 data);
static void spi2_wr(u32 data);
static void spi1_cs(int en);
static void spi2_cs(int en);
static void spi1_wait(void);
static void spi2_wait(void);
static void spi1_flush(void);
static void spi2_flush(void);

s_spi spi_ctrl[2];

void spi_init(void)
{
	/* Config SPI1 port */
	spi_ctrl[0].rd    = spi1_rd;
	spi_ctrl[0].wr    = spi1_wr;
	spi_ctrl[0].cs    = spi1_cs;
	spi_ctrl[0].wait  = spi1_wait;
	spi_ctrl[0].flush = spi1_flush;
	/* Config SPI2 port */
	spi_ctrl[1].rd    = spi2_rd;
	spi_ctrl[1].wr    = spi2_wr;
	spi_ctrl[1].cs    = spi2_cs;
	spi_ctrl[1].wait  = spi2_wait;
	spi_ctrl[1].flush = spi2_flush;
}

s_spi * spi_open(u32 port)
{
	u32 cr1 = port + SPI_CR1;
	s_spi *spi;
	
	if (port == SPI1)
	{
		spi_init_ios(port);
		reg_set(RCC_APB2ENR, 0x1000); /* Activate SPI1 */
		spi = &spi_ctrl[0];
	}
	else if (port == SPI2)
	{
		spi_init_ios(port);
		reg_set(RCC_APB1ENR, 0x4000); /* Activate SPI2 */
		spi = &spi_ctrl[1];
	}
	else
		return(0);
	
	reg_set(cr1, 0x300); /* Set SSM (NSS soft) and SSI */
	if (port == SPI2)
		reg_set(cr1,  0x08); /* Set BR=001 (fclk/4) */
	else
		reg_set(cr1,  0x08); /* Set BR=001 (fclk/4) */
	reg_set(cr1,  0x44); /* Set SPI Enable in master mode */
	return(spi);
}

static void spi1_cs(int en)
{
	vu32 *nss;
	
	nss = (vu32 *)GPIO_BSRR(GPIOA); /* GPIOA_BSRR */
	if (en)
		*nss = 0x00100000; // NSS = 0
	else
		*nss = 0x00000010; // NSS = 1
}

static u8 spi1_rd(void)
{
	u32 value;
	
	/* Wait for RX buffer filled */
	while ((*(vu32 *)SPI1_SR & 0x01) == 0)
		;

	/* Wait until BSY is cleared */
	while (*(vu32 *)SPI1_SR & 0x80)
		;

	value = *(vu32 *)SPI1_DR;
	return value;
}

void spi1_wr(u32 data)
{
	/* Wait for TX buffer empty */
	while ((*(vu32 *)SPI1_SR & 0x02) == 0)
		;
	
	/* Write the value */
	*(vu32 *)SPI1_DR = data;
}
static void spi1_wait(void)
{
  	/* Wait until BSY is cleared */
	while (*(vu32 *)SPI1_SR & 0x80)
		;
	
	/* Wait until the SCK is LOW (bsy cleared after sck rise) */
	while( (u32)*(vu32 *)(GPIOA + 0x08) & 0x20)
		;
}
static void spi1_flush(void)
{
	u32 value;

	/* Wait until BSY is cleared */
	while (*(vu32 *)SPI1_SR & 0x80)
		;
	
	/* Clean the RX buffer */
	if (*(vu32 *)SPI1_SR & 0x01)
	{
		while (*(vu32 *)SPI1_SR & 0x01)
			value = *(vu32 *)SPI1_DR;
		return;
	}
	return;
}

static void spi2_cs(int en)
{
	vu32 *nss;
	
	nss = (vu32 *)GPIO_BSRR(GPIOB);
	
	if (en)
		*nss = 0x10000000; // NSS = 0
	else
	{
		spi2_wait();
		*nss = 0x00001000; // NSS = 1
		spi2_flush();
	}
}

static u8 spi2_rd(void)
{
	u32 value;
	
	/* Wait for RX buffer filled */
	while ((*(vu32 *)SPI2_SR & 0x01) == 0)
		;
	
	/* Wait until BSY is cleared */
	while (*(vu32 *)SPI2_SR & 0x80)
		;
	
	value = *(vu32 *)SPI2_DR;
	return value;
}

void spi2_wr(u32 data)
{
	/* Wait for TX buffer empty */
	while ((*(vu32 *)SPI2_SR & 0x02) == 0)
		;
	
	/* Write the value */
	*(vu32 *)SPI2_DR = data;
}

static void spi2_wait(void)
{
	/* Wait until BSY is cleared */
	while (*(vu32 *)SPI2_SR & 0x80)
		;
	
	/* Wait until the SCK is LOW (bsy cleared after sck rise) */
	while( (u32)*(vu32 *)(GPIOB + 0x08) & 0x2000)
		;
}
static void spi2_flush(void)
{
	u32 value;

	/* Wait until BSY is cleared */
	while (*(vu32 *)SPI2_SR & 0x80)
		;
	
	/* Clean the RX buffer */
	if (*(vu32 *)SPI2_SR & 0x01)
	{
		while (*(vu32 *)SPI2_SR & 0x01)
			value = *(vu32 *)SPI2_DR;
		return;
	}
	return;
}

void spi_wr(u32 port, u32 w)
{
	vu32 *sr, *dr;
	sr  = (vu32 *)(port + SPI_SR);
	dr  = (vu32 *)(port + SPI_DR);
	
	/* Wait for TX buffer empty */
	while ((*sr & 0x02) == 0)
		;
	/* Write the value */
	*dr = w;
}

void spi_wait(u32 port)
{
	vu32 *sr;
	sr  = (vu32 *)(port + SPI_SR);
	
	/* Wait until BSY is cleared */
	while (*sr & 0x80)
		;
	
	/* Wait until the SCK is LOW (bsy cleared after sck rise) */
	if (port == SPI1)
	{
		while( (u32)*(vu32 *)(GPIOB + 0x08) & 0x2000)
			;
	}
	else
	{
		while( (u32)*(vu32 *)(GPIOA + 0x08) & 0x20)
			;
	}

}

static void spi_init_ios(u32 port)
{
	u32 val;
	
	if (port == SPI1)
	{
		reg_set(RCC_APB2ENR, 0x04); /* Activate GPIOA */
		
		/* Configure NSS pin (PA4) */
		val  = reg_rd(GPIOA);
		val &= 0xFFF0FFFF;
		val |= 0x00030000; /* output, 50MHz, push-pull */
		//val |= 0x000B0000; /* output, 50MHz, AF, push-pull */
		reg_wr(GPIOA, val);
		reg_wr(GPIO_BSRR(GPIOA), (0x01 << 4)); /* Disable SS (nss=1) */
		
		/* Configure SCK pin (PA5) */
		val  = reg_rd(GPIOA);
		val &= 0xFF0FFFFF;
		val |= 0x00B00000; /* output, 50MHz, AF, push-pull */
		reg_wr(GPIOA, val);
		
		/* Configure MOSI pin (PA7) */
		val  = reg_rd(GPIOA);
		val &= 0x0FFFFFFF;
		val |= 0xB0000000; /* output, 50MHz, AF, push-pull */
		reg_wr(GPIOA, val);
		
		/* Configure MISO pin (PA6) */
		val  = reg_rd(GPIOA);
		val &= 0xF0FFFFFF;
		val |= 0x04000000; /* input, floating */
		reg_wr(GPIOA, val);
	}
	else if (port == SPI2)
	{
		reg_set(RCC_APB2ENR, 0x08); /* Activate PortB */
		
		/* Configure NSS pin (PB12) */
		val  = reg_rd(GPIOB + 0x04);
		val &= 0xFFF0FFFF;
		val |= 0x00030000; /* output, 50MHz, push-pull */
		reg_wr(GPIOB+0x04, val);
		reg_wr(GPIO_BSRR(GPIOB), (0x01 << 12)); /* Disable SS (nss=1) */
		
		/* Configure SCK pin (PB13) */
		val  = reg_rd(GPIOB + 0x04);
		val &= 0xFF0FFFFF;
		val |= 0x00B00000; /* output, 50MHz, AF, push-pull */
		reg_wr(GPIOB+0x04, val);
		
		/* Configure MOSI pin (PB15) */
		val  = reg_rd(GPIOB + 0x04);
		val &= 0x0FFFFFFF;
		val |= 0xB0000000; /* output, 50MHz, AF, push-pull */
		reg_wr(GPIOB+0x04, val);
		
		/* Configure MISO pin (PB14) */
		val  = reg_rd(GPIOB + 0x04);
		val &= 0xF0FFFFFF;
		val |= 0x04000000; /* input, floating */
		reg_wr(GPIOB + 0x04, val);
	}
}
/* EOF */
