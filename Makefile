# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

export FIAT_CONTEXT ?= native
export FIAT_DRIVER  ?= binary
export FIAT_BOARD   ?= native

export FIAT_KERNEL  ?= ${FIAT_HOME}/src/fiat/target/kernel/imp
export FIAT_BUILD   ?= ${FIAT_HOME}/build

export FIAT_DEPS    ?= ${FIAT_BUILD/deps/${FIAT_BOARD}

# -----------------------------------------------------------------------------

export GCC_FLAGS    ?=
export GCC_PATHS    ?=
export GCC_LIBS     ?=

# -----------------------------------------------------------------------------

export DOCKER_IMAGE ?= danpage/fiat.$(subst /,-,${FIAT_BOARD})
export DOCKER_TAG   ?= ${FIAT_VERSION}
export DOCKER_FLAGS ?= 

export DOCKER_FLAGS += --volume "${FIAT_HOME}:/mnt/fiat_home" 
export DOCKER_FLAGS += --volume "${FIAT_KERNEL}:/mnt/fiat_kernel" 
export DOCKER_FLAGS += --volume "${FIAT_BUILD}:/mnt/fiat_build"

export DOCKER_FLAGS += --env DOCKER_GID="$(shell id --group)" 
export DOCKER_FLAGS += --env DOCKER_UID="$(shell id --user )" 

export DOCKER_FLAGS += --env FIAT_CONTEXT="native"
export DOCKER_FLAGS += --env FIAT_DRIVER="${FIAT_DRIVER}"
export DOCKER_FLAGS += --env FIAT_BOARD="${FIAT_BOARD}" 

export DOCKER_FLAGS += --env FIAT_HOME="/mnt/fiat_home"
export DOCKER_FLAGS += --env FIAT_KERNEL="/mnt/fiat_kernel"
export DOCKER_FLAGS += --env FIAT_BUILD="/mnt/fiat_build"

export DOCKER_FLAGS += --env GCC_FLAGS="${GCC_FLAGS}"
export DOCKER_FLAGS += --env GCC_PATHS="${GCC_PATHS}"
export DOCKER_FLAGS += --env GCC_LIBS="${GCC_LIBS}"

# =============================================================================

# Define targets to drive build process, depending on CONTEXT (i.e., on the 
# selected build context):
#
# 1. For the Docker build context, defer everything to the Docker container 
#    (i.e., execute make on the same target *within* the container).
#
# 2. For the native build context, 
#
#    - deal with various specific, global targets (e.g., documentation), or
#    - defer to the appropriate sub-Makefile for everything else.

# -----------------------------------------------------------------------------

ifeq "${FIAT_CONTEXT}" "docker"
% :
	@docker run --rm ${DOCKER_FLAGS} ${DOCKER_IMAGE}:${DOCKER_TAG} ${*}
endif

# -----------------------------------------------------------------------------

ifeq "${FIAT_CONTEXT}" "native"
   doc/%     :
	@make --directory="${FIAT_HOME}/doc"             ${*}
docker/%     :
	@make --directory="${FIAT_HOME}/src/docker"      ${*}
client/%     :
	@make --directory="${FIAT_HOME}/src/fiat/client" ${*}
target/%     :
	@make --directory="${FIAT_HOME}/src/fiat/target" ${*}

  venv/build : ${FIAT_HOME}/requirements.txt
	@${FIAT_HOME}/bin/venv.sh ${<}
  venv/clean : ${FIAT_HOME}/requirements.txt
	@rm --force --recursive ${FIAT_BUILD}/venv

clean        : 

spotless     : clean
	@rm --force --recursive ${FIAT_BUILD}/*
endif

# =============================================================================
