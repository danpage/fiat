# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

ifndef FIAT_HOME
  $(error "FIAT_HOME"          " is undefined you may want to execute 'source ./bin/conf.sh' to configure environment")
endif

ifndef FIAT_VERSION_MAJOR
  $(error "FIAT_VERSION_MAJOR" " is undefined you may want to execute 'source ./bin/conf.sh' to configure environment")
endif
ifndef FIAT_VERSION_MINOR
  $(error "FIAT_VERSION_MINOR" " is undefined you may want to execute 'source ./bin/conf.sh' to configure environment")
endif
ifndef FIAT_VERSION_PATCH
  $(error "FIAT_VERSION_PATCH" " is undefined you may want to execute 'source ./bin/conf.sh' to configure environment")
endif
ifndef FIAT_VERSION
  $(error "FIAT_VERSION"       " is undefined you may want to execute 'source ./bin/conf.sh' to configure environment")
endif

# =============================================================================

export FIAT_CONTEXT ?= native
export FIAT_DRIVER  ?= binary
export FIAT_BOARD   ?= native

export FIAT_KERNEL  ?= ${FIAT_HOME}/src/fiat/target/kernel/imp
export FIAT_BUILD   ?= ${FIAT_HOME}/build

export FIAT_DEPS    ?= ${FIAT_BUILD/deps/${FIAT_BOARD}

# -----------------------------------------------------------------------------

export DOCKER_IMAGE ?= danpage/fiat.$(subst /,-,${FIAT_BOARD})
export DOCKER_TAG   ?= ${FIAT_VERSION}
export DOCKER_FLAGS ?= 
export DOCKER_ENV   ?= 

export DOCKER_FLAGS += --volume "${FIAT_HOME}:/mnt/fiat_home" 
export DOCKER_FLAGS += --volume "${FIAT_KERNEL}:/mnt/fiat_kernel" 
export DOCKER_FLAGS += --volume "${FIAT_BUILD}:/mnt/fiat_build"

export DOCKER_ENV   += --env DOCKER_GID="$(shell id --group)" 
export DOCKER_ENV   += --env DOCKER_UID="$(shell id --user )" 

export DOCKER_ENV   += --env FIAT_CONTEXT="native"
export DOCKER_ENV   += --env FIAT_DRIVER="${FIAT_DRIVER}"
export DOCKER_ENV   += --env FIAT_BOARD="${FIAT_BOARD}" 

export DOCKER_ENV   += --env FIAT_HOME="/mnt/fiat_home"
export DOCKER_ENV   += --env FIAT_KERNEL="/mnt/fiat_kernel"
export DOCKER_ENV   += --env FIAT_BUILD="/mnt/fiat_build"

export DOCKER_ENV   += --env GCC_FLAGS
export DOCKER_ENV   += --env GCC_PATHS
export DOCKER_ENV   += --env GCC_LIBS
export DOCKER_ENV   += --env GCC_DEFS

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
	@docker run --rm ${DOCKER_FLAGS} ${DOCKER_ENV} ${DOCKER_IMAGE}:${DOCKER_TAG} ${*}
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
