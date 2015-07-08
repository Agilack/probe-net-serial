##
 # NetSerial Display Extention - Character firmware
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

FW = fw_display

all: main.o hardware.o spi.o display.o
	@echo " [LD] $(FW)"
	@$(CC) -mmcs51 main.rel hardware.rel spi.rel display.rel -o $(FW).ihx

.c.o:
	@echo " [CC] $@"
	@$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~
	rm -f *.rel *.lst *.sym *.asm *.rst
	rm -f $(FW).*
