#!/bin/bash

#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#


set -e

ISO=../../m1x.iso
CONFIG=../data/boot/limine.conf
KERNEL=../m1x/m1x.sys

mkdir -p iso_root/boot/

# Copy the kernel to the ISO root
cp $KERNEL iso_root/boot

# Copy boot files
cp $CONFIG stand/limine/limine-bios.sys \
    stand/limine/limine-bios-cd.bin \
    stand/limine/limine-uefi-cd.bin \
    iso_root/

# Generate the ISO
xorriso -as mkisofs -b limine-bios-cd.bin -no-emul-boot -boot-load-size 4 \
        -boot-info-table --efi-boot limine-uefi-cd.bin -efi-boot-part \
		--efi-boot-image --protective-msdos-label iso_root/ -o $ISO 1>/dev/null

stand/limine/limine bios-install $ISO 1>/dev/null
rm -rf iso_root
