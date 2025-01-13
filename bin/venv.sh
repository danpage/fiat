#!/bin/bash

# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

if [ -z "${REPO_HOME}" ] ; then
  echo "execute 'source ./bin/conf.sh' to configure environment" ; exit
fi

# -----------------------------------------------------------------------------

python3 -m venv --clear ${REPO_HOME}/build/venv && source ${REPO_HOME}/build/venv/bin/activate

if [ -z "${1}" ] ; then 
  python3 -m pip install --upgrade pip
  python3 -m pip install -r ${REPO_HOME}/requirements.txt
else
  python3 -m pip install --upgrade pip
  python3 -m pip install -r ${1}
fi

# =============================================================================
