MCU=atmega8
CFLAGS=-mmcu=$(MCU) -Os

# Generate hex file for zeptix.
zeptix.hex: zeptix.o
	avr-objcopy -j .text -j .data -O ihex zeptix.o zeptix.hex

# Generate all object files from their respective sources.
%.o: %.c
	avr-gcc $(CFLAGS) $< -o $*.o

# Load zeptix to atmega8 microcontroller.
load: zeptix.hex
	avrdude -c usbasp -p m8 -P usb -U flash:w:zeptix.hex

# Erase all object, hex and backup files
clean:
	rm -rf *.o *.hex *~
