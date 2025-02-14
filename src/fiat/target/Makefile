# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

define include 
  $(shell ${GCC_PREFIX}cpp -D"DECLARE_SPR(...)=" -D"DECLARE_GPR(...)=" -D"DECLARE_CMD(...)=" -D"INCLUDE(x)=x" ${GCC_FLAGS} -I ${FIAT_KERNEL} -P ${FIAT_HOME}/src/fiat/target/kernel/kernel.conf | sed -e '/^[[:space:]]*$$/d' | envsubst)
endef

EMPTY      :=
SPACE      := ${EMPTY} ${EMPTY}
COLON      := :

# -----------------------------------------------------------------------------

INCLUDES   +=             ${FIAT_HOME}/src/fiat/target/share
 HEADERS   += $(wildcard  ${FIAT_HOME}/src/fiat/target/share/*.[h])
 SOURCES   += $(wildcard  ${FIAT_HOME}/src/fiat/target/share/*.[csS])

INCLUDES   +=             ${FIAT_HOME}/src/fiat/target/board
 HEADERS   += $(wildcard  ${FIAT_HOME}/src/fiat/target/board/*.[h])
 SOURCES   += $(wildcard  ${FIAT_HOME}/src/fiat/target/board/*.[csS])
INCLUDES   +=             ${FIAT_HOME}/src/fiat/target/board/imp/${FIAT_BOARD}
 HEADERS   += $(wildcard  ${FIAT_HOME}/src/fiat/target/board/imp/${FIAT_BOARD}/*.[h])
 SOURCES   += $(wildcard  ${FIAT_HOME}/src/fiat/target/board/imp/${FIAT_BOARD}/*.[csS])

INCLUDES   +=             ${FIAT_HOME}/src/fiat/target/driver
 HEADERS   += $(wildcard  ${FIAT_HOME}/src/fiat/target/driver/*.[h])
 SOURCES   += $(wildcard  ${FIAT_HOME}/src/fiat/target/driver/*.[csS])
INCLUDES   +=             ${FIAT_HOME}/src/fiat/target/driver/imp/${FIAT_DRIVER}
 HEADERS   += $(wildcard  ${FIAT_HOME}/src/fiat/target/driver/imp/${FIAT_DRIVER}/*.[h])
 SOURCES   += $(wildcard  ${FIAT_HOME}/src/fiat/target/driver/imp/${FIAT_DRIVER}/*.[csS])

INCLUDES   +=             ${FIAT_HOME}/src/fiat/target/kernel
 HEADERS   += $(wildcard  ${FIAT_HOME}/src/fiat/target/kernel/*.[h])
 SOURCES   += $(wildcard  ${FIAT_HOME}/src/fiat/target/kernel/*.[csS])

INCLUDES   += $(sort $(dir         $(call include)))
 HEADERS   += $(sort $(filter %.h, $(call include)))
 SOURCES   += $(sort $(filter %.c, $(call include)))
 SOURCES   += $(sort $(filter %.s, $(call include)))
 SOURCES   += $(sort $(filter %.S, $(call include)))

 OBJECTS   += $(addprefix ${FIAT_BUILD}/target/${FIAT_BOARD}/, $(notdir $(patsubst %.c, %.o, $(filter %.c, ${SOURCES}))))
 OBJECTS   += $(addprefix ${FIAT_BUILD}/target/${FIAT_BOARD}/, $(notdir $(patsubst %.s, %.o, $(filter %.s, ${SOURCES}))))
 OBJECTS   += $(addprefix ${FIAT_BUILD}/target/${FIAT_BOARD}/, $(notdir $(patsubst %.S, %.o, $(filter %.S, ${SOURCES}))))

GCC_FLAGS  += -DFIAT_VERSION_MAJOR="${FIAT_VERSION_MAJOR}"
GCC_FLAGS  += -DFIAT_VERSION_MINOR="${FIAT_VERSION_MINOR}"
GCC_FLAGS  += -DFIAT_VERSION_PATCH="${FIAT_VERSION_PATCH}" -DFIAT_DRIVER='"${FIAT_DRIVER}"' -DFIAT_BOARD='"${FIAT_BOARD}"' 
GCC_PATHS  +=
GCC_LIBS   +=

include ${FIAT_HOME}/src/fiat/target/board/imp/${FIAT_BOARD}/Makefile.in

vpath %.h $(subst ${SPACE},${COLON},${INCLUDES})
vpath %.c $(subst ${SPACE},${COLON},${INCLUDES})
vpath %.s $(subst ${SPACE},${COLON},${INCLUDES})
vpath %.S $(subst ${SPACE},${COLON},${INCLUDES})

# =============================================================================

# Construct a definitive list of built files, plus their associated paths
# to allow pre-creation of an appropriate directory structure (note that
# use of sort is required to remove duplicates). 

BUILD_FILES  = ${OBJECTS}
BUILD_FILES += ${TARGETS}

BUILD_PATHS  = $(sort $(foreach FILE,${BUILD_FILES},$(dir ${FILE})))

# Define targets for built files not otherwise catered for.

${BUILD_PATHS} :
	@mkdir --parents ${@}

${CLEAN_PATHS} :
	@rm --force --recursive ${@}

# -----------------------------------------------------------------------------

# Define targets to drive build process.

debug    :
	@echo "INCLUDES   := ${INCLUDES}"
	@echo "SOURCES    := ${SOURCES}"
	@echo "HEADERS    := ${HEADERS}"
	@echo "OBJECTS    := ${OBJECTS}"
	@echo "TARGETS    := ${TARGETS}"

	@echo "GCC_PREFIX := ${GCC_PREFIX}"
	@echo "GCC_FLAGS  := ${GCC_FLAGS}"
	@echo "GCC_PATHS  := ${GCC_PATHS}"
	@echo "GCC_LIBS   := ${GCC_LIBS}"

build    :                      ${BUILD_PATHS} ${BUILD_FILES}

clean    :
	@rm --force --recursive ${BUILD_PATHS}

# =============================================================================
