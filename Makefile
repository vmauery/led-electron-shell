#UISP=uisp
OBJDUMP=avr-objdump
PROGRAMMER=/usr/bin/avrdude
DEVICE=m8

OBJCOPY=avr-objcopy
STRIP=avr-strip
CC=avr-gcc
MCU=atmega8
CFLAGS = -g -Os -mmcu=$(MCU) -MD
CFLAGS += -Wall -Werror -gdwarf-2 -fsigned-char
MAKEDEPEND = avr-gcc
SIZE = avr-size

HEX=electrons.hex
PROGRAMS=$(HEX:.hex=)
PROGOBJ=$(HEX:.hex=.o)
#LIBOBJ=usart.o
ALLOBJ=$(PROGOBJ) $(LIBOBJ)
ASSEMBLY=$(HEX:.hex=.asm)

DEPDIR = .deps
df = $(DEPDIR)/$(*F)

SRCS = $(ALLOBJ:.o=.c)

all: $(HEX)

%.o : %.c
	$(CC) $(CFLAGS) -c -o $@ $<
	@mkdir -p $(DEPDIR); cp $*.d $(df).d; \
	sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	    -e '/^$$/ d' -e 's/$$/ :/' < $(df).d >> $(df).d; \
	rm -f $*.d

-include $(SRCS:%.c=$(DEPDIR)/%.d)

lcd.h: Makefile
	echo '#include "'$(LCD_HEADER)'"' > lcd.h

$(PROGRAMS): %: %.o $(LIBRARY) 
	$(CC) $(CFLAGS) $@.o $(LIBRARY) -o $@

%-stripped: %
	$(STRIP) $^ -o $@
	$(SIZE) $^
	$(OBJDUMP) -D $^ > $^.lst

%.hex: %
	$(OBJCOPY) -O ihex $^ $@

p-%: %.hex
	$(PROGRAMMER) -p $(DEVICE) -P usb -c usbasp -y -e -U flash:w:$^
#	$(UISP) -dprog=$(PROG_METHOD) --segment=flash --erase 
#	$(UISP) -v=2 -dprog=$(PROG_METHOD) --segment=flash --upload if=$^ --verify

read:
	$(PROGRAMMER) -p $(DEVICE) -P usb -c usbasp -y -U flash:r:current.hex:i

$(LIBRARY): $(LIBOBJ)
	avr-ar -rcs $@ $(LIBOBJ)

clean:
	rm -rf $(HEX) $(PROGRAMS) $(ASSEMBLY) $(ALLOBJ) $(LIBRARY) $(DEPDIR) *.lst
