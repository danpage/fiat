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

${REPO_HOME}/build/docker/${BOARD}/Makefile      : ${REPO_HOME}/src/fiat/target/board/imp/${BOARD}/Makefile.in
	m4 --include="${REPO_HOME}/src/docker" --include="${REPO_HOME}/src/fiat/target/board/imp/${BOARD}" ${<} > ${@}
${REPO_HOME}/build/docker/${BOARD}/Dockerfile    : ${REPO_HOME}/src/docker/Dockerfile
	m4 --include="${REPO_HOME}/src/docker" --include="${REPO_HOME}/src/fiat/target/board/imp/${BOARD}" ${<} > ${@}
${REPO_HOME}/build/docker/${BOARD}/entrypoint.sh : ${REPO_HOME}/src/docker/entrypoint.sh
	m4 --include="${REPO_HOME}/src/docker" --include="${REPO_HOME}/src/fiat/target/board/imp/${BOARD}" ${<} > ${@}

BUILD_FILES  = ${REPO_HOME}/build/docker/${BOARD}/Makefile
BUILD_FILES += ${REPO_HOME}/build/docker/${BOARD}/Dockerfile
BUILD_FILES += ${REPO_HOME}/build/docker/${BOARD}/entrypoint.sh

BUILD_PATHS  = ${REPO_HOME}/build/docker/${BOARD}

${BUILD_PATHS} :
	@mkdir --parents ${@}

${CLEAN_PATHS} :
	@rm --force --recursive ${@}

# =============================================================================

build : ${BUILD_PATHS} ${BUILD_FILES}
	@docker build --tag ${DOCKER_REPO}:${DOCKER_TAG} --build-arg DATE="$(shell date -u +'%d/%m/%Y-%H:%M:%SZ')" --build-arg BOARD="${BOARD}" ${REPO_HOME}/build/docker/${BOARD}

clean : ${CLEAN_PATHS}
	@docker images | grep ${DOCKER_REPO} | grep ${DOCKER_TAG} | tr --squeeze-repeats ' ' | cut --delimiter=' ' --fields=3 | uniq | xargs --no-run-if-empty docker rmi --force && rm --force --recursive ${BUILD_PATHS}

push  :
	@docker push ${DOCKER_REPO}:${DOCKER_TAG}

pull  :
	@docker pull ${DOCKER_REPO}:${DOCKER_TAG}

# =============================================================================
