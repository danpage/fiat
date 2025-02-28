/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#include "kernel.h"

// ============================================================================

  #define DECLARE_SPR(x,y,z,...) byte __ ## x[ y ] = z;
  #define DECLARE_GPR(x,y,z,...) byte __ ## x[ y ] = z;
  #define DECLARE_CMD(x,y      )
  #define INCLUDE(x) 
  #include "fiat.conf"
  #undef  DECLARE_SPR
  #undef  DECLARE_GPR
  #undef  DECLARE_CMD
  #undef  INCLUDE

kernel_reg_t kernel_spr[] = {
  #define DECLARE_SPR(x,y,z,...) { .ident =   #x, .data = __ ## x, .size = y, .used = 0, .type = { __VA_ARGS__ } },
  #define DECLARE_GPR(x,y,z,...)
  #define DECLARE_CMD(x,y      )
  #define INCLUDE(x) 
  #include "fiat.conf"
  #undef  DECLARE_SPR
  #undef  DECLARE_GPR
  #undef  DECLARE_CMD
  #undef  INCLUDE
                                 { .ident = NULL, .data =    NULL, .size = 0, .used = 0, .type = {}              }
};

kernel_reg_t kernel_gpr[] = {
  #define DECLARE_SPR(x,y,z,...)
  #define DECLARE_GPR(x,y,z,...) { .ident =   #x, .data = __ ## x, .size = y, .used = 0, .type = { __VA_ARGS__ } },
  #define DECLARE_CMD(x,y      )
  #define INCLUDE(x) 
  #include "fiat.conf"
  #undef  DECLARE_SPR
  #undef  DECLARE_GPR
  #undef  DECLARE_CMD
  #undef  INCLUDE
                                 { .ident = NULL, .data =    NULL, .size = 0, .used = 0, .type = {}              }
};

// ----------------------------------------------------------------------------

kernel_reg_t* kernel_reg_byident( char* ident ) {
  kernel_reg_t* sprs = kernel_spr;

  for( int i = 0; ( sprs[ i ].ident != NULL ); i++ ) {
    if( !strcmp( ident, sprs[ i ].ident ) ) {
      return sprs + i;
    }
  }

  kernel_reg_t* gprs = kernel_gpr;

  for( int i = 0; ( gprs[ i ].ident != NULL ); i++ ) {
    if( !strcmp( ident, gprs[ i ].ident ) ) {
      return gprs + i;
    }
  }

  return NULL;
}

kernel_reg_t* kernel_reg_byindex( int   index ) {
  if(  ( index & 0x80 ) ) {
    kernel_reg_t* sprs = kernel_spr;

    for( int i = 0; ( sprs[ i ].ident != NULL ); i++ ) {
      if( ( index & 0x7F ) == i ) {
        return sprs + i;
      }
    }
  }
  if( !( index & 0x80 ) ) {
    kernel_reg_t* gprs = kernel_gpr;

    for( int i = 0; ( gprs[ i ].ident != NULL ); i++ ) {
      if( ( index & 0x7F ) == i ) {
        return gprs + i;
      }
    }
  }

  return NULL;
}

// ----------------------------------------------------------------------------

/** Provide a "weak", empty default version of major kernel prologue,
  * e.g., allowing a light-weight kernel implementation to omit it.
  */

ret_t __attribute__((weak)) kernel_prologue_major( int op, kernel_reg_t* spr, kernel_reg_t* gpr ) {
  return EXIT_SUCCESS;
}

/** Provide a "weak", empty default version of major kernel epilogue,
  * e.g., allowing a light-weight kernel implementation to omit it.
  */

ret_t __attribute__((weak)) kernel_epilogue_major( int op, kernel_reg_t* spr, kernel_reg_t* gpr ) {
  return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------

/** Provide a "weak", empty default version of minor kernel prologue,
  * e.g., allowing a light-weight kernel implementation to omit it.
  */

void  __attribute__((weak)) kernel_prologue_minor( int op, kernel_reg_t* spr, kernel_reg_t* gpr ) {

}

/** Provide a "weak", empty default version of minor kernel epilogue,
  * e.g., allowing a light-weight kernel implementation to omit it.
  */

void  __attribute__((weak)) kernel_epilogue_minor( int op, kernel_reg_t* spr, kernel_reg_t* gpr ) {

}

// ============================================================================
