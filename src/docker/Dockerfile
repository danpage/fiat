# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

# =============================================================================

FROM ubuntu

ARG DATE
ARG FIAT_BOARD

LABEL maintainer="Daniel Page <dan@phoo.org>" url="https://github.com/danpage/fiat" date="${DATE}" 

# -----------------------------------------------------------------------------

ENV DEBIAN_FRONTEND="noninteractive"

RUN apt-get --assume-yes --quiet update \
 && apt-get --assume-yes --quiet install apt-utils autoconf automake bzip2 doxygen gettext gcc git gosu make python3 sudo wget xz-utils

# -----------------------------------------------------------------------------

ENV FIAT_DEPS="/opt/fiat_deps/${FIAT_BOARD}"
RUN mkdir --parents ${FIAT_DEPS}

include(`Dockerfile.in')

COPY Makefile ${FIAT_DEPS}/Makefile
RUN make --directory="${FIAT_DEPS}" fetch-deps \
 && make --directory="${FIAT_DEPS}" patch-deps \
 && make --directory="${FIAT_DEPS}" build-deps

# -----------------------------------------------------------------------------

COPY ./entrypoint.sh /usr/sbin/entrypoint.sh
RUN chmod 755 /usr/sbin/entrypoint.sh
ENTRYPOINT [ "/usr/sbin/entrypoint.sh" ]

# =============================================================================
