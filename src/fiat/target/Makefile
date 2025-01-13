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

define include 
  $(shell cpp -D"DECLARE_SPR(...)=" -D"DECLARE_GPR(...)=" -D"DECLARE_CMD(...)=" -D"INCLUDE(x)=x" -P -I ${REPO_HOME}/src/fiat/target/kernel/imp ${REPO_HOME}/src/fiat/target/kernel/kernel.conf | sed -e '/^[[:space:]]*$$/d' | envsubst)
endef

EMPTY      :=
SPACE      := ${EMPTY} ${EMPTY}
COLON      := :

# -----------------------------------------------------------------------------

INCLUDES   +=             ${REPO_HOME}/src/fiat/target/share
 HEADERS   += $(wildcard  ${REPO_HOME}/src/fiat/target/share/*.[h])
 SOURCES   += $(wildcard  ${REPO_HOME}/src/fiat/target/share/*.[csS])

INCLUDES   +=             ${REPO_HOME}/src/fiat/target/board
 HEADERS   += $(wildcard  ${REPO_HOME}/src/fiat/target/board/*.[h])
 SOURCES   += $(wildcard  ${REPO_HOME}/src/fiat/target/board/*.[csS])
INCLUDES   +=             ${REPO_HOME}/src/fiat/target/board/imp/${BOARD}
 HEADERS   += $(wildcard  ${REPO_HOME}/src/fiat/target/board/imp/${BOARD}/*.[h])
 SOURCES   += $(wildcard  ${REPO_HOME}/src/fiat/target/board/imp/${BOARD}/*.[csS])

INCLUDES   +=             ${REPO_HOME}/src/fiat/target/driver
 HEADERS   += $(wildcard  ${REPO_HOME}/src/fiat/target/driver/*.[h])
 SOURCES   += $(wildcard  ${REPO_HOME}/src/fiat/target/driver/*.[csS])
INCLUDES   +=             ${REPO_HOME}/src/fiat/target/driver/imp/${DRIVER}
 HEADERS   += $(wildcard  ${REPO_HOME}/src/fiat/target/driver/imp/${DRIVER}/*.[h])
 SOURCES   += $(wildcard  ${REPO_HOME}/src/fiat/target/driver/imp/${DRIVER}/*.[csS])

INCLUDES   +=             ${REPO_HOME}/src/fiat/target/kernel
 HEADERS   += $(wildcard  ${REPO_HOME}/src/fiat/target/kernel/*.[h])
 SOURCES   += $(wildcard  ${REPO_HOME}/src/fiat/target/kernel/*.[csS])

INCLUDES   += $(sort $(dir         $(call include)))
 HEADERS   += $(sort $(filter %.h, $(call include)))
 SOURCES   += $(sort $(filter %.c, $(call include)))
 SOURCES   += $(sort $(filter %.s, $(call include)))
 SOURCES   += $(sort $(filter %.S, $(call include)))

 OBJECTS   += $(addprefix ${REPO_HOME}/build/target/${BOARD}/, $(notdir $(patsubst %.c, %.o, $(filter %.c, ${SOURCES}))))
 OBJECTS   += $(addprefix ${REPO_HOME}/build/target/${BOARD}/, $(notdir $(patsubst %.s, %.o, $(filter %.s, ${SOURCES}))))
 OBJECTS   += $(addprefix ${REPO_HOME}/build/target/${BOARD}/, $(notdir $(patsubst %.S, %.o, $(filter %.S, ${SOURCES}))))

GCC_PATHS  +=
GCC_FLAGS  += -DVERSION="${REPO_VERSION}" -DBOARD='"${BOARD}"' -DDRIVER='"${DRIVER}"' ${CONF}
GCC_LIBS   +=

include ${REPO_HOME}/src/fiat/target/board/imp/${BOARD}/Makefile.in

vpath %.h $(subst ${SPACE},${COLON},${INCLUDES})
vpath %.c $(subst ${SPACE},${COLON},${INCLUDES})
vpath %.s $(subst ${SPACE},${COLON},${INCLUDES})
vpath %.S $(subst ${SPACE},${COLON},${INCLUDES})

# =============================================================================

# Construct a definitive list of built files, plus their associated paths
# to allow pre-creation of an appropriate directory structure (note that
# use of sort is required to remove duplicates). 

BUILD_FILES    = ${OBJECTS}
BUILD_FILES   += ${TARGETS}

BUILD_PATHS    = $(sort $(foreach FILE,${BUILD_FILES},$(dir ${FILE})))

# Define targets for built files not otherwise catered for.

${BUILD_PATHS} :
	@mkdir --parents ${@}

${CLEAN_PATHS} :
	@rm --force --recursive ${@}

# Define targets to drive build process.

debug :
	@echo "INCLUDES = ${INCLUDES}"
	@echo "SOURCES  = ${SOURCES}"
	@echo "OBJECTS  = ${OBJECTS}"
	@echo "TARGETS  = ${TARGETS}"

script/% :
	@${REPO_HOME}/src/fiat/target/board/imp/${BOARD}/script/${*}

build    :                      ${BUILD_PATHS} ${BUILD_FILES}

clean    :
	@rm --force --recursive ${BUILD_PATHS}

# =============================================================================
