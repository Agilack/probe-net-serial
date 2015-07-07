/**
 * Registers mapping for EFM8BB1x MCUs
 *
 */
#ifndef EFM8BB1_H
#define EFM8BB1_H

__sfr __at 0xE0 ACC;     // Accumulator
__sfr __at 0xF0 B;       // B Register                             
__sfr __at 0x81 SP;      // Stack Pointer
__sfr __at 0x83 DPH;     // Data Pointer High
__sfr __at 0x82 DPL;     // Data Pointer Low
__sfr __at 0xEF RSTSRC;  // Reset Source
__sfr __at 0xB5 DEVICEID;// Device Identification
__sfr __at 0xAD DERIVID; // Derivative Identification
__sfr __at 0xB6 REVID;   // Revision Identifcation

// Clocks, Oscillators, and power
__sfr __at 0x8E CKCON0;  // Clock Control 0
__sfr __at 0xA9 CLKSEL;  // Clock Select
__sfr __at 0xB1 LFO0CN;  // Low Frequency Oscillator Control
__sfr __at 0xC7 HFO0CAL; // High Frequency Oscillator 0 Calibration
__sfr __at 0x87 PCON0;   // Power Control
__sfr __at 0xFF VDM0CN;  // Supply Monitor Control
__sfr __at 0xD1 REF0CN;  // Voltage Reference Control
__sfr __at 0xC9 REG0CN;  // Voltage Regulator 0 Control

// Interrupts
__sfr __at 0xE4 IT01CF;  // INT0/INT1 Configuration
__sfr __at 0xA8 IE;      // Interrupt Enable
__sfr __at 0xB8 IP;      // Interrupt Priority
__sfr __at 0xE6 EIE1;    // Extended Interrupt Enable 1
__sfr __at 0xF3 EIP1;    // Extended Interrupt Priority 1

// Timers
__sfr __at 0x88 TCON;    // Timer 0/1 Control
__sfr __at 0x89 TMOD;    // Timer 0/1 Mode
__sfr __at 0x8C TH0;     // Timer 0 High Byte
__sfr __at 0x8A TL0;     // Timer 0 Low Byte
__sfr __at 0x8D TH1;     // Timer 1 High Byte
__sfr __at 0x8B TL1;     // Timer 1 Low Byte
__sfr __at 0xC8 TMR2CN0; // Timer 2 Control 0
__sfr __at 0xCD TMR2H;   // Timer 2 High Byte
__sfr __at 0xCC TMR2L;   // Timer 2 Low Byte
__sfr __at 0xCB TMR2RLH; // Timer 2 Reload High Byte
__sfr __at 0xCA TMR2RLL; // Timer 2 Reload Low Byte
__sfr __at 0x91 TMR3CN0; // Timer 3 Control 0
__sfr __at 0x95 TMR3H;   // Timer 3 High Byte
__sfr __at 0x94 TMR3L;   // Timer 3 Low Byte
__sfr __at 0x93 TMR3RLH; // Timer 3 Reload High Byte
__sfr __at 0x92 TMR3RLL; // Timer 3 Reload Low Byte
__sfr __at 0x97 WDTCN;   // Watchdog Timer Control

// Communication : UART
__sfr __at 0x98 SCON0;   // UART0 Serial Port Control
__sfr __at 0x99 SBUF0;   // UART0 Serial Port Data Buffer

// Communication : SPI
__sfr __at 0xA1 SPI0CFG; // SPI0 Configuration
__sfr __at 0xA2 SPI0CKR; // SPI0 Clock Rate
__sfr __at 0xF8 SPI0CN0; // SPI0 Control
__sfr __at 0xA3 SPI0DAT; // SPI0 Data

// Communication : SMBus
__sfr __at 0xC0 SMB0CN0; // SMBus 0 Control
__sfr __at 0xC1 SMB0CF;  // SMBus 0 Configuration
__sfr __at 0xC2 SMB0DAT; // SMBus 0 Data
__sfr __at 0xD6 SMB0ADM; // SMBus 0 Slave Address Mask
__sfr __at 0xD7 SMB0ADR; // SMBus 0 Slave Address
__sfr __at 0xAC SMB0TC;  // SMBus 0 Timing and Pin Control

// Programmable Counter Array - PWM
__sfr __at 0xD8 PCA0CN0;  // PCA Control
__sfr __at 0xD9 PCA0MD;   // PCA Mode
__sfr __at 0x96 PCA0POL;  // PCA Output Polarity
__sfr __at 0x9C PCA0CLR;  // PCA Comparator Clear Control
__sfr __at 0x9E PCA0CENT; // PCA Center Alignment Enable
__sfr __at 0xDA PCA0CPM0; // PCA Channel 0 Capture/Compare Mode
__sfr __at 0xFC PCA0CPH0; // PCA Channel 0 Capture Module High Byte
__sfr __at 0xFB PCA0CPL0; // PCA Channel 0 Capture Module Low Byte
__sfr __at 0xDB PCA0CPM1; // PCA Channel 1 Capture/Compare Mode
__sfr __at 0xEA PCA0CPH1; // PCA Channel 1 Capture Module High Byte
__sfr __at 0xE9 PCA0CPL1; // PCA Channel 1 Capture Module Low Byte
__sfr __at 0xDC PCA0CPM2; // PCA Channel 2 Capture/Compare Mode
__sfr __at 0xEC PCA0CPH2; // PCA Channel 2 Capture Module High Byte
__sfr __at 0xEB PCA0CPL2; // PCA Channel 2 Capture Module Low Byte
__sfr __at 0xFA PCA0H;    // PCA Counter/Timer High Byte
__sfr __at 0xF9 PCA0L;    // PCA Counter/Timer Low Byte
__sfr __at 0xF7 PCA0PWM;  // PCA PWM Configuration

// GPIO ports
__sfr __at 0xE1 XBR0;    // Port I/O Crossbar 0
__sfr __at 0xE2 XBR1;    // Port I/O Crossbar 1
__sfr __at 0xE3 XBR2;    // Port I/O Crossbar 2
__sfr __at 0xF6 PRTDRV;  // Port Drive Strength
__sfr __at 0x80 P0;      // Port 0 Pin Latch
__sfr __at 0xFE P0MASK;  // Port 0 Mask
__sfr __at 0xFD P0MAT;   // Port 0 Match
__sfr __at 0xF1 P0MDIN;  // Port 0 Input Mode
__sfr __at 0xA4 P0MDOUT; // Port 0 Output Mode
__sfr __at 0xD4 P0SKIP;  // Port 0 Skip
__sfr __at 0x90 P1;      // Port 1 Pin Latch
__sfr __at 0xEE P1MASK;  // Port 1 Mask
__sfr __at 0xED P1MAT;   // Port 1 Match
__sfr __at 0xF2 P1MDIN;  // Port 1 Input Mode
__sfr __at 0xA5 P1MDOUT; // Port 1 Output Mode
__sfr __at 0xD5 P1SKIP;  // Port 1 Skip
__sfr __at 0xA0 P2;      // Port 2 Pin Latch
__sfr __at 0xA6 P2MDOUT; // Port 2 Output Mode

// Analog converter
__sfr __at 0xB3 ADC0AC;  // ADC0 Accumulator Configuration
__sfr __at 0xBC ADC0CF;  // ADC0 Configuration
__sfr __at 0xE8 ADC0CN0; // ADC0 Control 0
__sfr __at 0xB2 ADC0CN1; // ADC0 Control 1
__sfr __at 0xC4 ADC0GTH; // ADC0 Greater-Than High Byte
__sfr __at 0xC3 ADC0GTL; // ADC0 Greater-Than Low Byte
__sfr __at 0xBE ADC0H;   // ADC0 Data Word High Byte
__sfr __at 0xBD ADC0L;   // ADC0 Data Word Low Byte
__sfr __at 0xC6 ADC0LTH; // ADC0 Less-Than High Byte
__sfr __at 0xC5 ADC0LTL; // ADC0 Less-Than Low Byte
__sfr __at 0xBB ADC0MX;  // ADC0 Multiplexer Selection
__sfr __at 0xDF ADC0PWR; // ADC0 Power Control
__sfr __at 0xB9 ADC0TK;  // ADC0 Burst Mode Track Time

// Comparators
__sfr __at 0x9B CMP0CN0; // Comparator 0 Control 0
__sfr __at 0x9D CMP0MD;  // Comparator 0 Mode
__sfr __at 0x9F CMP0MX;  // Comparator 0 Multiplexer Selection
__sfr __at 0xBF CMP1CN0; // Comparator 1 Control 0
__sfr __at 0xAB CMP1MD;  // Comparator 1 Mode
__sfr __at 0xAA CMP1MX;  // Comparator 1 Multiplexer Selection

// CRC controller
__sfr __at 0xD2 CRC0AUTO;// CRC0 Automatic Control
__sfr __at 0xCE CRC0CN0; // CRC0 Control 0
__sfr __at 0xD3 CRC0CNT; // CRC0 Automatic Flash Sector Count
__sfr __at 0xDE CRC0DAT; // CRC0 Data Output
__sfr __at 0xCF CRC0FLIP;// CRC0 Bit Flip
__sfr __at 0xDD CRC0IN;  // CRC0 Data Input

// Flash and memory protect
__sfr __at 0xB7 FLKEY;   // Flash Lock and Key
__sfr __at 0x8F PSCTL;   // Program Store Control
__sfr __at 0xD0 PSW;     // Program Status Word

__sfr __at 0xAA _XPAGE;  // Point to the EMI0CN register

// SCON0 bits
__sbit __at (0x98) SCON0_RI;

// Port1 data bits
__sbit __at (0x90) P1_0;
__sbit __at (0x91) P1_1;
__sbit __at (0x92) P1_2;
__sbit __at (0x93) P1_3;
__sbit __at (0x94) P1_4;
__sbit __at (0x95) P1_5;
__sbit __at (0x96) P1_6;
__sbit __at (0x97) P1_7;

// SPI0 - CN0 bits
__sbit __at (0xF8) SPI0CN0_SPIEN;
__sbit __at (0xF9) SPI0CN0_TXBMT;
__sbit __at (0xFA) SPI0CN0_NSSMD0;
__sbit __at (0xFB) SPI0CN0_NSSMD1;
__sbit __at (0xFC) SPI0CN0_RXOVRN;
__sbit __at (0xFD) SPI0CN0_MODF;
__sbit __at (0xFE) SPI0CN0_WCOL;
__sbit __at (0xFF) SPI0CN0_SPIF;


#endif // EFM8BB1_H
