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
#include "hardware.h"
#include "main.h"

static void hw_setup_ios(void);
static void hw_setup_clock(void);

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
	hw_setup_clock();
	hw_setup_ios();
	
	/* Reset interrupt controller */
	reg_clr(EXTI_IMR,  0xFFFFFFFF); /* Interrupt Mask Register */
	reg_clr(EXTI_EMR,  0xFFFFFFFF); /* Event Mask Register */
}

static void hw_setup_ios(void)
{
	reg_set(RCC_APB2ENR, 0x01); // Enable AFIO
//	reg_set(RCC_APB2ENR, 0x04); // Enable GPIOA (used by leds)
//	reg_set(RCC_APB2ENR, 0x08); // Enable GPIOB (used by button)
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

static void hw_setup_clock(void)
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


u32 glb_tim2;

/**
 * 
 * 
 */
void hw_tim2_init(void)
{
	glb_tim2 = 0;
	
	reg_set(RCC_APB1ENR, 0x01); /* Activate Timer2 */
	/* Configure Timer2 */
	reg_wr (TIM2_CR1, 0x0200); /* Input clock divided by 4 */
	reg_wr (TIM2_ARR, 0x3680); /* Value used when reloaded */
	reg_wr (TIM2_PSC,   0x80); /* Prescaler       */
	reg_set(TIM2_DIER,  0x01); /* Set UIE bit     */
	/* NVIC: Enable interrupt 28 (TIM2) */
	reg_wr(0xE000E100, (1 << 28));
	/* Activate Timer2 (CEN=1) */
	reg_set(TIM2_CR1,     1);
}

void TIM2_IRQHandler(void)
{
	/* Increment counter */
	glb_tim2++;
	
	/* Clear event */
	reg_clr(TIM2_SR, 1);
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
