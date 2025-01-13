/* Copyright (C) 2024 Daniel Page <dan@phoo.org>
 *
 * Use of this source code is restricted per the MIT license, a copy of which 
 * can be found via https://opensource.org/license/mit (and which is included 
 * as LICENSE.txt within the associated archive or repository).
 */

#include "util.h"

// ============================================================================

bool ishex( char x ) {
  if      ( x >= '0' && x <= '9' ) {
    return true;
  }
  else if ( x >= 'a' && x <= 'f' ) {
    return true;
  }
  else if ( x >= 'A' && x <= 'F' ) {
    return true;
  }

  return false;
}

int  hextoint( char x ) {
  if      ( x >= '0' && x <= '9' ) {
    return ( ( int )( x ) - ( int )( '0' )      );
  }
  else if ( x >= 'a' && x <= 'f' ) {
    return ( ( int )( x ) - ( int )( 'a' ) + 10 );
  }
  else if ( x >= 'A' && x <= 'F' ) {
    return ( ( int )( x ) - ( int )( 'A' ) + 10 );
  }

  return -1;
}

char inttohex( int  x ) {
  if     ( x >=  0 && x <=  9 ) {
    return ( char )( ( int )( '0' ) + x       );
  }
  else if( x >= 10 && x <= 15 ) {
    return ( char )( ( int )( 'A' ) + x - 10  );
  }

  return '?';
}

// ============================================================================
