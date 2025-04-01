/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#ifndef __DRIVER_IMP_H
#define __DRIVER_IMP_H

#include "driver.h"

// ============================================================================

#define DRIVER_CMD(x,...) void x( driver_ctx_t* ctx ) { __VA_ARGS__; }

#define DRIVER_ERR(x) {               \
  driver_byte_wr( ctx, ACK_FAILURE ); \
  driver_byte_wr( ctx, x           ); \
  driver_crc_wr( ctx, ctx->crc_wr );  \
}

#define DRIVER_CRC {                          \
  if( driver_crc_rd( ctx ) != ctx->crc_rd ) { \
    DRIVER_ERR( ERR_CRC );                    \
  }                                           \
}

typedef struct {
  crc_t crc_rd;
  crc_t crc_wr;
} driver_ctx_t;

#define DECLARE_SPR(x,y,z,...)
#define DECLARE_GPR(x,y,z,...)
#define DECLARE_CMD(x,y      ) extern void y( driver_ctx_t* ctx );
#define INCLUDE(x) 
#include "fiat.conf"
#undef  DECLARE_SPR
#undef  DECLARE_GPR
#undef  DECLARE_CMD
#undef  INCLUDE

// ============================================================================

#endif
