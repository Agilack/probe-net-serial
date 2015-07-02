##
 # NetSerial Display Extention - Unit test
 #
 # Copyright (c) 2015 Saint-Genest Gwenael <gwen@agilack.fr>
 #
 # This file may be distributed and/or modified under the terms of the
 # GNU General Public License version 2 as published by the Free Software
 # Foundation. (See COPYING.GPL for details.)
 #
 # This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 # WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
##
CC = sdcc-sdcc
CFLAGS = -mmcs51 -I.

FW = ext-disp-ut-ios

all:
	$(CC) $(CFLAGS) -c hardware.c
	$(CC) $(CFLAGS) -c main.c
	$(CC) -mmcs51 main.rel hardware.rel -o $(FW).ihx

clean:
	rm -f *~
	rm -f *.rel *.lst *.sym *.asm *.rst
	rm -f $(FW).*
