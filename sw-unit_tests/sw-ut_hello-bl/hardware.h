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
#ifndef __HARDWARE_H
#define __HARDWARE_H

#define TIM2  ((u32)0x40000000)
#define SPI2  ((u32)0x40003800)
#define AFIO  ((u32)0x40010000)
#define EXTI  ((u32)0x40010400)
#define GPIOA ((u32)0x40010800)
#define GPIOB ((u32)0x40010C00)
#define GPIOC ((u32)0x40011000)
#define SPI1  ((u32)0x40013000)
#define UART1 ((u32)0x40013800)
#define RCC   ((u32)0x40021000)
#define FLASH ((u32)0x40022000)

#define RCC_CR      RCC
#define RCC_CFGR    (RCC + 0x04)
#define RCC_CIR     (RCC + 0x08)
#define RCC_AHBENR  (RCC + 0x14)
#define RCC_APB2ENR (RCC + 0x18)
#define RCC_APB1ENR (RCC + 0x1C)

#define GPIO_CRL(port)  port
#define GPIO_CRH(port)  (port+0x04)
#define GPIO_IDR(port)  (port+0x08)
#define GPIO_ODR(port)  (port+0x0c)
#define GPIO_BSRR(port) (port+0x10)

#define AFIO_MAPR       (AFIO + 0x04)
#define AFIO_EXTI_CR1   (AFIO + 0x08)
#define AFIO_EXTI_CR2   (AFIO + 0x0C)
#define AFIO_EXTI_CR3   (AFIO + 0x10)
#define AFIO_EXTI_CR4   (AFIO + 0x14)

#define EXTI_IMR   (EXTI  + 0x00)
#define EXTI_EMR   (EXTI  + 0x04)
#define EXTI_RTSR  (EXTI  + 0x08)
#define EXTI_FTSR  (EXTI  + 0x0C)
#define EXTI_PR    (EXTI  + 0x14)

#define SPI_CR1    0x00
#define SPI_CR2    0x04
#define SPI_SR     0x08
#define SPI_DR     0x0C
/**/
#define SPI1_CR1    (SPI1 + SPI_CR1)
#define SPI1_CR2    (SPI1 + SPI_CR2)
#define SPI1_SR     (SPI1 + SPI_SR)
#define SPI1_DR     (SPI1 + SPI_DR)
/* SPI controller 2 */
#define SPI2_CR1   (SPI2 + 0x00)
#define SPI2_CR2   (SPI2 + 0x04)
#define SPI2_SR    (SPI2 + 0x08)
#define SPI2_DR    (SPI2 + 0x0C)

#define UART_SR    (UART1 + 0x00)
#define UART_DR    (UART1 + 0x04)
#define UART_BRR   (UART1 + 0x08)
#define UART_CR1   (UART1 + 0x0C)

#define TIM2_CR1   (TIM2 + 0x00)
#define TIM2_CR2   (TIM2 + 0x04)
#define TIM2_SMCR  (TIM2 + 0x08)
#define TIM2_DIER  (TIM2 + 0x0C)
#define TIM2_SR    (TIM2 + 0x10)
#define TIM2_EGR   (TIM2 + 0x14)
#define TIM2_CNT   (TIM2 + 0x24)
#define TIM2_PSC   (TIM2 + 0x28)
#define TIM2_ARR   (TIM2 + 0x2C)

#define FLASH_ACR     (FLASH + 0x00)
#define FLASH_KEYR    (FLASH + 0x04)
#define FLASH_OPTKEYR (FLASH + 0x08)
#define FLASH_SR      (FLASH + 0x0C)
#define FLASH_CR      (FLASH + 0x10)
#define FLASH_AR      (FLASH + 0x14)
#define FLASH_OBR     (FLASH + 0x1C)
#define FLASH_WRPR    (FLASH + 0x20)

typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;
typedef volatile unsigned long  vu32;
typedef volatile unsigned short vu16;

void hw_init(void);
void hw_setup_irq(int n);
void hw_tim2_init(void);

/* Low level register functions */
void reg_wr (u32 reg, u32 value);
u32  reg_rd (u32 reg);
void reg_set(u32 reg, u32 value);
void reg_clr(u32 reg, u32 value);

/* Low level memory/flash functions */
void hw_flash_erase(u32 addr);
void hw_flash_wr(u8 *data, u32 addr, int len);

#endif
