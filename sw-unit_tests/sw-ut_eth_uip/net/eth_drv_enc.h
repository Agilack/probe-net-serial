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
#ifndef _ETH_DRIVER_ENC_H_
#define _ETH_DRIVER_ENC_H_

#define ENC_PT_GPWR 0x0C

void enc_init (s_spi *e_spi);
int  enc_reset(void);
void enc_bxsel(int n);
void enc_sbi  (u32 cmd);
void enc_bfsc (u8 adr, u8 mask, u8 set);
u8   enc_rcr(u8 radr);
u8   enc_rcru(u8 radr);
void enc_wcr(u8 radr, u8 rval);
u32  enc_r_pt(u8 id);
void enc_w_pt(u8 id, u32 addr);
u32  enc_rrxrdpt(void);
void enc_wrxrdpt(u32 addr);
u32  enc_rgprdpt(void);
void enc_wgprdpt(u32 addr);
void enc_wgpwrpt(u32 addr);
u32  enc_rgpwrpt(void);
u8   enc_rrxdata(int len, char *buffer);
void enc_wtxdata(int len, unsigned char *buffer);

#endif
