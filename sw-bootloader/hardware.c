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

static void hw_setup_ios(void);

u32 reg_rd(u32 reg)
{
	return ( *(volatile u32 *)reg );
}
void reg_wr(u32 reg, u32 value)
{
	*(volatile u32 *)reg = value;
}
void reg_set(u32 reg, u32 value)
{
	*(volatile u32 *)reg = ( *(volatile u32 *)reg | value );
}
void reg_clr(u32 reg, u32 value)
{
	*(volatile u32 *)reg = ( *(volatile u32 *)reg & ~value );
}

void hw_init(void)
{
//	hw_setup_clock();
	hw_setup_ios();
	
	/* Reset interrupt controller */
	reg_clr(EXTI_IMR,  0xFFFFFFFF); /* Interrupt Mask Register */
	reg_clr(EXTI_EMR,  0xFFFFFFFF); /* Event Mask Register */
}

static void hw_setup_ios(void)
{
	u32 val;

	reg_set(RCC_APB2ENR, 0x01); // Enable AFIO
	reg_set(RCC_APB2ENR, 0x04); // Enable GPIOA
//	reg_set(RCC_APB2ENR, 0x08); // Enable GPIOB

	/* Configure SW0 (GPIO A 13) */
	val  = reg_rd(GPIO_CRH(GPIOA));
	val &= 0xFF0FFFFF;
	val |= 0x00800000;
	reg_wr(GPIO_CRH(GPIOA), val);
	/* Set pull-up */
	reg_wr(GPIO_BSRR(GPIOA), 0x20000000);
}

void hw_setup_irq(int n)
{
	u32 val;
	
	if (n == 8)
	{
		/* Configure External Interrupt (IRQ) from ENC424J600 */
		val = reg_rd(AFIO_EXTI_CR3);
		val &= 0xFFFFFFF0;  /* AFIO: Port A for EXTI-8 */
		reg_wr(AFIO_EXTI_CR3, val);

		reg_set(EXTI_FTSR, 0x00000100); /* Trigger on Falling edge, TR8 */
		reg_wr (EXTI_PR,   0x00000100); /* Clear the Pending Register   */
		reg_set(EXTI_IMR,  0x00000100);
		
		reg_wr(0xE000E100, (1 << 23) ); /* NVIC: Enable EXTI9_5 interrupt */
	}
}

void hw_setup_clock(void)
{
	/* Activate HSE */
	reg_set(RCC_CR, 0x00010001);
	/* Wait the activation complete */
	while ((reg_rd(RCC_CR) & 0x00020000) == 0)
		;

	/* Enable flash prefetch buffer */
	reg_wr(FLASH_ACR, 0x00000012);

	/* Configure PLL */
	reg_set(RCC_CFGR, 0x001D2400); /* pll=72Mhz,APB1=36Mhz,APB2=36Mhz,AHB=72Mhz */
	reg_set(RCC_CR,   0x01000000); /* enable the pll */
	/* wait for it to come on */
	while ((reg_rd(RCC_CR) & 0x03000000) == 0)
		;
	
	/* Set SYSCLK as PLL */
	reg_set(RCC_CFGR, 0x00000002);
	/* Wait until switch complete */
	while ((reg_rd(RCC_CFGR) & 0x00000008) == 0)
		;

	return;
}

void hw_flash_unlock(void)
{
	u32 v;
	v = reg_rd(FLASH_CR);
	if ((v & 0x80) == 0)
		return;
	
	/* Unlock sequence */
	reg_wr(FLASH_KEYR, 0x45670123);
	reg_wr(FLASH_KEYR, 0xCDEF89AB);
}

void hw_flash_erase(u32 addr)
{
	while (reg_rd(FLASH_SR) & 1)
		;
	
	/* Set PER bit */
	reg_wr(FLASH_CR, 0x02);
	
	/* Set erase address to AR */
	reg_wr(FLASH_AR, addr);
	
	/* Set STRT bit */
	reg_wr(FLASH_CR, 0x42);
	
	while (reg_rd(FLASH_SR) & 1)
		;
}

void hw_flash_wr(u8 *data, u32 addr, int len)
{
	u16 wr_val;
	
	while (len > 0)
	{
		/* Set PG bit */
		reg_wr(FLASH_CR, 0x01);
		/* Write access to flash */
		wr_val = (data[1] << 8) | data[0];
		*(vu16 *)addr = wr_val;
		/* Wait for write cycle to complete */
		while (reg_rd(FLASH_SR) & 1)
			;
		/* Update counters and pointers */
		len  -= 2;
		data += 2;
		addr += 2;
	}
}

void Global_Handler(u8 n);
void NMI_Handler(void)        { Global_Handler('C'); }
void HardFault_Handler(void)  { Global_Handler('D'); }
void MemManage_Handler(void)  { Global_Handler('E'); }
void BusFault_Handler(void)   { Global_Handler('F'); }
void UsageFault_Handler(void) { Global_Handler('G'); }
void vPortSVCHandler(void)    { Global_Handler('L'); }
void DebugMon_Handler(void)   { Global_Handler('M'); }
void xPortPendSVHandler(void) { Global_Handler('O'); }
void xPortSysTickHandler(void){ Global_Handler('P'); }
void WWDG_IRQHandler(void)    { Global_Handler('Q'); }
void PVD_IRQHandler(void)     { Global_Handler('R'); }
void TAMPER_IRQHandler(void)  { Global_Handler('S'); }
void RTC_IRQHandler(void)     { Global_Handler('T'); }
void FLASH_IRQHandler(void)   { Global_Handler('U'); }
void RCC_IRQHandler(void)     { Global_Handler('V'); }
void EXTI0_IRQHandler(void)   { Global_Handler('W'); }
void EXTI1_IRQHandler(void)   { Global_Handler('X'); }
void EXTI2_IRQHandler(void)   { Global_Handler('Y'); }
void EXTI3_IRQHandler(void)   { Global_Handler('Z'); }
void EXTI4_IRQHandler(void)   { Global_Handler('a'); }

/* EOF */
