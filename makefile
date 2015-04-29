MCU=atmega8
CFLAGS=-mmcu=$(MCU) -Os

zeptix.hex: zeptix.o
	avr-objcopy -j .text -j .data -O ihex zeptix.o zeptix.hex

zeptix.o: zeptix.c
	avr-gcc $(CFLAGS) zeptix.c -o zeptix.o

load: zeptix.hex
	avrdude -c usbasp -p m8 -P usb -U flash:w:zeptix.hex

clean:
	rm -r *.o *.hex *~
