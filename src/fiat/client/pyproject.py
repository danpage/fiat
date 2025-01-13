# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

import hatchling.metadata.plugin.interface, os

class CustomMetaDataHook( hatchling.metadata.plugin.interface.MetadataHookInterface ) :
  def update( self, metadata ) :
    metadata[ 'version' ] = os.environ[ 'REPO_VERSION' ]
    metadata[ 'license' ] = { 'file' : os.path.relpath( os.path.join( os.environ[ 'REPO_HOME' ], 'LICENSE.txt' ) ), 'content-type' : 'text/plain' }
    metadata[ 'readme'  ] = { 'file' : os.path.relpath( os.path.join( os.environ[ 'REPO_HOME' ], 'doc', 'README.md' ) ), 'content-type' : 'text/markdown' }
