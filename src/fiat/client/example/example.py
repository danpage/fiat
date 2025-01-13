# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

import argparse, libfiat.client, random, sys

GPR_C = 0x00
GPR_K = 0x01
GPR_M = 0x02

if ( __name__ == '__main__' ) :
  parser = argparse.ArgumentParser( add_help = False )

  parser.add_argument( '--device', action = 'store', choices = [ 'socket', 'serial' ], default = 'socket' )
  parser.add_argument( '--driver', action = 'store', choices = [ 'binary', 'text'   ], default = 'binary' )

  parser.add_argument( '--seed',   action = 'store', default = None  )

  parser.add_argument( '--host',   action = 'store', default = None  )
  parser.add_argument( '--port',   action = 'store', default = None  )

  parser.add_argument( '--baud',   action = 'store', default = 38400 )

  argv = parser.parse_args() ; random.seed( int( argv.seed ) )

  if   ( argv.device == 'serial' ) :
    client = libfiat.client.ClientSerial( driver = argv.driver ) ; client.open(               ( argv.port ), baudrate = argv.baud )
  elif ( argv.device == 'socket' ) :
    client = libfiat.client.ClientSocket( driver = argv.driver ) ; client.open( argv.host, int( argv.port ),                      )

  ( ack,   ) = client.reset()

  ( ack, n ) = client.sizeof( GPR_C )
  print( 'sizeof( GPR_C ) = {0:d}'.format( n ) )
  ( ack, n ) = client.sizeof( GPR_K )
  print( 'sizeof( GPR_K ) = {0:d}'.format( n ) )
  ( ack, n ) = client.sizeof( GPR_M )
  print( 'sizeof( GPR_M ) = {0:d}'.format( n ) )

  ( ack, t ) = client.typeof( GPR_C )
  print( 'typeof( GPR_C ) = {0:02X} => wr={1:d}, rd={2:d}, length={3:d}'.format( t, t.wr(), t.rd(), t.length() ) )
  ( ack, t ) = client.typeof( GPR_K )
  print( 'typeof( GPR_K ) = {0:02X} => wr={1:d}, rd={2:d}, length={3:d}'.format( t, t.wr(), t.rd(), t.length() ) )
  ( ack, t ) = client.typeof( GPR_M )
  print( 'typeof( GPR_M ) = {0:02X} => wr={1:d}, rd={2:d}, length={3:d}'.format( t, t.wr(), t.rd(), t.length() ) )

  k = random.randbytes( 16 )
  m = random.randbytes( 16 )

  ( ack,   ) = client.wr( GPR_K, bytes( k ) )
  ( ack,   ) = client.wr( GPR_M, bytes( m ) )

  ( ack,   ) = client.kernel()

  ( ack, c ) = client.rd( GPR_C             )

  for i in range( 16 ) :
    print( 'i = {0:2d} : c[i] = 0x{1:02X}, k[i] ^ m[i] = 0x{2:02X}'.format( i, c[ i ], k[ i ] ^ m[ i ] ) )

  client.close()

