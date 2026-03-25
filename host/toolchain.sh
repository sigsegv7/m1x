#!/bin/sh

#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

TARGET=x86_64-pc-mirocom
MAKE=make

mkdir -p var/
mkdir -p var/cc/root/usr/include/
mkdir -p var/cc/root/usr/bin/

# Don't build again if the lock exists
if [ -f var/cc/.lock ]; then
    echo "var/cc/.lock exists, skipping toolchain build"
    exit 1
fi

pushd var/cc/toolchain
bash build.sh

mkdir -p gcc
pushd gcc

# Configure gcc
../gcc-patched/configure --target=$TARGET \
    --prefix=$(pwd) --with-sysroot=$(pwd)/../../root/ \
    --disable-nls --enable-languages=c --disable-multilib

# Build gcc
$MAKE all-gcc
$MAKE install-gcc

# Lock the directory
popd
popd
touch var/cc/.lock
