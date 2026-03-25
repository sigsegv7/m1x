#!/bin/sh

#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

build() {
    cd src/; make -j1
}

build
