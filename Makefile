# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

ifndef FIAT_PATH_REPO
  $(error "FIAT_PATH_REPO"     " is undefined: you may want to execute 'source ./bin/conf.sh' to configure environment")
endif

ifndef FIAT_VERSION_MAJOR
  $(error "FIAT_VERSION_MAJOR" " is undefined: you may want to execute 'source ./bin/conf.sh' to configure environment")
endif
ifndef FIAT_VERSION_MINOR
  $(error "FIAT_VERSION_MINOR" " is undefined: you may want to execute 'source ./bin/conf.sh' to configure environment")
endif
ifndef FIAT_VERSION_PATCH
  $(error "FIAT_VERSION_PATCH" " is undefined: you may want to execute 'source ./bin/conf.sh' to configure environment")
endif

# =============================================================================

export FIAT_CONTEXT       ?= native
export FIAT_DRIVER        ?= binary
export FIAT_BOARD         ?= native

export FIAT_PATH_REPO     ?= ${CURDIR}
export FIAT_PATH_IMP      ?= ${FIAT_PATH_REPO}/src/fiat/target/kernel/imp/
export FIAT_PATH_BUILD    ?= ${FIAT_PATH_REPO}/build
export FIAT_PATH_DEP      ?= ${FIAT_PATH_REPO}/build/deps/${FIAT_BOARD}

export FIAT_VERSION_MAJOR ?= 0
export FIAT_VERSION_MINOR ?= 0
export FIAT_VERSION_PATCH ?= 0

# -----------------------------------------------------------------------------

export DOCKER_IMAGE       ?= danpage/fiat.$(subst /,-,${FIAT_BOARD})
export DOCKER_TAG         ?= ${FIAT_VERSION_MAJOR}.${FIAT_VERSION_MINOR}.${FIAT_VERSION_PATCH}

# =============================================================================

# Define targets to drive build process, depending on FIAT_CONTEXT (i.e., on
# the selected build context):
#
# 1. For the Docker build context, defer everything to the Docker container 
#    (i.e., execute make on the same target *within* the container).
#
# 2. For the native build context, 
#
#    - deal with any specific, global targets (e.g., virtual environment),
#    - defer to the appropriate sub-Makefile for everything else.

ifeq "${FIAT_CONTEXT}" "docker"
include ${FIAT_PATH_REPO}/Makefile.context_docker
endif
ifeq "${FIAT_CONTEXT}" "native"
include ${FIAT_PATH_REPO}/Makefile.context_native
endif

# =============================================================================
