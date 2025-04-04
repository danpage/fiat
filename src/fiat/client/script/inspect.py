# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

import argparse, binascii, json, libfiat, logging, random, sys

# =============================================================================

def  inspect( client ) :
  index  = 0x00

  gpr_wr = dict()
  gpr_rd = dict()

  while ( True ) :
    t = client.nameof( index )

    if ( t[ 0 ] == libfiat.util.Ack.FAILURE ) :
      logging.info( 'index 0x{0:02X} => failure'.format( index ) ) ; break
    else :
      nameof = t[ 1 ]

    t = client.sizeof( index )

    if ( t[ 0 ] == libfiat.util.Ack.FAILURE ) :
      logging.info( 'index 0x{0:02X} => failure'.format( index ) ) ; break
    else :
      sizeof = t[ 1 ]

    t = client.typeof( index )

    if ( t[ 0 ] == libfiat.util.Ack.FAILURE ) :
      logging.info( 'index 0x{0:02X} => failure'.format( index ) ) ; break
    else :
      typeof = t[ 1 ]

    logging.info( 'index 0x{0:02X} => success'.format( index ) )
    logging.info( '- nameof( 0x{0:02X} ) = {1:s}'                                        .format( index, nameof                                            ) )  
    logging.info( '- sizeof( 0x{0:02X} ) = {1:d}'                                        .format( index, sizeof                                            ) )
    logging.info( '- typeof( 0x{0:02X} ) = 0x{1:02X} => wr={2:d}, rd={3:d}, length={4:d}'.format( index, typeof, typeof.wr(), typeof.rd(), typeof.length() ) )

    if ( typeof.wr() ) :
      gpr_wr[ index ] = { 'nameof' : nameof, 'sizeof' : sizeof, 'typeof' : typeof }
    if ( typeof.rd() ) :
      gpr_rd[ index ] = { 'nameof' : nameof, 'sizeof' : sizeof, 'typeof' : typeof }

    index += 1

  return ( gpr_wr, gpr_rd )

def generate( client, gpr_wr, gpr_rd ) :
  vectors = list()

  for i in range( argv.count ) :
    t_wr = dict( gpr_wr )
    t_rd = dict( gpr_rd )

    for ( k, v ) in t_wr.items() :
      if   ( v[ 'typeof' ].length() == 0 ) : #    fixed length
        data = bytes( random.randbytes(                    v[ 'sizeof' ]   ) )
      elif ( v[ 'typeof' ].length() == 1 ) : # variable length
        data = bytes( random.randbytes( random.randint( 1, v[ 'sizeof' ] ) ) )

      ( ack,      ) = client.wr( k, data )
      v[ 'data' ] = binascii.b2a_hex( data ).decode( 'ascii' )

    ( ack, ) = client.kernel_prologue()
    ( ack, ) = client.kernel()
    ( ack, ) = client.kernel_epilogue()

    for ( k, v ) in t_rd.items() :
      ( ack, data ) = client.rd( k       )
      v[ 'data' ] = binascii.b2a_hex( data ).decode( 'ascii' )

    vectors.append( ( t_wr, t_rd ) )

  with ( open( argv.file, 'w' ) if ( argv.file != None ) else sys.stdout ) as fd :
    json.dump( vectors, fd )

def validate( client, gpr_wr, gpr_rd ) :
  with ( open( argv.file, 'r' ) if ( argv.file != None ) else sys.stdin  ) as fd :
    vectors = json.load( fd )

  for ( i, ( t_wr, t_rd ) ) in enumerate( vectors ) :
    for ( k, v ) in t_wr.items() :
      data = bytes( binascii.a2b_hex( v[ 'data' ] ) )
      ( ack,      ) = client.wr( int( k ), data )

    ( ack, ) = client.kernel_prologue()
    ( ack, ) = client.kernel()
    ( ack, ) = client.kernel_epilogue()

    for ( k, v ) in t_rd.items() :
      data = bytes( binascii.a2b_hex( v[ 'data' ] ) )
      ( ack, t    ) = client.rd( int( k )       )

      if ( data == t ) :
        logging.info( 'vector {0:d} passed'.format( i ) )
      else :
        logging.info( 'vector {0:d} failed'.format( i ) )

# -----------------------------------------------------------------------------

if ( __name__ == '__main__' ) :
  parser = argparse.ArgumentParser( add_help = False )

  parser.add_argument( '--mode',  action = 'store', choices = [ 'inspect', 'generate', 'validate' ], default = 'inspect' )

  parser.add_argument( '--seed',  action = 'store',                                                  default =         0 )
  parser.add_argument( '--file',  action = 'store',                                                  default =      None )
  parser.add_argument( '--count', action = 'store',                                                  default =        10 )

  ( argv, _ ) = parser.parse_known_args( sys.argv )

  client = libfiat.open( sys.argv ) ; random.seed( int( argv.seed ) ) ; logging.basicConfig( level = logging.INFO )

  if   ( argv.mode ==  'inspect' ) :
    ( gpr_wr, gpr_rd ) = inspect( client )

  elif ( argv.mode == 'generate' ) :
    ( gpr_wr, gpr_rd ) = inspect( client ) ; generate( client, gpr_wr, gpr_rd )

  elif ( argv.mode == 'validate' ) :
    ( gpr_wr, gpr_rd ) = inspect( client ) ; validate( client, gpr_wr, gpr_rd )

# =============================================================================
