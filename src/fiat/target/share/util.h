/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#ifndef __UTIL_H
#define __UTIL_H

#include <stdbool.h>
#include  <stdint.h>
#include  <stdlib.h>
#include   <stdio.h>

#include  <limits.h>
#include  <string.h>

// ============================================================================

#define SIZEOF(x) ( sizeof(x)     )
#define BITSOF(x) ( sizeof(x) * 8 )

// ----------------------------------------------------------------------------

/** An enumeration 
  * that captures
  * communicated request         tags.
  */

typedef enum {
  REQ_PING            = 0x00,
  REQ_RESET           = 0x01,

  REQ_SIZEOF          = 0x02,
  REQ_USEDOF          = 0x03,
  REQ_TYPEOF          = 0x04,

  REQ_WR              = 0x05,
  REQ_RD              = 0x06,

  REQ_KERNEL          = 0x07,
  REQ_KERNEL_PROLOGUE = 0x08,
  REQ_KERNEL_EPILOGUE = 0x09
} req_t;

/** An enumeration 
  * that captures
  * communicated acknowledgement tags.
  */

typedef enum {
  ACK_SUCCESS         = 0x00,
  ACK_FAILURE         = 0x01
} ack_t;

typedef uint8_t byte;

/** An enumeration 
  * that captures
  * GPIO pin identifiers.
  */

typedef enum {
  PIN_TRIGGER
} pin_t;

/** shared
  */

/** A type 
  * that captures
  * Time-Stamp Counter (TSC)
  * values.
  */

typedef uint64_t tsc_t;

/** A type 
  * that captures
  * return code
  * values.
  */

typedef byte     ret_t;

// ----------------------------------------------------------------------------

extern bool    ishex( char x );

extern int  hextoint( char x );
extern char inttohex( int  x );

// ============================================================================

#endif
