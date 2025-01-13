/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#ifndef __BOARD_H
#define __BOARD_H

#include "util.h"

// ============================================================================

/** Initialise the board.
  *
  * @return  a flag indicating failure (false) or success (true).
  */

extern bool  board_init();

// ----------------------------------------------------------------------------

/** Write a byte to   the UART,
  * blocking until successful.
  *
  * @param x the value to write.
  */

extern void  board_uart_wr( byte x );

/** Read  a byte from the UART,
  * blocking until successful.
  *
  * @return  the value     read.
  */

extern byte  board_uart_rd();

// ----------------------------------------------------------------------------

/** Write a value to   (or assert a value   on) a GPIO pin.
  *
  * @param p the GPIO pin identifier.
  * @param x the value to write.
  */

extern void  board_gpio_wr( gpio_t p, bool x );

/** Read  a value from (or sample a value from) a GPIO pin.
  *
  * @param p the GPIO pin identifier.
  * @return  the value     read.
  */

extern bool  board_gpio_rd( gpio_t p         );

// ----------------------------------------------------------------------------

/** Read (or sample) the time-stamp counter.
  *
  * @return  the value     read.
  */

extern tsc_t board_tsc_rd();

// ----------------------------------------------------------------------------

/** Read (or sample) n bytes of randomness.
  *
  * @param r the destination buffer.
  * @return  the number of bytes read (and then written into r, which may be less than n).
  */

extern int   board_rng_rd( byte* r, int n );

// ============================================================================

#endif
