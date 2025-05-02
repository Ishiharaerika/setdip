PREFIX=mep-elf-
CC=$(PREFIX)gcc
LD=$(PREFIX)gcc
OBJCOPY=$(PREFIX)objcopy
XXD=xxd

CFLAGS=-fno-delete-null-pointer-checks -nostdlib -fno-optimize-sibling-calls -mc=tiny -Os -std=gnu99 -mel
LDFLAGS=-nodefaultlibs -nostdlib
OBJCOPYFLAGS=-O binary

SRC_C=payload.c
OBJ=$(SRC_C:.c=.o)

TARGET_ELF=payload.elf
TARGET_BIN=payload

.PHONY: all clean

all: payload.h

payload.h: $(TARGET_BIN)
	$(XXD) -i $< > $@
	rm -f $(OBJ) $(TARGET_ELF) $(TARGET_BIN) *.ao

$(TARGET_BIN): $(TARGET_ELF)
	$(OBJCOPY) $(OBJCOPYFLAGS) $< $@

$(TARGET_ELF): $(OBJ)
	$(LD) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

%.ao: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ) $(TARGET_ELF) $(TARGET_BIN) payload.h *.ao