MCU=atmega8
CFLAGS=-mmcu=$(MCU) -Os
SRC=zeptix.c tasks.c algorithms/lottery.c

# Generate hex file for zeptix.
zeptix.hex: zeptix.o
	avr-objcopy -j .text -j .data -O ihex zeptix.o zeptix.hex

# Generate zeptix object file
zeptix.o: $(SRC)
	avr-gcc $(CFLAGS) $(SRC) -o zeptix.o

# Generate all object files from their respective sources.
%.o: %.c
	avr-gcc $< -o $*.o

# Load zeptix to atmega8 microcontroller.
load: zeptix.hex
	avrdude -c usbasp -p m8 -P usb -U flash:w:zeptix.hex

# Erase all object, hex and backup files
clean:
	rm -rf *.o *.hex *~ */*.o */*.hex */*~
