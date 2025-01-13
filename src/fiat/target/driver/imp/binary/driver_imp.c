/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#include "driver_imp.h" 

// ============================================================================

#define driver_byte_wr board_uart_wr
#define driver_byte_rd board_uart_rd

void driver_vint_wr( int x ) {
  while( true ) {
    byte t = x & 0x7F; x >>= 7;

    if( x ) {
      driver_byte_wr( t | 0x80 );
    }
    else {
      driver_byte_wr( t | 0x00 ); break;
    }
  }

  return;
}

int  driver_vint_rd() {
  int r = 0, n = 0;

  while( true ) {
    byte t = driver_byte_rd(); r |= ( t & 0x7F ) << n; n += 7;

    if( !( t & 0x80 ) ) {
      break;
    }
  }

  return r;
}

// ----------------------------------------------------------------------------

DRIVER_CMD(driver_cmd_ping,  {
  driver_byte_wr( ACK_SUCCESS );
});

DRIVER_CMD(driver_cmd_reset, {
  driver_do_reset();

  driver_byte_wr( ACK_SUCCESS );
});

DRIVER_CMD(driver_cmd_sizeof, {
  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd() ) ) == NULL ) {
    driver_byte_wr( ACK_FAILURE );
  }

  driver_byte_wr( ACK_SUCCESS ); driver_vint_wr( spec->size       );
});

DRIVER_CMD(driver_cmd_usedof, {
  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd() ) ) == NULL ) {
    driver_byte_wr( ACK_FAILURE );
  }

  driver_byte_wr( ACK_SUCCESS ); driver_vint_wr( spec->used       );
});

DRIVER_CMD(driver_cmd_typeof, {
  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd() ) ) == NULL ) {
    driver_byte_wr( ACK_FAILURE );
  }

  driver_byte_wr( ACK_SUCCESS ); driver_vint_wr( spec->type.flags );
});

DRIVER_CMD(driver_cmd_wr, {
  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd() ) ) == NULL ) {
    driver_byte_wr( ACK_FAILURE ); return;
  }
  if( !spec->type.wr ) {
    driver_byte_wr( ACK_FAILURE ); return;
  }

  int size = driver_vint_rd();

  if( ( spec->type.length == KERNEL_REG_LENGTH_FIX ) && ( size != spec->size ) ) {
    driver_byte_wr( ACK_FAILURE ); return;
  }
  if(                                                   ( size >  spec->size ) ) {
    driver_byte_wr( ACK_FAILURE ); return;
  }

  for( int i = 0; i < size; i++ ) {
    spec->data[ i ] = driver_byte_rd();
  }

  driver_byte_wr( ACK_SUCCESS );
});

DRIVER_CMD(driver_cmd_rd, {
  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd() ) ) == NULL ) {
    driver_byte_wr( ACK_FAILURE ); return;
  }
  if( !spec->type.rd ) {
    driver_byte_wr( ACK_FAILURE ); return;
  }

  int size;

  if( spec->type.length == KERNEL_REG_LENGTH_FIX ) {
    size = spec->size;
  }
  else {
    size = spec->used;
  }

  driver_byte_wr( ACK_SUCCESS ); driver_vint_wr( size );

  for( int i = 0; i < size; i++ ) {
    driver_byte_wr( spec->data[ i ] );
  }
});

DRIVER_CMD(driver_cmd_kernel,          {
  int op = driver_byte_rd(), rep = driver_vint_rd();

  driver_do_kernel( op, rep );

  driver_byte_wr( ACK_SUCCESS );
});

DRIVER_CMD(driver_cmd_kernel_prologue, {
  int op = driver_byte_rd();

  driver_do_kernel_prologue( op );

  driver_byte_wr( ACK_SUCCESS );
});

DRIVER_CMD(driver_cmd_kernel_epilogue, {
  int op = driver_byte_rd();

  driver_do_kernel_epilogue( op );

  driver_byte_wr( ACK_SUCCESS );
});

// ----------------------------------------------------------------------------

driver_ctx_t driver_ctx;

void driver_interact() {
  bool f = false;

  switch( driver_vint_rd() ) {
    #define DECLARE_SPR(x,y,z,...)
    #define DECLARE_GPR(x,y,z,...)
    #define DECLARE_CMD(x,y,z    ) case y: { z( &driver_ctx ); f = true; break; }
    #define INCLUDE(x) 
    #include "kernel.conf"
    #undef  DECLARE_SPR
    #undef  DECLARE_GPR
    #undef  DECLARE_CMD
    #undef  INCLUDE
  }

  if( !f ) {
    driver_byte_wr( ACK_FAILURE );
  }

  return;
}

// ============================================================================
