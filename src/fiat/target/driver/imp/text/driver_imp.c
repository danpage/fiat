/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#include "driver_imp.h"

// ============================================================================

char* driver_line_rd( char* x ) {
  char* p = x;

  while( true ) {
    *p = board_uart_rd();

    if( *p == '\x0D' ) {
      break;
    }
    
    p++;
  }

  *p = '\x00';

  return x;
}

char* driver_line_wr( char* x ) {
  char* p = x;

  while( true ) {
    if( *p == '\x00' ) {
      break;
    }

    board_uart_wr( *p );

    p++;
  }

  board_uart_wr( '\x0D' );

  return x;
}

// ----------------------------------------------------------------------------

char* driver_byte_wr( char* r, byte x ) {
  *r++ = inttohex( ( x >> 4 ) & 0xF );
  *r++ = inttohex( ( x >> 0 ) & 0xF );

  *r   = '\x00';

  return r;
}

byte  driver_byte_rd( char* x ) {
  byte r  = ( byte )( hextoint( *x++ ) & 0xF ) << 4;
       r |= ( byte )( hextoint( *x++ ) & 0xF ) << 0;

  return r;
}

char* driver_vint_wr( char* r, int  x ) {
  while( true ) {
    byte t = x & 0x7F; x >>= 7;

    if( x ) {
      driver_byte_wr( r, t | 0x80 ); r += 2;
    }
    else {
      driver_byte_wr( r, t | 0x00 ); r += 2; break;
    }
  }

  *r   = '\x00'; 

  return r;
}

int   driver_vint_rd( char* x ) {
  int r = 0, n = 0;

  while( true ) {
    byte t = driver_byte_rd( x ); x += 2; r |= ( t & 0x7F ) << n; n += 7;

    if( !( t & 0x80 ) ) {
      break;
    }
  }

  return r;
}

// ----------------------------------------------------------------------------

DRIVER_CMD(driver_cmd_ping, {
  char* p = ctx->ack;

  if( ctx->tok_len != 1 ) {
    p = strext( p, "-" ); return;
  }

    p = strext( p, "+" );
});

DRIVER_CMD(driver_cmd_reset, {
  char* p = ctx->ack;

  if( ctx->tok_len != 1 ) {
    p = strext( p, "-" ); return;
  }

  driver_do_reset();

    p = strext( p, "+" );
});

DRIVER_CMD(driver_cmd_version, {
  char* p = ctx->ack;

  if( ctx->tok_len != 1 ) {
    p = strext( p, "-" ); return;
  }

    p = strext( p, "+" );
    p = strext( p, " " );
    p = driver_byte_wr( p, FIAT_VERSION_PATCH );
    p = strext( p, " " );
    p = driver_byte_wr( p, FIAT_VERSION_MINOR );
    p = strext( p, " " );
    p = driver_byte_wr( p, FIAT_VERSION_MAJOR );
});

DRIVER_CMD(driver_cmd_nameof, {
  char* p = ctx->ack;

  if( ctx->tok_len != 2 ) {
    p = strext( p, "-" ); return;
  }

  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd( ctx->tok_ptr[ 1 ] ) ) ) == NULL ) {
    p = strext( p, "-" ); return;
  }

  int size = strlen( spec->ident );

    p = strext( p, "+" );
    p = strext( p, " " );
    p = driver_vint_wr( p, size             );
    p = strext( p, " " );

  for( int i = 0; i < size; i++ ) {
    p = driver_byte_wr( p, spec->ident[ i ] );
  }
});

DRIVER_CMD(driver_cmd_sizeof, {
  char* p = ctx->ack;

  if( ctx->tok_len != 2 ) {
    p = strext( p, "-" ); return;
  }

  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd( ctx->tok_ptr[ 1 ] ) ) ) == NULL ) {
    p = strext( p, "-" ); return;
  }

    p = strext( p, "+" );
    p = strext( p, " " );
    p = driver_vint_wr( p, spec->size       );
});

DRIVER_CMD(driver_cmd_usedof, {
  char* p = ctx->ack;

  if( ctx->tok_len != 2 ) {
    p = strext( p, "-" ); return;
  }

  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd( ctx->tok_ptr[ 1 ] ) ) ) == NULL ) {
    p = strext( p, "-" ); return;
  }

    p = strext( p, "+" );
    p = strext( p, " " );
    p = driver_vint_wr( p, spec->used       );
});

DRIVER_CMD(driver_cmd_typeof, {
  char* p = ctx->ack;

  if( ctx->tok_len != 2 ) {
    p = strext( p, "-" ); return;
  }

  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd( ctx->tok_ptr[ 1 ] ) ) ) == NULL ) {
    p = strext( p, "-" ); return;
  }

    p = strext( p, "+" );
    p = strext( p, " " );
    p = driver_vint_wr( p, spec->type.flags );
});

DRIVER_CMD(driver_cmd_wr, {
  char* p = ctx->ack;

  if( ctx->tok_len != 4 ) {
    p = strext( p, "-" ); return;
  }

  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd( ctx->tok_ptr[ 1 ] ) ) ) == NULL ) {
    p = strext( p, "-" ); return;
  }
  if( !spec->type.wr ) {
    p = strext( p, "-" ); return;
  }

  int size = driver_byte_rd( ctx->tok_ptr[ 2 ] );

  if( ( spec->type.length == KERNEL_REG_LENGTH_FIX ) && ( size != spec->size ) ) {
    p = strext( p, "-" ); return;
  }
  if(                                                   ( size >  spec->size ) ) {
    p = strext( p, "-" ); return;
  }

  for( int i = 0; i < size; i++ ) {
    spec->data[ i ] = driver_byte_rd( ctx->tok_ptr[ 3 ] + ( i << 1 ) );
  }

    p = strext( p, "+" );
});

DRIVER_CMD(driver_cmd_rd, {
  char* p = ctx->ack;

  if( ctx->tok_len != 2 ) {
    p = strext( p, "-" ); return;
  }

  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd( ctx->tok_ptr[ 1 ] ) ) ) == NULL ) {
    p = strext( p, "-" ); return;
  }
  if( !spec->type.rd ) {
    p = strext( p, "-" ); return;
  }

  int size;

  if( spec->type.length == KERNEL_REG_LENGTH_FIX ) {
    size = spec->size;
  }
  else {
    size = spec->used;
  }

    p = strext( p, "+" );
    p = strext( p, " " );
    p = driver_vint_wr( p, size             );
    p = strext( p, " " );

  for( int i = 0; i < size; i++ ) {
    p = driver_byte_wr( p, spec->data[ i ]  );
  }
});

DRIVER_CMD(driver_cmd_kernel, {
  char* p = ctx->ack;

  if( ctx->tok_len != 3 ) {
    p = strext( p, "-" ); return;
  }

  int op = driver_byte_rd( ctx->tok_ptr[ 1 ] ), rep = driver_vint_rd( ctx->tok_ptr[ 2 ] );

  driver_do_kernel( op, rep );

    p = strext( p, "+" );
});

DRIVER_CMD(driver_cmd_kernel_prologue, {
  char* p = ctx->ack;

  if( ctx->tok_len != 2 ) {
    p = strext( p, "-" ); return;
  }

  int op = driver_byte_rd( ctx->tok_ptr[ 1 ] );

  driver_do_kernel_prologue( op );

    p = strext( p, "+" );
});

DRIVER_CMD(driver_cmd_kernel_epilogue, {
  char* p = ctx->ack;

  if( ctx->tok_len != 2 ) {
    p = strext( p, "-" ); return;
  }

  int op = driver_byte_rd( ctx->tok_ptr[ 1 ] );

  driver_do_kernel_epilogue( op );

    p = strext( p, "+" );
});

// ----------------------------------------------------------------------------

driver_ctx_t driver_ctx;

void driver_interact() {
  driver_ctx.tok_len = 0;

  for( int i = 0; i < ( SIZEOF( driver_ctx.tok_ptr ) / SIZEOF( char* ) ); i++ ) {
    driver_ctx.tok_ptr[ i ] = NULL;
  }

  strcpy( driver_ctx.req, "" );
  strcpy( driver_ctx.ack, "" );

  for( char* p = driver_line_rd( driver_ctx.req ); true; p++ ) {
    if( driver_ctx.tok_ptr[ driver_ctx.tok_len ] == NULL ) {
      driver_ctx.tok_ptr[ driver_ctx.tok_len ] = p;
    }

    if( *p == '\x00' ) { //   eol
                   driver_ctx.tok_len++;    break;
    }
    if( *p == '\x20' ) { // space
      *p = '\x00'; driver_ctx.tok_len++; continue;
    }
  }

  bool f = false;

  if( driver_ctx.tok_len > 0 ) {
    switch( driver_ctx.tok_ptr[ 0 ][ 0 ] ) {
      #define DECLARE_SPR(x,y,z,...)
      #define DECLARE_GPR(x,y,z,...)
      #define DECLARE_CMD(x,y      ) case x: { y( &driver_ctx ); f = true; break; }
      #define INCLUDE(x) 
      #include "fiat.conf"
      #undef  DECLARE_SPR
      #undef  DECLARE_GPR
      #undef  DECLARE_CMD
      #undef  INCLUDE
    }
  }

  if( !f ) {
    strext( driver_ctx.ack, "-" );
  }

  driver_line_wr( driver_ctx.ack );

  return;
}

// ============================================================================
