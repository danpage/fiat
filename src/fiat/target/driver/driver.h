/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#ifndef __DRIVER_H
#define __DRIVER_H

#include "util.h"

#include  "board.h"
#include "kernel.h"

// ============================================================================

#define DRIVER_EXECUTE(x,y,z,...) {                \
  tsc_t tsc_0 = board_tsc_rd();                    \
  board_gpio_wr( PIN_TRIGGER, x );                 \
  ret_t ret   = z( __VA_ARGS__ );                  \
  board_gpio_wr( PIN_TRIGGER, y );                 \
  tsc_t tsc_1 = board_tsc_rd();                    \
                                                   \
  tsc_t tsc   = tsc_1 - tsc_0;                     \
                                                   \
  memcpy( spec_ret->data, &ret, SIZEOF( ret_t ) ); \
  memcpy( spec_tsc->data, &tsc, SIZEOF( tsc_t ) ); \
}

extern void driver_do_reset();

extern void driver_do_kernel         ( int op, int rep );
extern void driver_do_kernel_prologue( int op          );
extern void driver_do_kernel_epilogue( int op          );

extern void driver_interact();

// ============================================================================

#endif
