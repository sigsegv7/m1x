#
# Copyright (c) 2026, Ian Moffett.
# Provided under the BSD-3 clause.
#

CFILES = $(shell find . -name "*.c")
CFILES += $(shell find ../foreign/ -name "*.c")
CFILES += $(shell find ../lib/ -name "*.c")
CFILES += $(shell find ../dev/ -name "*.c")
DFILES = $(CFILES:.c=.d)
OFILES = $(CFILES:.c=.o)

CC = ../../$(SYS_CC)
CFLAGS = 					\
	$(SYS_CFLAGS) 			\
	-D_KERNEL				\
	-MMD					\
	-I../../sdk/include/	\
	-I../include/			\
	-I../foreign/flanterm/src	\
	-DPRINTF_DISABLE_SUPPORT_PTRDIFF_T \
	-DPRINTF_DISABLE_SUPPORT_FLOAT		\
	-D_M1X_ARCH="\"$(ARCH_TARGET)\""

.PHONY: all
all: $(OFILES)

-include $(DFILES)
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
