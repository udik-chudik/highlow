.SILENT:clean
all: clean compile main.hex
compile: main.c
	avr-gcc *.c -mmcu=atmega16 -Wall -gdwarf-2 -std=gnu99 -Wl,-u,vfprintf -lprintf_flt  -DF_CPU=8000000UL -Os  -lm -lc  -o main.elf
	avr-size -C --mcu=atmega16 main.elf
main.hex: main.elf
	avr-objcopy -O ihex main.elf main.hex
clean:
	-rm main.elf main.hex
