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

#define DRIVER_CMD(x,...) void x( driver_ctx_t* ctx ) { __VA_ARGS__; return; }

typedef struct {

} driver_ctx_t;

#define DECLARE_SPR(x,y,z,...)
#define DECLARE_GPR(x,y,z,...)
#define DECLARE_CMD(x,y      ) extern void y( driver_ctx_t* ctx );
#define INCLUDE(x) 
#include "kernel.conf"
#undef  DECLARE_SPR
#undef  DECLARE_GPR
#undef  DECLARE_CMD
#undef  INCLUDE

// ============================================================================

#endif
