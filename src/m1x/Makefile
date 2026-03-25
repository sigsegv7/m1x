#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

include ../mk/default.mk

.PHONY: all
all: target kern arch

.PHONY: target
target:
	mkdir -p target/machine/
	rsync -avr include/arch/$(ARCH_TARGET)/*.h target/machine/

.PHONY: kern
kern:
	cd kern/; make $(PASSDOWN_ARGS) ARCH_TARGET=$(ARCH_TARGET)

.PHONY: arch
arch:
	cd arch/$(ARCH_TARGET)/; make $(PASSDOWN_ARGS)
