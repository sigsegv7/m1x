ARCH_TARGET = x86_64

CC = \
	../var/cc/toolchain/gcc/bin/$(ARCH_TARGET)-pc-mirocom-gcc
LD = \
    ../var/cc/toolchain/build-binutils/bin/$(ARCH_TARGET)-pc-mirocom-ld
AR = \
	 ../var/cc/toolchain/build-binutils/bin/$(ARCH_TARGET)-pc-mirocom-ar

SYS_CFLAGS =			\
	-nostdlib			\
	-nostdinc			\
	-ffreestanding		\
	-fexceptions		\
	--std=gnu11			\
	-mcmodel=kernel		\
	-Wno-attributes		\
	-fno-stack-protector\
	-D_M1X_MULTICORE

ifeq ($(ARCH_TARGET),x86_64)
	SYS_CFLAGS += 		\
		-mno-sse		\
		-mno-sse2		\
		-mno-sse3		\
		-mno-avx		\
		-mno-avx2		\
		-mno-80387		\
		-mno-3dnow		\
		-mno-mmx
endif

PASSDOWN_ARGS = \
	ARCH=$(ARCH_TARGET)		\
	SYS_CC=$(CC)			\
	SYS_LD=$(LD)			\
	SYS_CFLAGS="$(SYS_CFLAGS)"
