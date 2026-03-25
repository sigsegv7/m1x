#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

CFILES = $(shell find . -name "*.c")
OFILES = $(CFILES:.c=.o)
CC = ../$(SYS_CC)

CFLAGS = 					\
	$(SYS_CFLAGS) 			\
	-Iinclude/

.PHONY: all
all:  $(OFILES)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
