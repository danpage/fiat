# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

import hatchling.metadata.plugin.interface, os

class CustomMetaDataHook( hatchling.metadata.plugin.interface.MetadataHookInterface ) :
  def update( self, metadata ) :
    metadata[ 'version' ] = os.environ[ 'FIAT_VERSION_MAJOR' ] + '.' + os.environ[ 'FIAT_VERSION_MINOR' ] + '.' + os.environ[ 'FIAT_VERSION_PATCH' ]
    metadata[ 'license' ] = { 'file' : os.path.relpath( os.path.join( os.environ[ 'FIAT_PATH_REPO' ], 'LICENSE.txt' ) ), 'content-type' : 'text/plain' }
    metadata[ 'readme'  ] = { 'file' : os.path.relpath( os.path.join( os.environ[ 'FIAT_PATH_REPO' ], 'doc', 'README.md' ) ), 'content-type' : 'text/markdown' }
