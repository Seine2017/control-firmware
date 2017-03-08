# Board type (uno, promicro, nano):
BOARD = nano
# Name of microcontroller (atmega328p, atmega32u4):
MCU = atmega328p
# Microcontroller clock frequency (Hz).
F_CPU = 16000000

# Name of C compiler:
CC = avr-gcc
# Flags passed to C preprocessor:
CPPFLAGS = -Isrc -DF_CPU=$(F_CPU) -DBOARD_$(BOARD)=1
# Flags passed to C compiler:
CFLAGS = -O2 -Wall -Wno-unused-function -mmcu=$(MCU)
# Name of linker:
LD = avr-gcc
# Flags passed to linker:
LDFLAGS = -mmcu=$(MCU)
# Name of objcopy tool:
OBJCOPY = avr-objcopy
# Name of size tool:
SIZE = avr-size

SOURCES = src/main.c src/control.c src/clock.c src/escs.c src/imu.c src/MadgwickAHRS.c src/debug.c
OBJECTS = $(SOURCES:.c=.o)

all: control-firmware.hex

control-firmware.hex: control-firmware.elf
	$(OBJCOPY) -O ihex $< $@
	$(SIZE) $@

control-firmware.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $^

*.o: *.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $<

clean:
	rm -f *.hex *.elf src/*.o

flash:
	avrdude -p$(MCU) -cc232hm -P/dev/ttyUSB0 -b57600 -U flash:w:control-firmware.hex:i
