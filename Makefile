# ARM Cross Compiler Flags
CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m4 -mthumb -nostdlib # compiler flags
CPPFLAGS=-DSTM32F401xE -Ivendor/CMSIS/CMSIS/Core/Include -Ivendor/ST/STM32F4/Include # pre-processor
LINKER_SCRIPT=linker_script.ld
LDFLAGS=-T $(LINKER_SCRIPT)

# Default Target
all: blinky.elf

blinky.elf: main.o startup.o system_stm32f4xx.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) main.o startup.o system_stm32f4xx.o -o blinky.elf

main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) main.c -c

startup.o: startup.c
	$(CC) $(CFLAGS) $(CPPFLAGS) startup.c -c

system_stm32f4xx.o: vendor/ST/STM32F4/Source/Templates/system_stm32f4xx.c
	$(CC) $(CFLAGS) $(CPPFLAGS) vendor/ST/STM32F4/Source/Templates/system_stm32f4xx.c -c

# Flash/Programmer Flags
PROGRAMMER=openocd
PFLAGS=-f interface/stlink.cfg -f target/stm32f4x.cfg

flash: blinky.elf
	$(PROGRAMMER) $(PFLAGS) -c "program blinky.elf verify reset exit"
	rm -f *.o
