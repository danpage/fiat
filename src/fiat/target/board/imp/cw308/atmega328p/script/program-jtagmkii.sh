#!/bin/bash

# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

PROGRAMMER="jtag2isp"
PART="atmega328p"

avrdude -P usb -c ${PROGRAMMER} -p ${PART} -U "flash:w:${1}"
