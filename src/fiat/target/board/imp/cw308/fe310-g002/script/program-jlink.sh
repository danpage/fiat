#!/bin/bash

# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

TRANSPORT="jtag"
CONFIG="${FIAT_HOME}/src/fiat/target/board/imp/cw308/fe310-g002/script/fe310-g002.cfg"

openocd --file    "interface/jlink.cfg"           \
        --command "transport select ${TRANSPORT}" \
        --file    "${CONFIG}"                     \
        --command "init"                          \
        --command "targets"                       \
        --command "halt"                          \
        --command "flash write_image erase ${1}"  \
        --command "reset run"                     \
        --command "shutdown"
