/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#include "board_imp.h"

// ============================================================================

bool  board_init() {
  set_global_interrupt_enable( 0 );
  
  return  true;
}

// ----------------------------------------------------------------------------

void  board_uart_wr( byte x ) {
  uart_t uart = DEFAULT_UART;

  while( DEV_READ( uart + UART_STATUS_REG ) & UART_STATUS_TX_FULL  ) {
    /* spin */
  }

         DEV_WRITE( uart + UART_TX_REG, x );
}

byte  board_uart_rd() {
  uart_t uart = DEFAULT_UART;

  while( DEV_READ( uart + UART_STATUS_REG ) & UART_STATUS_RX_EMPTY ) { 
    /* spin */
  }

  return DEV_READ ( uart + UART_RX_REG    );
}

// ----------------------------------------------------------------------------

void  board_gpio_wr( pin_t p, bool x ) {
  if( p == PIN_TRIGGER ) {
    if( x ) {
      set_outputs( GPIO_OUT, 0x1 );
    }
    else {
      set_outputs( GPIO_OUT, 0x0 );
    }
  }
}

bool  board_gpio_rd( pin_t p         ) {
  return false;
}

// ----------------------------------------------------------------------------

tsc_t board_tsc_rd() {
  return ( tsc_t )( get_mcycle() );
}

// ----------------------------------------------------------------------------

int   board_rng_rd( byte* r, int n ) {
  return 0;
}

// ============================================================================
