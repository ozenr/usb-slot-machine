# ARM Cross Compiler Flags
CC=arm-none-eabi-gcc
CFLAGS=-mcpu=cortex-m4 -mthumb  # compiler flags
CPPFLAGS=-DSTM32F401xE -Ivendor/CMSIS/CMSIS/Core/Include -Ivendor/ST/STM32F4/Include -Idrivers/inc # pre-processor
LINKER_SCRIPT=linker_script.ld
LDFLAGS=-T $(LINKER_SCRIPT) --specs=nano.specs --specs=nosys.specs

# Default Target
EXEC=app.elf
all: $(EXEC)

app.elf: main.o startup.o system_stm32f4xx.o syscalls.o itm.o spi.o st7735s.o
	$(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $(EXEC)

syscalls.o: syscalls.c
	$(CC) $(CFLAGS) $(CPPFLAGS) syscalls.c -c 

main.o: main.c
	$(CC) $(CFLAGS) $(CPPFLAGS) main.c -c

startup.o: startup.c
	$(CC) $(CFLAGS) $(CPPFLAGS) startup.c -c

system_stm32f4xx.o: vendor/ST/STM32F4/Source/Templates/system_stm32f4xx.c
	$(CC) $(CFLAGS) $(CPPFLAGS) vendor/ST/STM32F4/Source/Templates/system_stm32f4xx.c -c

itm.o: drivers/src/itm.c
	$(CC) $(CFLAGS) $(CPPFLAGS) drivers/src/itm.c -c

spi.o: drivers/src/spi.c
	$(CC) $(CFLAGS) $(CPPFLAGS) drivers/src/spi.c -c

st7735s.o: drivers/src/st7735s.c
	$(CC) $(CFLAGS) $(CPPFLAGS) drivers/src/st7735s.c -c

# Flash/Programmer Flags
PROGRAMMER=openocd
PFLAGS=-f interface/stlink.cfg -f target/stm32f4x.cfg

flash: $(EXEC)
	$(PROGRAMMER) $(PFLAGS) -c "program $(EXEC) verify reset exit"

trace: $(EXEC)
	$(PROGRAMMER) $(PFLAGS) -c "init" -c "reset halt" -c "stm32f4x.tpiu configure -protocol uart -output /dev/stdout -formatter off -traceclk 84000000" -c "stm32f4x.tpiu enable" -c "itm port 0 on" -c resume

# Nice Shortcut Commands
.PHONY: clean
clean:
	rm -rf *.o *.elf

.PHONY: objdump
objdump:
	arm-none-eabi-objdump -h app.elf > objdump.txt

.PHONY: size
size:
	arm-none-eabi-size -Wl --print-memory-usage app.elf 
