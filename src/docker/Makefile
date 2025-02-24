# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

export DOCKER_FLAGS    += 

export DOCKER_DEFS     += --build-arg DATE="$(shell date -u +'%d/%m/%Y-%H:%M:%SZ')" 

export DOCKER_DEFS     += --build-arg FIAT_CONTEXT="native"
export DOCKER_DEFS     += --build-arg FIAT_DRIVER="${FIAT_DRIVER}"
export DOCKER_DEFS     += --build-arg FIAT_BOARD="${FIAT_BOARD}"

export DOCKER_DEFS     += --build-arg FIAT_PATH_REPO="/mnt/fiat_home"
export DOCKER_DEFS     += --build-arg FIAT_PATH_IMP="/mnt/fiat_imp"
export DOCKER_DEFS     += --build-arg FIAT_PATH_BUILD="/mnt/fiat_build"
export DOCKER_DEFS     += --build-arg FIAT_PATH_DEP="/opt/fiat_dep/${FIAT_BOARD}"

# =============================================================================

${FIAT_PATH_BUILD}/docker/${FIAT_BOARD}/Makefile      : ${FIAT_PATH_REPO}/src/fiat/target/board/imp/${FIAT_BOARD}/Makefile.in
	@m4 --include="${FIAT_PATH_REPO}/src/docker" --include="${FIAT_PATH_REPO}/src/fiat/target/board/imp/${FIAT_BOARD}" ${<} > ${@}
${FIAT_PATH_BUILD}/docker/${FIAT_BOARD}/Dockerfile    : ${FIAT_PATH_REPO}/src/docker/Dockerfile
	@m4 --include="${FIAT_PATH_REPO}/src/docker" --include="${FIAT_PATH_REPO}/src/fiat/target/board/imp/${FIAT_BOARD}" ${<} > ${@}
${FIAT_PATH_BUILD}/docker/${FIAT_BOARD}/entrypoint.sh : ${FIAT_PATH_REPO}/src/docker/entrypoint.sh
	@m4 --include="${FIAT_PATH_REPO}/src/docker" --include="${FIAT_PATH_REPO}/src/fiat/target/board/imp/${FIAT_BOARD}" ${<} > ${@}

# -----------------------------------------------------------------------------

BUILD_FILES += ${FIAT_PATH_BUILD}/docker/${FIAT_BOARD}/Makefile
BUILD_FILES += ${FIAT_PATH_BUILD}/docker/${FIAT_BOARD}/Dockerfile
BUILD_FILES += ${FIAT_PATH_BUILD}/docker/${FIAT_BOARD}/entrypoint.sh

BUILD_PATHS += ${FIAT_PATH_BUILD}/docker/${FIAT_BOARD}

${BUILD_PATHS} :
	@mkdir --parents ${@}

# -----------------------------------------------------------------------------

build :                         ${BUILD_PATHS} ${BUILD_FILES}
	@docker build --tag ${DOCKER_IMAGE}:${DOCKER_TAG} ${DOCKER_FLAGS} ${DOCKER_DEFS} ${FIAT_PATH_BUILD}/docker/${FIAT_BOARD}

clean :
	@rm --force --recursive ${BUILD_PATHS}

push  :
	@docker push ${DOCKER_IMAGE}:${DOCKER_TAG}

pull  :
	@docker pull ${DOCKER_IMAGE}:${DOCKER_TAG}

# =============================================================================
