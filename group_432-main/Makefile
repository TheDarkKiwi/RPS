# File:   Makefile
# Author: Jack Edwards, Conor McNeill
# Descr:  Makefile for Rock Paper Scissors

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I. -Idrivers -Ifonts -Idrivers/avr -Iutils
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm

# Default target.
all: mainGame.out


# Compile: create object files from C source files.
mainGame.o: mainGame.c drivers/avr/system.h drivers/avr/pio.h utils/pacer.h drivers/navswitch.h utils/tinygl.h utils/font.h drivers/display.h drivers/avr/ir_uart.h mainGame.h
	$(CC) -c $(CFLAGS) $< -o $@


pio.o: drivers/avr/pio.c drivers/avr/pio.h drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: drivers/avr/system.c drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: drivers/avr/timer.c drivers/avr/system.h drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: utils/pacer.c drivers/avr/system.h drivers/avr/timer.h utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: drivers/navswitch.c drivers/avr/delay.h drivers/avr/pio.h drivers/avr/system.h drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: drivers/ledmat.c drivers/avr/pio.h drivers/avr/system.h drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: utils/tinygl.c drivers/avr/system.h drivers/display.h utils/font.h utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: utils/font.c drivers/avr/system.h utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@
 
display.o: drivers/display.c drivers/avr/system.h drivers/display.h drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: drivers/avr/ir_uart.c drivers/avr/ir_uart.h drivers/avr/pio.h drivers/avr/system.h drivers/avr/timer0.h drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: drivers/avr/timer0.c drivers/avr/bits.h drivers/avr/prescale.h drivers/avr/system.h drivers/avr/timer0.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: drivers/avr/usart1.c drivers/avr/system.h drivers/avr/usart1.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: drivers/avr/prescale.c drivers/avr/prescale.h drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

led.o: drivers/led.c drivers/avr/system.h drivers/avr/pio.h drivers/led.h
	$(CC) -c $(CFLAGS) $< -o $@

# Link: create output file (executable) from object files.
mainGame.out: mainGame.o pio.o system.o timer.o pacer.o navswitch.o font.o tinygl.o ledmat.o display.o ir_uart.o timer0.o usart1.o prescale.o led.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Create hex file for programming from executable file.
mainGame.hex: mainGame.out
	$(OBJCOPY) -O ihex mainGame.out mainGame.hex


# Target: clean project.
.PHONY: clean
clean:
	-$(DEL) *.o *.out *.hex

 
# Target: program project.
.PHONY: program
program: mainGame.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash mainGame.hex; dfu-programmer atmega32u2 start


