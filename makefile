MCU=atmega8
CFLAGS=-mmcu=$(MCU) -Os

zeptix.hex: zeptix.o
	avr-objcopy -j .text -j .data -O ihex zeptix.o zeptix.hex

# Generate all object files from their source.
%.o: %.c
	avr-gcc $(CFLAGS) $< -o $*.o

load: zeptix.hex
	avrdude -c usbasp -p m8 -P usb -U flash:w:zeptix.hex

clean:
	rm -rf *.o *.hex *~
