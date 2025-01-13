# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

ifndef REPO_HOME
  $(error "execute 'source ./bin/conf.sh' to configure environment")
endif
ifndef REPO_VERSION
  $(error "execute 'source ./bin/conf.sh' to configure environment")
endif

# =============================================================================

export CONTEXT      ?= native

export DRIVER       ?= binary
export BOARD        ?= native
export CONF         ?=

export DEPS         ?= ${REPO_HOME}/build/deps/${BOARD}

# -----------------------------------------------------------------------------

export DOCKER_REPO  ?= danpage/fiat.$(subst /,-,${BOARD})
export DOCKER_TAG   ?= ${REPO_VERSION}
export DOCKER_FLAGS ?= 

export DOCKER_FLAGS += --env DOCKER_GID="$(shell id --group)" 
export DOCKER_FLAGS += --env DOCKER_UID="$(shell id --user)" 

export DOCKER_FLAGS += --env    CONTEXT="native" 

export DOCKER_FLAGS += --env     DRIVER="${DRIVER}" 
export DOCKER_FLAGS += --env      BOARD="${BOARD}" 
export DOCKER_FLAGS += --env       CONF="${CONF}" 

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

ifeq "${CONTEXT}" "docker"
%        :
	@docker run --rm --volume "${REPO_HOME}:/mnt/fiat" ${DOCKER_FLAGS} ${DOCKER_REPO}:${DOCKER_TAG} ${*}
endif

# -----------------------------------------------------------------------------

ifeq "${CONTEXT}" "native"
   doc/%     :
	@make --directory="${REPO_HOME}/doc"             ${*}
docker/%     :
	@make --directory="${REPO_HOME}/src/docker"      ${*}
client/%     :
	@make --directory="${REPO_HOME}/src/fiat/client" ${*}
target/%     :
	@make --directory="${REPO_HOME}/src/fiat/target" ${*}

  venv/build : ${REPO_HOME}/requirements.txt
	@${REPO_HOME}/bin/venv.sh ${<}
  venv/clean : ${REPO_HOME}/requirements.txt
	@rm --force --recursive ${REPO_HOME}/build/venv

clean        : 

spotless     : clean
	@rm --force --recursive ${REPO_HOME}/build/*
endif

# =============================================================================
