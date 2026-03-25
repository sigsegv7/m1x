#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

include mk/default.mk

.PHONY: all
all: sdk m1x boot

.PHONY: boot
boot:
	cd boot/; make

.PHONY: sdk
sdk:
	cd sdk/; make $(PASSDOWN_ARGS)

.PHONY: m1x
m1x:
	cd m1x/; make
