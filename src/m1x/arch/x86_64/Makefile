#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

MISC_OFILES = $(shell find ../../ -name "*.o")
SDK_OFILES = $(shell find ../../../sdk/ -name "*.o")

ASMFILES = $(shell find . -name "*.S" | grep -v "boot")
ASMOFILES = $(ASMFILES:.S=.S.o)
CFILES = $(shell find . -name "*.c")
OFILES = $(CFILES:.c=.o)
DFILES = $(shell find . -name "*.d")

KERN_BIN = ../../m1x.sys

LD = ../../../$(SYS_LD)
CC = ../../../$(SYS_CC)

CFLAGS = 				\
	$(SYS_CFLAGS)		\
	-D_KERNEL			\
	-I../../target/		\
	-I../../include/	\
	-I../../../sdk/include/

.PHONY: all
all: $(OFILES) $(ASMOFILES)
	$(LD) $(MISC_OFILES) $(SDK_OFILES) -Tconf/link.ld -o $(KERN_BIN) $(LDFLAGS)

%.S.o: %.S
	$(CC) -c $< $(CFLAGS) -o $@

-include $(DFILES)
%.o: %.c
	$(CC) -c $< $(CFLAGS) -I../../inc/ -MMD -o $@
