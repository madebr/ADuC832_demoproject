
ARCH = mcs51

#Fedora prefixes sdcc binaries
CCPREFIX=sdcc-

CC      = $(CCPREFIX)sdcc
CPP     = $(CCPREFIX)sdcpp
AS      = $(CCPREFIX)sdas8051
LD      = $(CCPREFIX)sdld
PACKIHX = $(CCPREFIX)packihx
HEX2BIN = $(CCPREFIX)makebin

OUTPUT_IHX=output.ihx
CSRCS=main.c clock.c lcd.c led.c pwm.c serial.c adc.c dac.c watchdog.c
ASRCS=
OBJS=$(CSRCS:.c=.rel) $(ASRCS:.asm=.rel)

#Options for memory model: small, medium, large or huge
MEMORY_MODEL = small
#ADuC832

INTERNAL_XRAMSIZE = 2048
HOME_POS=0x0000
ISEG_POS=0x0000
BSEG_POS=0x0000
#8051 architecture

CFLAGS = -S --std-c99 --Werror --model-$(MEMORY_MODEL) -m$(ARCH) --opt-code-size
AFLAGS = -plosgffwz
MFLAGS = -m${ARCH}
LDFLAGS = -n -mwux -i $(OUTPUT_IHX) -Y -b HOME=$(HOME_POS) \
    -b ISEG=$(ISEG_POS) -M -b BSEG=$(BSEG_POS) \
    -k /usr/share/sdcc/lib/small -l mcs51 -l libsdcc# -l libint -l liblong -l libfloat

DEBUG ?= 0
ifeq ($(DEBUG), 1)
  CFLAGS  += --debug
  AFLAGS  += -y
  LDFLAGS += -y
endif
.PRECIOUS: %.asm %.rel

.PHONY: all
all: $(OUTPUT_IHX)

.PHONY: clean
clean:
	rm -f $(OUTPUT_IHX) $(OUTPUT_IHX:.ihx=.map) $(OUTPUT_IHX:.ihx=.mem) $(OUTPUT_IHX:.ihx=.cdb) \
  $(OUTPUT_IHX:.ihx=.cdb) $(OUTPUT_IHX:.ihx=.omf) \
  $(CSRCS:.c=.d) $(CSRCS:.c=.lst) $(CSRCS:.c=.rel) $(CSRCS:.c=.sym) $(CSRCS:.c=.asm) \
  $(CSRCS:.c=.rst) $(CSRCS:.c=.adb) \
  $(ASRCS:.asm=.lst) $(ASRCS:.asm=.rel) $(ASRCS:.asm=.sym) $(ASRCS:.asm=.adb)

%.asm: %.c
	$(CC) $(CFLAGS) $< -o $@

%.rel: %.asm
	$(AS) $(AFLAGS) $@ $<

$(OUTPUT_IHX): $(OBJS)
	$(LD) $(LDFLAGS) $(OBJS)

#Dependency tracker for c sources
%.d: %.c
	@$(CC) -M $< | sed 's/$(<:.c=.rel)/$(?:.c=.asm)/' >$@

-include $(CSRCS:.c=.d)

