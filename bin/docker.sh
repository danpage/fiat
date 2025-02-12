#!/bin/bash

# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

if [ -z "${FIAT_HOME}" ] ; then
  echo "execute 'source ./bin/conf.sh' to configure environment" ; exit
fi

# -----------------------------------------------------------------------------

FIAT_BOARDS=$(find ${FIAT_HOME}/src/fiat/target/board/imp -type f -name Makefile.in | sed -e "s|${FIAT_HOME}/src/fiat/target/board/imp/\(.*\)/Makefile.in|\1|")

for FIAT_BOARD in ${FIAT_BOARDS} ; do
  FIAT_CONTEXT="native" FIAT_BOARD="${FIAT_BOARD}" make -f ${FIAT_HOME}/Makefile docker/${1}
done

# =============================================================================
