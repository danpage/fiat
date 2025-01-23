/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#include "board_imp.h"

// ============================================================================

bool  board_init() {
  return  true;
}

// ----------------------------------------------------------------------------

void  board_uart_wr( byte x ) {
         putc( x, stdout ); fflush( stdout );
}

byte  board_uart_rd() {
  return getc(    stdin  );
}

// ----------------------------------------------------------------------------

void  board_gpio_wr( pin_t p, bool x ) {

}

bool  board_gpio_rd( pin_t p         ) {
  return false;
}

// ----------------------------------------------------------------------------

tsc_t board_tsc_rd() {
  return 0;
}

// ----------------------------------------------------------------------------

int   board_rng_rd( byte* r, int n ) {
  return 0;
}

// ============================================================================
