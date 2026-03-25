#!/bin/sh

#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

qemu-system-x86_64  \
    --enable-kvm    \
    -cpu host       \
    -m 2G           \
    -monitor stdio  \
    -cdrom m1x.iso
