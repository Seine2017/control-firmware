# Name of C compiler:
CC = avr-gcc
# Flags passed to C preprocessor:
CPPFLAGS = -Isrc -DF_CPU=16000000
# Flags passed to C compiler:
CFLAGS = -Wall -mmcu=atmega328p
# Name of linker:
LD = avr-gcc
# Flags passed to linker:
LDFLAGS =
# Name of objcopy tool:
OBJCOPY = avr-objcopy

SOURCES = src/main.c src/control.c
OBJECTS = $(SOURCES:.c=.o)

all: control-firmware.hex

control-firmware.hex: control-firmware.elf
	$(OBJCOPY) -O ihex $< $@

control-firmware.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

*.o: *.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $<

clean:
	rm -f *.hex *.elf src/*.o
