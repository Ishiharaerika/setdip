PLUGIN_OBJS = kernel.o
HEADERS = $(wildcard *.h)

PLUGIN_LIBS = -lSceKblForKernel_365_stub

PREFIX  = arm-vita-eabi
CC      = $(PREFIX)-gcc
CFLAGS  = -Wl,-q -Wall -O3
ASFLAGS = $(CFLAGS)

all: diphwbkpt.skprx

diphwbkpt.skprx: diphwbkpt.velf
	vita-make-fself -c $< $@

diphwbkpt.velf: diphwbkpt.elf
	vita-elf-create -n $< $@

diphwbkpt.elf: $(PLUGIN_OBJS)
	$(CC) $(CFLAGS) $^ $(PLUGIN_LIBS) -o $@ -nostdlib

clean:
	rm -f $(PLUGIN_OBJS) diphwbkpt.elf diphwbkpt.velf diphwbkpt.skprx