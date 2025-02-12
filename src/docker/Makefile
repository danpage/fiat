# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

${FIAT_BUILD}/docker/${FIAT_BOARD}/Makefile      : ${FIAT_HOME}/src/fiat/target/board/imp/${FIAT_BOARD}/Makefile.in
	@m4 --include="${FIAT_HOME}/src/docker" --include="${FIAT_HOME}/src/fiat/target/board/imp/${FIAT_BOARD}" ${<} > ${@}
${FIAT_BUILD}/docker/${FIAT_BOARD}/Dockerfile    : ${FIAT_HOME}/src/docker/Dockerfile
	@m4 --include="${FIAT_HOME}/src/docker" --include="${FIAT_HOME}/src/fiat/target/board/imp/${FIAT_BOARD}" ${<} > ${@}
${FIAT_BUILD}/docker/${FIAT_BOARD}/entrypoint.sh : ${FIAT_HOME}/src/docker/entrypoint.sh
	@m4 --include="${FIAT_HOME}/src/docker" --include="${FIAT_HOME}/src/fiat/target/board/imp/${FIAT_BOARD}" ${<} > ${@}

# =============================================================================

BUILD_FILES  = ${FIAT_BUILD}/docker/${FIAT_BOARD}/Makefile
BUILD_FILES += ${FIAT_BUILD}/docker/${FIAT_BOARD}/Dockerfile
BUILD_FILES += ${FIAT_BUILD}/docker/${FIAT_BOARD}/entrypoint.sh

BUILD_PATHS  = ${FIAT_BUILD}/docker/${FIAT_BOARD}

${BUILD_PATHS} :
	@mkdir --parents ${@}

${CLEAN_PATHS} :
	@rm --force --recursive ${@}

# -----------------------------------------------------------------------------

build : ${BUILD_PATHS} ${BUILD_FILES}
	@docker build --tag ${DOCKER_IMAGE}:${DOCKER_TAG} --build-arg DATE="$(shell date -u +'%d/%m/%Y-%H:%M:%SZ')" --build-arg FIAT_BOARD="${FIAT_BOARD}" ${FIAT_BUILD}/docker/${FIAT_BOARD}

clean : ${CLEAN_PATHS}
	@docker images | grep ${DOCKER_IMAGE} | grep ${DOCKER_TAG} | tr --squeeze-repeats ' ' | cut --delimiter=' ' --fields=3 | uniq | xargs --no-run-if-empty docker rmi --force && rm --force --recursive ${BUILD_PATHS}

push  :
	@docker push ${DOCKER_IMAGE}:${DOCKER_TAG}

pull  :
	@docker pull ${DOCKER_IMAGE}:${DOCKER_TAG}

# =============================================================================
