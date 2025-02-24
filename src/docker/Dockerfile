# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

FROM ubuntu

ARG DATE

ARG FIAT_CONTEXT
ARG FIAT_DRIVER
ARG FIAT_BOARD

ARG FIAT_PATH_REPO
ARG FIAT_PATH_IMP
ARG FIAT_PATH_BUILD
ARG FIAT_PATH_DEP

LABEL maintainer="Daniel Page <dan@phoo.org>" url="https://github.com/danpage/fiat" date="${DATE}" 

# -----------------------------------------------------------------------------

ENV DEBIAN_FRONTEND="noninteractive"

RUN apt-get --assume-yes --quiet update \
 && apt-get --assume-yes --quiet install apt-utils autoconf automake bzip2 doxygen gettext gcc git gosu make python3 sudo wget xz-utils

# -----------------------------------------------------------------------------

RUN mkdir --parents ${FIAT_PATH_DEP}

include(`Dockerfile.in')

COPY Makefile ${FIAT_PATH_DEP}/Makefile
RUN make --directory="${FIAT_PATH_DEP}" fetch-dep \
 && make --directory="${FIAT_PATH_DEP}" patch-dep \
 && make --directory="${FIAT_PATH_DEP}" build-dep

# -----------------------------------------------------------------------------

COPY ./entrypoint.sh /usr/sbin/entrypoint.sh
RUN chmod 755 /usr/sbin/entrypoint.sh
ENTRYPOINT [ "/usr/sbin/entrypoint.sh" ]

# =============================================================================
