#!/bin/sh

#
# Copyright (c) 2026, Mirocom Laboratories
# Provided under the BSD-3 clause
#

#
# Check if a list of programs are installed on the system
#
# <@>: List of programs to check
#
check_deps() {
    for dep in $@; do
        printf "Checking if $dep is installed... "
        which $dep &>/dev/null
        if [ $? -ne 0 ]; then
            echo "no"
            echo "Please install $dep!"
            exit 1
        fi

        echo "yes"
    done
}

#
# Obtain the Mirocom gcc toolchain
#
get_toolchain() {
    if [ ! -d var/cc/toolchain ]; then
        git clone https://git.mirocom.org/Mirocom/mirocom-toolchain --depth=1 var/cc/toolchain
        cd var/cc/toolchain
        tar -xzvf toolchain.tar.gz
        mv public/* .; rm -rf public/
        cd ../../../
    fi
}

#
# Obtain all deps
#
get_deps() {
    get_toolchain
}

# Make sure everything is installed
check_deps   \
    git     \
    make    \
    gcc     \
    xorriso

# Obtain all the deps we need
get_deps
