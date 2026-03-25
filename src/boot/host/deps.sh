#!/bin/sh

#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

if [ ! -d stand/limine ]; then
	git clone https://codeberg.org/Limine/Limine.git --branch=v10.x-binary --depth=1 stand/limine
    make -C stand/limine
fi
