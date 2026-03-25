#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

include ../mk/default.mk

.PHONY: all
all: kern arch

.PHONY: kern
kern:
	cd kern/; make $(PASSDOWN_ARGS) ARCH_TARGET=$(ARCH_TARGET)

.PHONY: arch
arch:
	cd arch/$(ARCH_TARGET)/; make $(PASSDOWN_ARGS)
