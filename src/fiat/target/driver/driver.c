/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#include "driver.h" 

// ============================================================================

kernel_reg_t* spec_ret = NULL;
kernel_reg_t* spec_tsc = NULL;

void driver_do_reset() {
  for( int i = 0; ( kernel_gpr[ i ].ident != NULL ); i++ ) {
    memset( kernel_gpr[ i ].data, 0, SIZEOF( byte ) * kernel_gpr[ i ].size );
  }

  return;
}

#define EXECUTE

void driver_do_kernel         ( int op, int rep ) {
  for( int i = 0; i < rep; i++ ) {
    kernel_prologue_minor( op, kernel_spr, kernel_gpr );
    DRIVER_EXECUTE( true, false, kernel,                op, kernel_spr, kernel_gpr);
    kernel_epilogue_minor( op, kernel_spr, kernel_gpr );
  }

  return;
}

void driver_do_kernel_prologue( int op          ) {
    DRIVER_EXECUTE(false, false, kernel_prologue_major, op, kernel_spr, kernel_gpr);

  return;
}

void driver_do_kernel_epilogue( int op          ) {
    DRIVER_EXECUTE(false, false, kernel_epilogue_major, op, kernel_spr, kernel_gpr);

  return;
}

// ----------------------------------------------------------------------------

int main( int argc, char* argv[] ) {
  if( !board_init() ) {
    return EXIT_FAILURE;
  }

  spec_ret = kernel_reg_byident( "ret" );
  spec_tsc = kernel_reg_byident( "tsc" );

  if( spec_ret == NULL ) {
    return EXIT_FAILURE;
  }
  if( spec_tsc == NULL ) {
    return EXIT_FAILURE;
  }

  while( true ) {
    driver_interact();
  }

    return EXIT_SUCCESS;
}

// ============================================================================
