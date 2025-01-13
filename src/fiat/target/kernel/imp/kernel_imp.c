/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#include "kernel_imp.h"

// ============================================================================

ret_t kernel( int op, kernel_reg_t* spr, kernel_reg_t* gpr ) {
  return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------

ret_t kernel_prologue_major( int op, kernel_reg_t* spr, kernel_reg_t* gpr ) {
  return EXIT_SUCCESS;
}

ret_t kernel_epilogue_major( int op, kernel_reg_t* spr, kernel_reg_t* gpr ) {
  return EXIT_SUCCESS;
}

// ----------------------------------------------------------------------------

void  kernel_prologue_minor( int op, kernel_reg_t* spr, kernel_reg_t* gpr ) {

}

void  kernel_epilogue_minor( int op, kernel_reg_t* spr, kernel_reg_t* gpr ) {

}

// ============================================================================
