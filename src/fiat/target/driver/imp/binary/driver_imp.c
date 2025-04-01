/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#include "driver_imp.h" 

// ============================================================================

void  driver_crc_wr( driver_ctx_t* ctx, crc_t x ) {
  board_uart_wr( ( ( ( crc_t )( x ) ) >> 0 ) & 0xFF );
  board_uart_wr( ( ( ( crc_t )( x ) ) >> 8 ) & 0xFF );

  return;
}

crc_t driver_crc_rd( driver_ctx_t* ctx          ) {
  crc_t r = ( ( ( crc_t )( board_uart_rd() ) & 0xFF ) << 0 ) |
            ( ( ( crc_t )( board_uart_rd() ) & 0xFF ) << 8 ) ;

  return r;
}

void  driver_byte_wr( driver_ctx_t* ctx, byte x ) {
  ctx->crc_wr = crc( ctx->crc_wr, &x, 1 );

  board_uart_wr( x );

  return;
}

byte  driver_byte_rd( driver_ctx_t* ctx         ) {
  byte x = board_uart_rd();

  ctx->crc_rd = crc( ctx->crc_rd, &x, 1 );

  return x;
}

void  driver_vint_wr( driver_ctx_t* ctx, int x ) {
  while( true ) {
    byte t = x & 0x7F; x >>= 7;

    if( x ) {
      driver_byte_wr( ctx, t | 0x80 );
    }
    else {
      driver_byte_wr( ctx, t | 0x00 ); break;
    }
  }

  return;
}

int   driver_vint_rd( driver_ctx_t* ctx        ) {
  int r = 0, n = 0;

  while( true ) {
    byte t = driver_byte_rd( ctx ); r |= ( t & 0x7F ) << n; n += 7;

    if( !( t & 0x80 ) ) {
      break;
    }
  }

  return r;
}

// ----------------------------------------------------------------------------

DRIVER_CMD(driver_cmd_ping,{
  DRIVER_CRC;

  driver_byte_wr( ctx, ACK_SUCCESS );
  driver_crc_wr( ctx, ctx->crc_wr );
});

DRIVER_CMD(driver_cmd_reset,{
  DRIVER_CRC;

  driver_do_reset();

  driver_byte_wr( ctx, ACK_SUCCESS );
  driver_crc_wr( ctx, ctx->crc_wr );
});

DRIVER_CMD(driver_cmd_version,{
  DRIVER_CRC;

  driver_byte_wr( ctx, ACK_SUCCESS );

  driver_byte_wr( ctx, FIAT_VERSION_PATCH );
  driver_byte_wr( ctx, FIAT_VERSION_MINOR );
  driver_byte_wr( ctx, FIAT_VERSION_MAJOR );

  driver_crc_wr( ctx, ctx->crc_wr );
});

DRIVER_CMD(driver_cmd_nameof,{
  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd( ctx ) ) ) == NULL ) {
    DRIVER_ERR( ERR_INDEX      );
  }

  int size = strlen( spec->ident );

  DRIVER_CRC;

  driver_byte_wr( ctx, ACK_SUCCESS ); driver_vint_wr( ctx,       size       ); 

  for( int i = 0; i < size; i++ ) {
    driver_byte_wr( ctx, spec->ident[ i ] );
  }

  driver_crc_wr( ctx, ctx->crc_wr );
});

DRIVER_CMD(driver_cmd_sizeof,{
  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd( ctx ) ) ) == NULL ) {
    DRIVER_ERR( ERR_INDEX      );
  }

  DRIVER_CRC;

  driver_byte_wr( ctx, ACK_SUCCESS ); driver_vint_wr( ctx, spec->size       );
  driver_crc_wr( ctx, ctx->crc_wr );
});

DRIVER_CMD(driver_cmd_usedof,{
  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd( ctx ) ) ) == NULL ) {
    DRIVER_ERR( ERR_INDEX      );
  }

  DRIVER_CRC;

  driver_byte_wr( ctx, ACK_SUCCESS ); driver_vint_wr( ctx, spec->used       );
  driver_crc_wr( ctx, ctx->crc_wr );
});

DRIVER_CMD(driver_cmd_typeof,{
  kernel_reg_t* spec = NULL;

  if( ( spec = kernel_reg_byindex( driver_byte_rd( ctx ) ) ) == NULL ) {
    DRIVER_ERR( ERR_INDEX      );
  }

  DRIVER_CRC;

  driver_byte_wr( ctx, ACK_SUCCESS ); driver_vint_wr( ctx, spec->type.flags );
  driver_crc_wr( ctx, ctx->crc_wr );
});

DRIVER_CMD(driver_cmd_wr,{
  kernel_reg_t* spec = NULL; int size;

  if( ( spec = kernel_reg_byindex( driver_byte_rd( ctx ) ) ) == NULL ) {
    DRIVER_ERR( ERR_INDEX      );
  }
  if( !spec->type.wr ) {
    DRIVER_ERR( ERR_PERMISSION );
  }

  size = driver_vint_rd( ctx );

  if( ( spec->type.length == KERNEL_REG_LENGTH_FIX ) && ( size != spec->size ) ) {
    DRIVER_ERR( ERR_SIZE       );
  }
  if(                                                   ( size >  spec->size ) ) {
    DRIVER_ERR( ERR_SIZE       );
  }

  for( int i = 0; i < size; i++ ) {
    spec->data[ i ] = driver_byte_rd( ctx );
  }

  DRIVER_CRC;

  driver_byte_wr( ctx, ACK_SUCCESS );
  driver_crc_wr( ctx, ctx->crc_wr );
});

DRIVER_CMD(driver_cmd_rd,{
  kernel_reg_t* spec = NULL; int size;

  if( ( spec = kernel_reg_byindex( driver_byte_rd( ctx ) ) ) == NULL ) {
    DRIVER_ERR( ERR_INDEX      );
  }
  if( !spec->type.rd ) {
    DRIVER_ERR( ERR_PERMISSION );
  }

  if( spec->type.length == KERNEL_REG_LENGTH_FIX ) {
    size = spec->size;
  }
  else {
    size = spec->used;
  }

  DRIVER_CRC;

  driver_byte_wr( ctx, ACK_SUCCESS ); driver_vint_wr( ctx, size );

  for( int i = 0; i < size; i++ ) {
    driver_byte_wr( ctx, spec->data[ i ] );
  }

  driver_crc_wr( ctx, ctx->crc_wr );
});

DRIVER_CMD(driver_cmd_kernel,{
  int op = driver_byte_rd( ctx ), rep = driver_vint_rd( ctx );

  DRIVER_CRC;

  driver_do_kernel( op, rep );

  driver_byte_wr( ctx, ACK_SUCCESS );
  driver_crc_wr( ctx, ctx->crc_wr );
});

DRIVER_CMD(driver_cmd_kernel_prologue,{
  int op = driver_byte_rd( ctx );

  DRIVER_CRC;

  driver_do_kernel_prologue( op );

  driver_byte_wr( ctx, ACK_SUCCESS );
  driver_crc_wr( ctx, ctx->crc_wr );
});

DRIVER_CMD(driver_cmd_kernel_epilogue,{
  int op = driver_byte_rd( ctx );

  DRIVER_CRC;

  driver_do_kernel_epilogue( op );

  driver_byte_wr( ctx, ACK_SUCCESS );
  driver_crc_wr( ctx, ctx->crc_wr );
});

// ----------------------------------------------------------------------------

driver_ctx_t driver_ctx;

void driver_interact() {
  driver_ctx_t* ctx = &driver_ctx;

  ctx->crc_rd = crc( 0, NULL, 0 );
  ctx->crc_wr = crc( 0, NULL, 0 );

  switch( driver_vint_rd( ctx ) ) {
    #define DECLARE_SPR(x,y,z,...)
    #define DECLARE_GPR(x,y,z,...)
    #define DECLARE_CMD(x,y      ) case x: { y( ctx ); break; }
    #define INCLUDE(x) 
    #include "fiat.conf"
    #undef  DECLARE_SPR
    #undef  DECLARE_GPR
    #undef  DECLARE_CMD
    #undef  INCLUDE

    default : {
      DRIVER_ERR( ERR_COMMAND );
      break;
    }
  }

  return;
}

// ============================================================================
