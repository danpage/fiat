# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

EMPTY      :=
SPACE      := ${EMPTY} ${EMPTY}
COLON      := :

# -----------------------------------------------------------------------------

INCLUDE := -D"DECLARE_SPR(...)=" -D"DECLARE_GPR(...)=" -D"DECLARE_CMD(...)=" -D"INCLUDE(x)=x"

${FIAT_PATH_BUILD}/target/${FIAT_BOARD}/fiat.conf : ${FIAT_PATH_REPO}/src/fiat/target/kernel/kernel.conf
	@mkdir --parents ${FIAT_PATH_BUILD}/target/${FIAT_BOARD}
	@cat ${<} | cpp -I ${FIAT_PATH_IMP} ${GCC_DEFS}            -P - | sed -e '/^[[:space:]]*$$/d' | envsubst                                                                    > ${@}

${FIAT_PATH_BUILD}/target/${FIAT_BOARD}/fiat.mk   : ${FIAT_PATH_REPO}/src/fiat/target/kernel/kernel.conf
	@mkdir --parents ${FIAT_PATH_BUILD}/target/${FIAT_BOARD}
	@cat ${<} | cpp -I ${FIAT_PATH_IMP} ${GCC_DEFS} ${INCLUDE} -P - | sed -e '/^[[:space:]]*$$/d' | envsubst | xargs --no-run-if-empty -- dirname | sed -e 's/^/INCLUDES += /'  > ${@}
	@cat ${<} | cpp -I ${FIAT_PATH_IMP} ${GCC_DEFS} ${INCLUDE} -P - | sed -e '/^[[:space:]]*$$/d' | envsubst | grep '.*\.h'                       | sed -e 's/^/ HEADERS += /' >> ${@}
	@cat ${<} | cpp -I ${FIAT_PATH_IMP} ${GCC_DEFS} ${INCLUDE} -P - | sed -e '/^[[:space:]]*$$/d' | envsubst | grep '.*\.c'                       | sed -e 's/^/ SOURCES += /' >> ${@}
	@cat ${<} | cpp -I ${FIAT_PATH_IMP} ${GCC_DEFS} ${INCLUDE} -P - | sed -e '/^[[:space:]]*$$/d' | envsubst | grep '.*\.s'                       | sed -e 's/^/ SOURCES += /' >> ${@}
	@cat ${<} | cpp -I ${FIAT_PATH_IMP} ${GCC_DEFS} ${INCLUDE} -P - | sed -e '/^[[:space:]]*$$/d' | envsubst | grep '.*\.S'                       | sed -e 's/^/ SOURCES += /' >> ${@}

-include ${FIAT_PATH_BUILD}/target/${FIAT_BOARD}/fiat.mk

# -----------------------------------------------------------------------------

INCLUDES   += ${FIAT_PATH_BUILD}/target/${FIAT_BOARD}
 HEADERS   += 
 SOURCES   += 

INCLUDES   +=             ${FIAT_PATH_REPO}/src/fiat/target/share
 HEADERS   += $(wildcard  ${FIAT_PATH_REPO}/src/fiat/target/share/*.[h])
 SOURCES   += $(wildcard  ${FIAT_PATH_REPO}/src/fiat/target/share/*.[csS])

INCLUDES   +=             ${FIAT_PATH_REPO}/src/fiat/target/board
 HEADERS   += $(wildcard  ${FIAT_PATH_REPO}/src/fiat/target/board/*.[h])
 SOURCES   += $(wildcard  ${FIAT_PATH_REPO}/src/fiat/target/board/*.[csS])
INCLUDES   +=             ${FIAT_PATH_REPO}/src/fiat/target/board/imp/${FIAT_BOARD}
 HEADERS   += $(wildcard  ${FIAT_PATH_REPO}/src/fiat/target/board/imp/${FIAT_BOARD}/*.[h])
 SOURCES   += $(wildcard  ${FIAT_PATH_REPO}/src/fiat/target/board/imp/${FIAT_BOARD}/*.[csS])

INCLUDES   +=             ${FIAT_PATH_REPO}/src/fiat/target/driver
 HEADERS   += $(wildcard  ${FIAT_PATH_REPO}/src/fiat/target/driver/*.[h])
 SOURCES   += $(wildcard  ${FIAT_PATH_REPO}/src/fiat/target/driver/*.[csS])
INCLUDES   +=             ${FIAT_PATH_REPO}/src/fiat/target/driver/imp/${FIAT_DRIVER}
 HEADERS   += $(wildcard  ${FIAT_PATH_REPO}/src/fiat/target/driver/imp/${FIAT_DRIVER}/*.[h])
 SOURCES   += $(wildcard  ${FIAT_PATH_REPO}/src/fiat/target/driver/imp/${FIAT_DRIVER}/*.[csS])

INCLUDES   +=             ${FIAT_PATH_REPO}/src/fiat/target/kernel
 HEADERS   += $(wildcard  ${FIAT_PATH_REPO}/src/fiat/target/kernel/*.[h])
 SOURCES   += $(wildcard  ${FIAT_PATH_REPO}/src/fiat/target/kernel/*.[csS])

INCLUDES   +=             ${FIAT_PATH_IMP}
 HEADERS   += $(wildcard  ${FIAT_PATH_IMP}/*.[h])
 SOURCES   += $(wildcard  ${FIAT_PATH_IMP}/*.[csS])

 OBJECTS   += $(addprefix ${FIAT_PATH_BUILD}/target/${FIAT_BOARD}/, $(notdir $(patsubst %.c, %.o, $(filter %.c, ${SOURCES}))))
 OBJECTS   += $(addprefix ${FIAT_PATH_BUILD}/target/${FIAT_BOARD}/, $(notdir $(patsubst %.s, %.o, $(filter %.s, ${SOURCES}))))
 OBJECTS   += $(addprefix ${FIAT_PATH_BUILD}/target/${FIAT_BOARD}/, $(notdir $(patsubst %.S, %.o, $(filter %.S, ${SOURCES}))))

GCC_FLAGS  +=
GCC_PATHS  += $(patsubst %, -I %, ${INCLUDES})
GCC_LIBS   +=

GCC_DEFS   += -DFIAT_VERSION_MAJOR="${FIAT_VERSION_MAJOR}"
GCC_DEFS   += -DFIAT_VERSION_MINOR="${FIAT_VERSION_MINOR}"
GCC_DEFS   += -DFIAT_VERSION_PATCH="${FIAT_VERSION_PATCH}" -DFIAT_CONTEXT='"${FIAT_CONTEXT}"' -DFIAT_DRIVER='"${FIAT_DRIVER}"' -DFIAT_BOARD='"${FIAT_BOARD}"'

-include ${FIAT_PATH_REPO}/src/fiat/target/board/imp/${FIAT_BOARD}/Makefile.in

vpath %.h $(subst ${SPACE},${COLON},${INCLUDES})
vpath %.c $(subst ${SPACE},${COLON},${INCLUDES})
vpath %.s $(subst ${SPACE},${COLON},${INCLUDES})
vpath %.S $(subst ${SPACE},${COLON},${INCLUDES})

# =============================================================================

BUILD_FILES += ${FIAT_PATH_BUILD}/target/${FIAT_BOARD}/fiat.conf
BUILD_FILES += ${FIAT_PATH_BUILD}/target/${FIAT_BOARD}/fiat.mk
BUILD_FILES += ${OBJECTS}
BUILD_FILES += ${TARGETS}

BUILD_PATHS += $(sort $(foreach FILE,${BUILD_FILES},$(dir ${FILE})))

${BUILD_PATHS} :
	@mkdir --parents ${@}

# -----------------------------------------------------------------------------

debug :	
	@echo "FIAT_CONTEXT    := ${FIAT_CONTEXT}"
	@echo "FIAT_DRIVER     := ${FIAT_DRIVER}"
	@echo "FIAT_BOARD      := ${FIAT_BOARD}"

	@echo "FIAT_PATH_REPO  := ${FIAT_PATH_REPO}"
	@echo "FIAT_PATH_IMP   := ${FIAT_PATH_IMP}"
	@echo "FIAT_PATH_BUILD := ${FIAT_PATH_BUILD}"
	@echo "FIAT_PATH_DEP   := ${FIAT_PATH_DEP}"

	@echo "INCLUDES        := ${INCLUDES}"
	@echo "SOURCES         := ${SOURCES}"
	@echo "HEADERS         := ${HEADERS}"
	@echo "OBJECTS         := ${OBJECTS}"
	@echo "TARGETS         := ${TARGETS}"

	@echo "GCC_PREFIX      := ${GCC_PREFIX}"
	@echo "GCC_FLAGS       := ${GCC_FLAGS}"
	@echo "GCC_PATHS       := ${GCC_PATHS}"
	@echo "GCC_LIBS        := ${GCC_LIBS}"
	@echo "GCC_DEFS        := ${GCC_DEFS}"

build :                         ${BUILD_PATHS} ${BUILD_FILES}

clean :
	@rm --force --recursive ${BUILD_PATHS}

# =============================================================================
