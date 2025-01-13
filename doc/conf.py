# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

import os, sys

# =============================================================================

sys.path.insert( 0, os.path.join( '..', 'src', 'fiat', 'client' ) )

project = 'FIAT'
copyright = '2024, Daniel Page'
author = 'Daniel Page'

extensions = [ 'sphinx.ext.autodoc', 'sphinx.ext.autosummary', 'hawkmoth', 'hawkmoth.ext.javadoc', 'myst_parser' ]

html_theme = 'sphinx_rtd_theme'
html_static_path = []

hawkmoth_root = '../src/fiat'
hawkmoth_transform_default = 'javadoc'
hawkmoth_clang = [ '-I../src/fiat/target/share', '-I../src/fiat/target/kernel', '-I../src/fiat/target/kernel/imp' ]

# =============================================================================
