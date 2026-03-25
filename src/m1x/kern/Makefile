#
# Copyright (c) 2026, Ian Moffett.
# Provided under the BSD-3 clause.
#

CFILES = $(shell find . -name "*.c")
DFILES = $(CFILES:.c=.d)
OFILES = $(CFILES:.c=.o)

CC = ../../$(SYS_CC)
CFLAGS = 					\
	$(SYS_CFLAGS) 			\
	-D_KERNEL				\
	-MMD					\
	-I../../sdk/include/		\
	-DPRINTF_DISABLE_SUPPORT_PTRDIFF_T \
	-DPRINTF_DISABLE_SUPPORT_FLOAT

.PHONY: all
all: $(OFILES)

-include $(DFILES)
%.o: %.c
	$(CC) -c $(CFLAGS) $< -o $@
