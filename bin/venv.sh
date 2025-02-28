#!/bin/bash

# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

if [ -z "${FIAT_PATH_REPO}" ] ; then
  echo "FIAT_PATH_REPO"     " is undefined: you may want to execute 'source ./bin/conf.sh' to configure environment" ; exit
fi

# -----------------------------------------------------------------------------

python3 -m venv --clear ${FIAT_PATH_REPO}/build/venv && source ${FIAT_PATH_REPO}/build/venv/bin/activate

if [ -z "${1}" ] ; then 
  python3 -m pip install --upgrade pip
  python3 -m pip install -r ${FIAT_PATH_REPO}/requirements.txt
else
  python3 -m pip install --upgrade pip
  python3 -m pip install -r ${1}
fi

# =============================================================================
