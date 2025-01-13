/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#ifndef __KERNEL_H
#define __KERNEL_H

#include "util.h"

// ============================================================================

#define KERNEL_REG_LENGTH_FIX ( 0 )
#define KERNEL_REG_LENGTH_VAR ( 1 )

typedef union {
    uint8_t  flags;

  struct {
    uint8_t     wr:1;
    uint8_t     rd:1;
    uint8_t length:1;
  };
} kernel_reg_type_t;

typedef struct {
  char* ident; byte* data; int size; int used; kernel_reg_type_t type;
} kernel_reg_t;

// ----------------------------------------------------------------------------

/** The kernel Special-Purpose Register (SPR) set.
  */

extern kernel_reg_t kernel_spr[];

/** The kernel General-Purpose Register (GPR) set.
  */

extern kernel_reg_t kernel_gpr[];

/** Search for a register based on an identifier.
  *
  * @param ident the identifier.
  * @return a GPR or SPR whose identifier matches ident, or NULL if none exists.
  */

extern kernel_reg_t* kernel_reg_byident( char* ident );

/** Search for a register based on an index.
  *
  * @param index the index.
  * @return a GPR or SPR whose index      matches index, or NULL if none exists.
  */

extern kernel_reg_t* kernel_reg_byindex( int   index );

/** The kernel functionality.
  *
  * @param op  an operation identifier (or opcode), allowing selection of sub-functionality.
  * @param spr the special-purpose register set.
  * @param gpr the general-purpose register set.
  * @return a return code indicating failure (e.g., EXIT_FAILURE) or success (e.g., EXIT_SUCCESS).
  */

extern ret_t kernel               ( int op, kernel_reg_t* spr, kernel_reg_t* gpr );

/** A function that performs "major" initialisation,
  * optionally, and manually
  * invoked before the kernel functionality (e.g., to   allocate memory).
  *
  * @param op  an operation identifier (or opcode), allowing selection of sub-functionality.
  * @param spr the special-purpose register set.
  * @param gpr the general-purpose register set.
  * @return a return code indicating failure (e.g., EXIT_FAILURE) or success (e.g., EXIT_SUCCESS).
  */

extern ret_t kernel_prologue_major( int op, kernel_reg_t* spr, kernel_reg_t* gpr );

/** A function that performs "major"   finalisation,
  * optionally, and manually
  * invoked after  the kernel functionality (e.g., to deallocate memory).
  *
  * @param op  an operation identifier (or opcode), allowing selection of sub-functionality.
  * @param spr the special-purpose register set.
  * @param gpr the general-purpose register set.
  * @return a return code indicating failure (e.g., EXIT_FAILURE) or success (e.g., EXIT_SUCCESS).
  */

extern ret_t kernel_epilogue_major( int op, kernel_reg_t* spr, kernel_reg_t* gpr );

/** A function that performs "minor" initialisation,
  * automatically
  * invoked before the kernel functionality.
  *
  * @param op  an operation identifier (or opcode), allowing selection of sub-functionality.
  * @param spr the special-purpose register set.
  * @param gpr the general-purpose register set.
  */

extern void  kernel_prologue_minor( int op, kernel_reg_t* spr, kernel_reg_t* gpr );

/** A function that performs "minor"   finalisation,
  * automatically
  * invoked after  the kernel functionality.
  *
  * @param op  an operation identifier (or opcode), allowing selection of sub-functionality.
  * @param spr the special-purpose register set.
  * @param gpr the general-purpose register set.
  */

extern void  kernel_epilogue_minor( int op, kernel_reg_t* spr, kernel_reg_t* gpr );

// ============================================================================

#endif
