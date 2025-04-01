# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

import argparse, libfiat.client

# =============================================================================

all = [ 'client', 'driver', 'util' ]

def open( argv ) :
  parser = argparse.ArgumentParser( add_help = False )

  parser.add_argument( '--libfiat-device', dest = 'device', action = 'store', choices = [ 'socket', 'serial' ], default = 'socket' )
  parser.add_argument( '--libfiat-driver', dest = 'driver', action = 'store', choices = [ 'binary', 'text'   ], default = 'binary' )

  parser.add_argument( '--libfiat-host',   dest =   'host', action = 'store', default = None  )
  parser.add_argument( '--libfiat-port',   dest =   'port', action = 'store', default = None  )

  parser.add_argument( '--libfiat-baud',   dest =   'baud', action = 'store', default = 38400 )

  ( argv, _ ) = parser.parse_known_args( argv )

  if   ( argv.device == 'serial' ) :
    client = libfiat.client.ClientImpSerial( driver = argv.driver ) ; client.open(               ( argv.port ), baudrate = argv.baud ) ; return client
  elif ( argv.device == 'socket' ) :
    client = libfiat.client.ClientImpSocket( driver = argv.driver ) ; client.open( argv.host, int( argv.port ),                      ) ; return client

  return None

# =============================================================================
