EESchema Schematic File Version 2
LIBS:display-r01
LIBS:display-r01-cache
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "Display daughter board"
Date ""
Rev "0.1"
Comp "Agilack"
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ER-OLED0.91-3 DISP2
U 1 1 55460A4F
P 8900 4950
F 0 "DISP2" H 9150 4350 60  0000 C CNN
F 1 "ER-OLED0.91-3" H 8300 5550 60  0000 C CNN
F 2 "" H 8900 4950 60  0000 C CNN
F 3 "" H 8900 4950 60  0000 C CNN
	1    8900 4950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 55460A75
P 8900 5650
F 0 "#PWR01" H 8900 5650 30  0001 C CNN
F 1 "GND" H 8900 5580 30  0001 C CNN
F 2 "" H 8900 5650 60  0000 C CNN
F 3 "" H 8900 5650 60  0000 C CNN
	1    8900 5650
	1    0    0    -1  
$EndComp
$Comp
L C C101
U 1 1 5546212E
P 9750 1500
F 0 "C101" V 9850 1550 50  0000 L CNN
F 1 "1uF" V 9850 1250 50  0000 L CNN
F 2 "" H 9750 1500 60  0000 C CNN
F 3 "" H 9750 1500 60  0000 C CNN
	1    9750 1500
	0    -1   -1   0   
$EndComp
$Comp
L C C102
U 1 1 5546214B
P 9750 1900
F 0 "C102" V 9850 1950 50  0000 L CNN
F 1 "1uF" V 9850 1650 50  0000 L CNN
F 2 "" H 9750 1900 60  0000 C CNN
F 3 "" H 9750 1900 60  0000 C CNN
	1    9750 1900
	0    -1   -1   0   
$EndComp
$Comp
L ER-OLED0.91-1 DISP1
U 1 1 554868DB
P 8900 1850
F 0 "DISP1" H 9150 1250 60  0000 C CNN
F 1 "ER-OLED0.91-1" H 8250 2450 60  0000 C CNN
F 2 "" H 8900 1850 60  0000 C CNN
F 3 "" H 8900 1850 60  0000 C CNN
	1    8900 1850
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR02
U 1 1 55486949
P 8900 2550
F 0 "#PWR02" H 8900 2550 30  0001 C CNN
F 1 "GND" H 8900 2480 30  0001 C CNN
F 2 "" H 8900 2550 60  0000 C CNN
F 3 "" H 8900 2550 60  0000 C CNN
	1    8900 2550
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P1
U 1 1 5550CCF8
P 1250 1700
F 0 "P1" H 1330 1700 40  0000 L CNN
F 1 "CONN_1" H 1250 1755 30  0001 C CNN
F 2 "" H 1250 1700 60  0000 C CNN
F 3 "" H 1250 1700 60  0000 C CNN
F 4 "Samtec" H 1250 1700 60  0001 C CNN "Manufacturer"
F 5 "SSW-101-01-G-S" H 1250 1700 60  0001 C CNN "Manuf Ref"
	1    1250 1700
	-1   0    0    1   
$EndComp
$Comp
L CONN_1 P2
U 1 1 5550CD07
P 1250 2000
F 0 "P2" H 1330 2000 40  0000 L CNN
F 1 "CONN_1" H 1250 2055 30  0001 C CNN
F 2 "" H 1250 2000 60  0000 C CNN
F 3 "" H 1250 2000 60  0000 C CNN
F 4 "Samtec" H 1250 2000 60  0001 C CNN "Manufacturer"
F 5 "SSW-101-01-G-S" H 1250 2000 60  0001 C CNN "Manuf Ref"
	1    1250 2000
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR03
U 1 1 5550CD16
P 1500 2100
F 0 "#PWR03" H 1500 2100 30  0001 C CNN
F 1 "GND" H 1500 2030 30  0001 C CNN
F 2 "" H 1500 2100 60  0000 C CNN
F 3 "" H 1500 2100 60  0000 C CNN
	1    1500 2100
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR04
U 1 1 5550CD4A
P 1500 1600
F 0 "#PWR04" H 1500 1560 30  0001 C CNN
F 1 "+3.3V" H 1500 1710 30  0000 C CNN
F 2 "" H 1500 1600 60  0000 C CNN
F 3 "" H 1500 1600 60  0000 C CNN
	1    1500 1600
	1    0    0    -1  
$EndComp
$Comp
L TLV61220 U2
U 1 1 555B4BBA
P 2950 4700
F 0 "U2" H 2950 5100 60  0000 C CNN
F 1 "TLV61220" H 3250 4300 60  0000 C CNN
F 2 "" H 2950 4550 60  0000 C CNN
F 3 "" H 2950 4550 60  0000 C CNN
	1    2950 4700
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR L101
U 1 1 555B4BC9
P 2150 4500
F 0 "L101" V 2300 4500 60  0000 C CNN
F 1 "4.7uH" V 2100 4500 40  0000 C CNN
F 2 "" H 2150 4500 60  0000 C CNN
F 3 "" H 2150 4500 60  0000 C CNN
F 4 "Laird Technologies" V 2150 4500 60  0001 C CNN "Manufacturer"
F 5 "TYS40124R7M-10" V 2150 4500 60  0001 C CNN "Manuf Ref"
	1    2150 4500
	0    -1   -1   0   
$EndComp
$Comp
L +3.3V #PWR05
U 1 1 555B4F77
P 1450 4600
F 0 "#PWR05" H 1450 4560 30  0001 C CNN
F 1 "+3.3V" H 1450 4710 30  0000 C CNN
F 2 "" H 1450 4600 60  0000 C CNN
F 3 "" H 1450 4600 60  0000 C CNN
	1    1450 4600
	1    0    0    -1  
$EndComp
$Comp
L C C105
U 1 1 555B4F86
P 1550 4950
F 0 "C105" H 1600 5050 50  0000 L CNN
F 1 "10uF" H 1600 4850 50  0000 L CNN
F 2 "" H 1550 4950 60  0000 C CNN
F 3 "" H 1550 4950 60  0000 C CNN
F 4 "AVX" H 1550 4950 60  0001 C CNN "Manufacturer"
F 5 "08056C106KAT2A" H 1550 4950 60  0001 C CNN "Manuf Ref"
	1    1550 4950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 555B4F95
P 1550 5200
F 0 "#PWR06" H 1550 5200 30  0001 C CNN
F 1 "GND" H 1550 5130 30  0001 C CNN
F 2 "" H 1550 5200 60  0000 C CNN
F 3 "" H 1550 5200 60  0000 C CNN
	1    1550 5200
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR07
U 1 1 555B5057
P 2950 5200
F 0 "#PWR07" H 2950 5200 30  0001 C CNN
F 1 "GND" H 2950 5130 30  0001 C CNN
F 2 "" H 2950 5200 60  0000 C CNN
F 3 "" H 2950 5200 60  0000 C CNN
	1    2950 5200
	1    0    0    -1  
$EndComp
$Comp
L R R101
U 1 1 555B509D
P 3700 4800
F 0 "R101" V 3800 4800 50  0000 C CNN
F 1 "R" V 3700 4800 50  0000 C CNN
F 2 "" H 3700 4800 60  0000 C CNN
F 3 "" H 3700 4800 60  0000 C CNN
	1    3700 4800
	1    0    0    -1  
$EndComp
$Comp
L R R102
U 1 1 555B50AC
P 3700 5400
F 0 "R102" V 3800 5400 50  0000 C CNN
F 1 "R" V 3700 5400 50  0000 C CNN
F 2 "" H 3700 5400 60  0000 C CNN
F 3 "" H 3700 5400 60  0000 C CNN
	1    3700 5400
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR08
U 1 1 555B50C5
P 3700 5700
F 0 "#PWR08" H 3700 5700 30  0001 C CNN
F 1 "GND" H 3700 5630 30  0001 C CNN
F 2 "" H 3700 5700 60  0000 C CNN
F 3 "" H 3700 5700 60  0000 C CNN
	1    3700 5700
	1    0    0    -1  
$EndComp
$Comp
L C C103
U 1 1 555B52F8
P 4100 4800
F 0 "C103" H 4150 4900 50  0000 L CNN
F 1 "10uF" H 4150 4700 50  0000 L CNN
F 2 "" H 4100 4800 60  0000 C CNN
F 3 "" H 4100 4800 60  0000 C CNN
F 4 "AVX" H 4100 4800 60  0001 C CNN "Manufacturer"
F 5 "08056C106KAT2A" H 4100 4800 60  0001 C CNN "Manuf Ref"
	1    4100 4800
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR09
U 1 1 555B540D
P 4100 5050
F 0 "#PWR09" H 4100 5050 30  0001 C CNN
F 1 "GND" H 4100 4980 30  0001 C CNN
F 2 "" H 4100 5050 60  0000 C CNN
F 3 "" H 4100 5050 60  0000 C CNN
	1    4100 5050
	1    0    0    -1  
$EndComp
$Comp
L +VDISP #PWR010
U 1 1 555B565D
P 4600 4400
F 0 "#PWR010" H 4600 4360 30  0001 C CNN
F 1 "+VDISP" H 4600 4510 30  0000 C CNN
F 2 "" H 4600 4400 60  0000 C CNN
F 3 "" H 4600 4400 60  0000 C CNN
	1    4600 4400
	1    0    0    -1  
$EndComp
$Comp
L EFM8BB10F8G-A-SOIC16 U1
U 1 1 555B5D86
P 4650 1950
F 0 "U1" H 4400 2450 60  0000 C CNN
F 1 "EFM8BB10F8G-A-SOIC16" H 5200 2450 50  0000 C CNN
F 2 "" H 4650 1750 60  0000 C CNN
F 3 "" H 4650 1750 60  0000 C CNN
	1    4650 1950
	1    0    0    -1  
$EndComp
Text GLabel 5200 1600 2    45   Output ~ 0
SPI_SCK
Text GLabel 8250 1900 0    45   Input ~ 0
SPI_SCK
Text GLabel 5200 1900 2    45   Output ~ 0
SPI_CS
Text GLabel 8250 1800 0    45   Input ~ 0
SPI_CS
Text GLabel 8250 2000 0    45   Input ~ 0
SPI_MOSI
Text GLabel 5200 1800 2    45   Output ~ 0
SPI_MOSI
$Comp
L CONN_1 MISO1
U 1 1 555B5EAC
P 5350 1700
F 0 "MISO1" H 5430 1700 40  0000 L CNN
F 1 "CONN_1" H 5350 1755 30  0001 C CNN
F 2 "" H 5350 1700 60  0000 C CNN
F 3 "" H 5350 1700 60  0000 C CNN
	1    5350 1700
	1    0    0    -1  
$EndComp
Text GLabel 5200 2100 2    45   Input ~ 0
UART_RX
$Comp
L R R103
U 1 1 555B67C8
P 8250 2600
F 0 "R103" V 8350 2600 50  0000 C CNN
F 1 "560k" V 8250 2600 45  0000 C CNN
F 2 "" H 8250 2600 60  0000 C CNN
F 3 "" H 8250 2600 60  0000 C CNN
	1    8250 2600
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR011
U 1 1 555B6743
P 8900 1100
F 0 "#PWR011" H 8900 1060 30  0001 C CNN
F 1 "+3.3V" H 8900 1210 30  0000 C CNN
F 2 "" H 8900 1100 60  0000 C CNN
F 3 "" H 8900 1100 60  0000 C CNN
	1    8900 1100
	1    0    0    -1  
$EndComp
$Comp
L +VDISP #PWR012
U 1 1 555B6752
P 8600 1100
F 0 "#PWR012" H 8600 1060 30  0001 C CNN
F 1 "+VDISP" H 8600 1210 30  0000 C CNN
F 2 "" H 8600 1100 60  0000 C CNN
F 3 "" H 8600 1100 60  0000 C CNN
	1    8600 1100
	1    0    0    -1  
$EndComp
$Comp
L C C107
U 1 1 555B684D
P 9550 2600
F 0 "C107" H 9600 2700 50  0000 L CNN
F 1 "2,2uF" H 9600 2500 50  0000 L CNN
F 2 "" H 9550 2600 60  0000 C CNN
F 3 "" H 9550 2600 60  0000 C CNN
	1    9550 2600
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 555B6914
P 8250 2900
F 0 "#PWR013" H 8250 2900 30  0001 C CNN
F 1 "GND" H 8250 2830 30  0001 C CNN
F 2 "" H 8250 2900 60  0000 C CNN
F 3 "" H 8250 2900 60  0000 C CNN
	1    8250 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 555B6923
P 9550 2850
F 0 "#PWR014" H 9550 2850 30  0001 C CNN
F 1 "GND" H 9550 2780 30  0001 C CNN
F 2 "" H 9550 2850 60  0000 C CNN
F 3 "" H 9550 2850 60  0000 C CNN
	1    9550 2850
	1    0    0    -1  
$EndComp
$Comp
L C C106
U 1 1 555B6A3A
P 9400 1000
F 0 "C106" H 9450 1100 50  0000 L CNN
F 1 "4,7uF" H 9450 900 50  0000 L CNN
F 2 "" H 9400 1000 60  0000 C CNN
F 3 "" H 9400 1000 60  0000 C CNN
	1    9400 1000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 555B6A53
P 9400 1250
F 0 "#PWR015" H 9400 1250 30  0001 C CNN
F 1 "GND" H 9400 1180 30  0001 C CNN
F 2 "" H 9400 1250 60  0000 C CNN
F 3 "" H 9400 1250 60  0000 C CNN
	1    9400 1250
	1    0    0    -1  
$EndComp
Text GLabel 8250 1500 0    45   Input ~ 0
DISP_RST
Text GLabel 8250 1700 0    45   Input ~ 0
DISP_DC
Text GLabel 4100 1700 0    45   Output ~ 0
DISP_RST
$Comp
L +3.3V #PWR016
U 1 1 555DE30B
P 4650 1300
F 0 "#PWR016" H 4650 1260 30  0001 C CNN
F 1 "+3.3V" H 4650 1410 30  0000 C CNN
F 2 "" H 4650 1300 60  0000 C CNN
F 3 "" H 4650 1300 60  0000 C CNN
	1    4650 1300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 555DE3AB
P 4650 2600
F 0 "#PWR017" H 4650 2600 30  0001 C CNN
F 1 "GND" H 4650 2530 30  0001 C CNN
F 2 "" H 4650 2600 60  0000 C CNN
F 3 "" H 4650 2600 60  0000 C CNN
	1    4650 2600
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 TP101
U 1 1 555DE6E4
P 5350 2000
F 0 "TP101" H 5430 2000 40  0000 L CNN
F 1 "CONN_1" H 5350 2055 30  0001 C CNN
F 2 "" H 5350 2000 60  0000 C CNN
F 3 "" H 5350 2000 60  0000 C CNN
	1    5350 2000
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 P3
U 1 1 555DE6F3
P 1250 2300
F 0 "P3" H 1330 2300 40  0000 L CNN
F 1 "CONN_1" H 1250 2355 30  0001 C CNN
F 2 "" H 1250 2300 60  0000 C CNN
F 3 "" H 1250 2300 60  0000 C CNN
	1    1250 2300
	-1   0    0    1   
$EndComp
Text GLabel 1500 2300 2    45   Output ~ 0
UART_RX
$Comp
L CONN_1 TP102
U 1 1 555DF80C
P 5350 2200
F 0 "TP102" H 5430 2200 40  0000 L CNN
F 1 "CONN_1" H 5350 2255 30  0001 C CNN
F 2 "" H 5350 2200 60  0000 C CNN
F 3 "" H 5350 2200 60  0000 C CNN
	1    5350 2200
	1    0    0    -1  
$EndComp
$Comp
L CONN_1 TP103
U 1 1 555DF825
P 5350 2300
F 0 "TP103" H 5430 2300 40  0000 L CNN
F 1 "CONN_1" H 5350 2355 30  0001 C CNN
F 2 "" H 5350 2300 60  0000 C CNN
F 3 "" H 5350 2300 60  0000 C CNN
	1    5350 2300
	1    0    0    -1  
$EndComp
Text GLabel 4100 1600 0    45   Output ~ 0
DISP_DC
$Comp
L C C109
U 1 1 5562EEA2
P 9750 4600
F 0 "C109" H 9800 4700 50  0000 L CNN
F 1 "1uF" V 9850 4350 50  0000 L CNN
F 2 "" H 9750 4600 60  0000 C CNN
F 3 "" H 9750 4600 60  0000 C CNN
	1    9750 4600
	0    -1   -1   0   
$EndComp
$Comp
L C C110
U 1 1 5562EEB1
P 9750 5000
F 0 "C110" H 9800 5100 50  0000 L CNN
F 1 "1uF" V 9850 4750 50  0000 L CNN
F 2 "" H 9750 5000 60  0000 C CNN
F 3 "" H 9750 5000 60  0000 C CNN
	1    9750 5000
	0    -1   -1   0   
$EndComp
$Comp
L C C108
U 1 1 5562F26A
P 9400 4050
F 0 "C108" H 9450 4150 50  0000 L CNN
F 1 "4,7uF" H 9450 3950 50  0000 L CNN
F 2 "" H 9400 4050 60  0000 C CNN
F 3 "" H 9400 4050 60  0000 C CNN
	1    9400 4050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 5562F283
P 9400 4350
F 0 "#PWR018" H 9400 4350 30  0001 C CNN
F 1 "GND" H 9400 4280 30  0001 C CNN
F 2 "" H 9400 4350 60  0000 C CNN
F 3 "" H 9400 4350 60  0000 C CNN
	1    9400 4350
	1    0    0    -1  
$EndComp
$Comp
L C C111
U 1 1 5562F46A
P 9550 5700
F 0 "C111" H 9600 5800 50  0000 L CNN
F 1 "C" H 9600 5600 50  0000 L CNN
F 2 "" H 9550 5700 60  0000 C CNN
F 3 "" H 9550 5700 60  0000 C CNN
	1    9550 5700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR019
U 1 1 5562F483
P 9550 5950
F 0 "#PWR019" H 9550 5950 30  0001 C CNN
F 1 "GND" H 9550 5880 30  0001 C CNN
F 2 "" H 9550 5950 60  0000 C CNN
F 3 "" H 9550 5950 60  0000 C CNN
	1    9550 5950
	1    0    0    -1  
$EndComp
$Comp
L +VDISP #PWR020
U 1 1 5562FEC8
P 8600 4200
F 0 "#PWR020" H 8600 4160 30  0001 C CNN
F 1 "+VDISP" H 8600 4310 30  0000 C CNN
F 2 "" H 8600 4200 60  0000 C CNN
F 3 "" H 8600 4200 60  0000 C CNN
	1    8600 4200
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR021
U 1 1 556300E5
P 8900 4200
F 0 "#PWR021" H 8900 4160 30  0001 C CNN
F 1 "+3.3V" H 8900 4310 30  0000 C CNN
F 2 "" H 8900 4200 60  0000 C CNN
F 3 "" H 8900 4200 60  0000 C CNN
	1    8900 4200
	1    0    0    -1  
$EndComp
Text GLabel 8250 4600 0    45   Input ~ 0
DISP_RST
Wire Wire Line
	8900 5650 8900 5600
Wire Wire Line
	9950 1500 10050 1500
Wire Wire Line
	10050 1500 10050 1650
Wire Wire Line
	10050 1650 9450 1650
Wire Wire Line
	9450 1500 9550 1500
Wire Wire Line
	9450 1900 9550 1900
Wire Wire Line
	9450 2050 10050 2050
Wire Wire Line
	10050 2050 10050 1900
Wire Wire Line
	10050 1900 9950 1900
Wire Wire Line
	8900 2550 8900 2500
Wire Wire Line
	1500 2100 1500 2000
Wire Wire Line
	1500 2000 1400 2000
Wire Wire Line
	1500 1600 1500 1700
Wire Wire Line
	1500 1700 1400 1700
Wire Wire Line
	2450 4500 2550 4500
Wire Wire Line
	2550 4900 2450 4900
Wire Wire Line
	2450 4900 2450 4700
Wire Wire Line
	1450 4700 2550 4700
Wire Wire Line
	1750 4700 1750 4500
Wire Wire Line
	1750 4500 1850 4500
Connection ~ 2450 4700
Wire Wire Line
	1450 4600 1450 4700
Connection ~ 1750 4700
Wire Wire Line
	1550 5200 1550 5150
Wire Wire Line
	1550 4750 1550 4700
Connection ~ 1550 4700
Wire Wire Line
	2950 5200 2950 5150
Wire Wire Line
	3700 5700 3700 5650
Wire Wire Line
	3700 5150 3700 5050
Wire Wire Line
	3700 5100 3500 5100
Wire Wire Line
	3500 5100 3500 4900
Wire Wire Line
	3500 4900 3350 4900
Connection ~ 3700 5100
Wire Wire Line
	3350 4500 4600 4500
Wire Wire Line
	3700 4500 3700 4550
Wire Wire Line
	4100 5050 4100 5000
Wire Wire Line
	4100 4500 4100 4600
Connection ~ 3700 4500
Connection ~ 4100 4500
Wire Wire Line
	4600 4500 4600 4400
Wire Wire Line
	5200 1600 5100 1600
Wire Wire Line
	8250 1900 8350 1900
Wire Wire Line
	8250 1800 8350 1800
Wire Wire Line
	8250 2000 8350 2000
Wire Wire Line
	5200 1700 5100 1700
Wire Wire Line
	5200 2100 5100 2100
Wire Wire Line
	5100 2000 5200 2000
Wire Wire Line
	8600 1100 8600 1150
Wire Wire Line
	8600 1150 8800 1150
Wire Wire Line
	8800 1150 8800 1200
Wire Wire Line
	8900 1200 8900 1100
Wire Wire Line
	8250 2350 8250 2250
Wire Wire Line
	8150 2250 8350 2250
Wire Wire Line
	9550 2250 9550 2400
Wire Wire Line
	9550 2250 9450 2250
Wire Wire Line
	9550 2850 9550 2800
Wire Wire Line
	8250 2900 8250 2850
Wire Wire Line
	9400 1250 9400 1200
Wire Wire Line
	9400 800  9400 750 
Wire Wire Line
	9400 750  9200 750 
Wire Wire Line
	9200 750  9200 1150
Wire Wire Line
	9200 1150 9000 1150
Wire Wire Line
	9000 1150 9000 1200
Wire Wire Line
	8250 1500 8350 1500
Wire Wire Line
	8350 1700 8250 1700
Wire Wire Line
	5200 1800 5100 1800
Wire Wire Line
	5200 1900 5100 1900
Wire Wire Line
	4650 1300 4650 1350
Wire Wire Line
	4650 2600 4650 2550
Wire Wire Line
	1500 2300 1400 2300
Wire Wire Line
	5200 2200 5100 2200
Wire Wire Line
	5100 2300 5200 2300
Wire Wire Line
	4100 1600 4200 1600
Wire Wire Line
	4100 1700 4200 1700
Wire Wire Line
	9450 4750 10050 4750
Wire Wire Line
	10050 4750 10050 4600
Wire Wire Line
	10050 4600 9950 4600
Wire Wire Line
	9550 4600 9450 4600
Wire Wire Line
	9550 5000 9450 5000
Wire Wire Line
	9450 5150 10050 5150
Wire Wire Line
	10050 5150 10050 5000
Wire Wire Line
	10050 5000 9950 5000
Wire Wire Line
	9400 4250 9400 4350
Wire Wire Line
	9200 3800 9400 3800
Wire Wire Line
	9400 3800 9400 3850
Wire Wire Line
	9000 4300 9000 4250
Wire Wire Line
	9000 4250 9200 4250
Wire Wire Line
	9200 4250 9200 3800
Wire Wire Line
	9550 5950 9550 5900
Wire Wire Line
	9550 5500 9550 5350
Wire Wire Line
	9550 5350 9450 5350
Wire Wire Line
	8250 5350 8350 5350
Wire Wire Line
	8600 4200 8600 4250
Wire Wire Line
	8600 4250 8800 4250
Wire Wire Line
	8800 4250 8800 4300
Wire Wire Line
	8900 4200 8900 4300
Wire Wire Line
	8250 4600 8350 4600
Wire Wire Line
	8250 4800 8350 4800
Wire Wire Line
	8250 4900 8350 4900
Wire Wire Line
	8250 5000 8350 5000
Wire Wire Line
	8250 5100 8350 5100
Text GLabel 8250 4800 0    45   Input ~ 0
DISP_DC
Text GLabel 8250 4900 0    45   Input ~ 0
SPI_CS
Text GLabel 8250 5000 0    45   Input ~ 0
SPI_SCK
Text GLabel 8250 5100 0    45   Input ~ 0
SPI_MOSI
Text GLabel 8150 2250 0    45   UnSpc ~ 0
IREF
Connection ~ 8250 2250
Text GLabel 8250 5350 0    45   UnSpc ~ 0
IREF
NoConn ~ 4200 1800
NoConn ~ 4200 1900
$Comp
L CONN_1 PP1
U 1 1 55632950
P 3950 2100
F 0 "PP1" H 4030 2100 40  0000 L CNN
F 1 "CONN_1" H 3950 2155 30  0001 C CNN
F 2 "" H 3950 2100 60  0000 C CNN
F 3 "" H 3950 2100 60  0000 C CNN
	1    3950 2100
	-1   0    0    1   
$EndComp
$Comp
L CONN_1 PP2
U 1 1 5563295F
P 3950 2300
F 0 "PP2" H 4030 2300 40  0000 L CNN
F 1 "CONN_1" H 3950 2355 30  0001 C CNN
F 2 "" H 3950 2300 60  0000 C CNN
F 3 "" H 3950 2300 60  0000 C CNN
	1    3950 2300
	-1   0    0    1   
$EndComp
Wire Wire Line
	4100 2100 4200 2100
Wire Wire Line
	4200 2300 4100 2300
$EndSCHEMATC