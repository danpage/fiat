#!/bin/bash

# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

if [ -z "${DOCKER_USER}" ] ; then
  DOCKER_USER="fiat"
fi 
if [ -z "${DOCKER_UID}"  ] ; then
  DOCKER_UID="1000"
fi 
if [ -z "${DOCKER_GID}"  ] ; then
  DOCKER_GID="1000"
fi 

groupadd --force --gid ${DOCKER_GID} ${DOCKER_USER} ; useradd --gid ${DOCKER_GID} --uid ${DOCKER_UID} --non-unique --no-user-group --create-home --shell /bin/bash ${DOCKER_USER}

# -----------------------------------------------------------------------------

cd /mnt/fiat_home ; source ./bin/conf.sh ; exec /usr/sbin/gosu ${DOCKER_USER} bash --login -c "make ${*}"

# =============================================================================
