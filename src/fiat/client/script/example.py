# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

import argparse, libfiat, logging, random, sys

# =============================================================================

GPR_C = 0x00
GPR_K = 0x01
GPR_M = 0x02

# -----------------------------------------------------------------------------

if ( __name__ == '__main__' ) :
  client = libfiat.open( sys.argv ) ; random.seed( 0 )

  ( ack,   ) = client.reset()

  for index in [ GPR_C, GPR_K, GPR_M ] :
    ( ack, nameof ) = client.nameof( index )
    print( 'nameof( 0x{0:02X} ) = {1:s}'.format( index, nameof ) )  

  for index in [ GPR_C, GPR_K, GPR_M ] :
    ( ack, sizeof ) = client.sizeof( index )
    print( 'sizeof( 0x{0:02X} ) = {1:d}'.format( index, sizeof ) )

  for index in [ GPR_C, GPR_K, GPR_M ] :
    ( ack, typeof ) = client.typeof( index )
    print( 'typeof( 0x{0:02X} ) = 0x{1:02X} => wr={2:d}, rd={3:d}, length={4:d}'.format( index, typeof, typeof.wr(), typeof.rd(), typeof.length() ) )

  k = random.randbytes( 16 )
  m = random.randbytes( 16 )

  ( ack,   ) = client.wr( GPR_K, bytes( k ) )
  ( ack,   ) = client.wr( GPR_M, bytes( m ) )

  ( ack,   ) = client.kernel()

  ( ack, c ) = client.rd( GPR_C             )

  for i in range( 16 ) :
    print( 'i = {0:2d} : c[i] = 0x{1:02X}, k[i] ^ m[i] = 0x{2:02X}'.format( i, c[ i ], k[ i ] ^ m[ i ] ) )

  client.close()

# =============================================================================
