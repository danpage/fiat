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

.PHONY: build

build        :
	@hatch build   --clean     ${REPO_HOME}/build/client

publish      :
	@hatch publish --repo main ${REPO_HOME}/build/client
publish-test :
	@hatch publish --repo test ${REPO_HOME}/build/client

clean        :
	@hatch clean

spotless     :
	@rm --force --recursive ${REPO_HOME}/build/client

# =============================================================================
