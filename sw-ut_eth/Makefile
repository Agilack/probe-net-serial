
CROSS = arm-none-eabi

# toolchain (using code sourcery now)
THUMB    = -mthumb
THUMB_IW = -mthumb-interwork

# Target file name (without extension).
BUILDDIR = build
TARGET = $(BUILDDIR)/ut_eth

# Define programs and commands.
CC = $(CROSS)-gcc
OBJCOPY = $(CROSS)-objcopy

ASFLAGS = -mcpu=cortex-m3 $(THUMB_IW) -I. -x assembler-with-cpp

CFLAGS = -mcpu=cortex-m3 $(THUMB_IW) -I. -g -O0
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS += -Wall -Wimplicit -Wcast-align -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -Inet

LDFLAGS = -nostartfiles -Wl,-Map=$(TARGET).map,--cref,--gc-sections
LDFLAGS += -lc -lgcc -Tc_only_md.ld

ASRC = c_only_startup.s cortexm3_macro.s

SRC = main.c hardware.c spi.c uart.c net/eth.c net/eth_drv_enc.c

# Define all object files.
_COBJ =  $(SRC:.c=.o)
_AOBJ =  $(ASRC:.s=.o)
COBJ = $(patsubst %, $(BUILDDIR)/%,$(_COBJ))
AOBJ = $(patsubst %, $(BUILDDIR)/%,$(_AOBJ))

# go!
all: env elf bin

bin: $(TARGET).bin
elf: $(TARGET).elf

env:
	mkdir -p $(BUILDDIR)
	mkdir -p $(BUILDDIR)/net
	mkdir -p $(BUILDDIR)/rfid
	mkdir -p $(BUILDDIR)/rtos

# Create final output file (.hex) from ELF output file.
%.hex: %.elf
	@echo
	$(OBJCOPY) -O binary $< $@

# Create final output file (.bin) from ELF output file.
%.bin: %.elf
	@echo
	$(OBJCOPY) -O binary $< $@


%.elf:  $(COBJ) $(AOBJ)
	$(CC) $(THUMB) $(CFLAGS) $(AOBJ) $(COBJ) --output $@ $(LDFLAGS)

$(COBJ) : $(BUILDDIR)/%.o : %.c
	$(CC) -c $(THUMB) $(CFLAGS) $< -o $@

$(AOBJ) : $(BUILDDIR)/%.o : %.s
	$(CC) -c $(THUMB) $(ASFLAGS) $< -o $@

clean:
	@echo
	rm -f $(TARGET).*
	rm -f $(COBJ)
	rm -f $(AOBJ)
	rm -f *~
