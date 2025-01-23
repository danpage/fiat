# Copyright (C) 2024 Daniel Page <dan@phoo.org>
#
# Use of this source code is restricted per the MIT license, a copy of which 
# can be found via https://opensource.org/license/mit (and which is included 
# as LICENSE.txt within the associated archive or repository).

import argparse, chipwhisperer as cw

# =============================================================================

PLL0 = 0
PLL1 = 1
PLL2 = 2

FPGA_FREQ = 100000000
FPGA_VCC  = 1.0

if( __name__ == '__main__' ) :
  parser = argparse.ArgumentParser( add_help = False )

  parser.add_argument( '--bitstream', type = str,            default = None      )

  parser.add_argument( '--fpga-freq', type = int,            default = FPGA_FREQ )
  parser.add_argument( '--fpga-vcc',  type = float,          default = FPGA_VCC  )

  parser.add_argument( '--slurp',     action = 'store_true', default = False     )
  parser.add_argument( '--verbose',   action = 'store_true', default = False     )

  argv = parser.parse_args()

  # connect and program

  if ( argv.bitstream ) :
    target = cw.target( None, cw.targets.CW305, slurp = argv.slurp, bsfile = argv.bitstream, force = True )
  else :
    target = cw.target( None, cw.targets.CW305, slurp = argv.slurp                                        )

  # configure VCC

  target.vccint_set( argv.fpga_vcc )

  # configure PLL

  target.pll.pll_enable_set( True )

  target.pll.pll_outenable_set( False, PLL0 ) # disable PLL0
  target.pll.pll_outenable_set(  True, PLL1 ) #  enable PLL1 for the FPGA
  target.pll.pll_outenable_set( False, PLL2 ) # disable PLL2

  target.pll.pll_outfreq_set( argv.fpga_freq, PLL1 )

  # configure USB clock

  target.clkusbautooff = True
  target.clksleeptime  = 1

  # dump debugging information

  if ( argv.verbose ) :
    print( 'PLL0: {0:d}, {1:f}, {2:s}'.format( target.pll.pll_outenable_get( PLL0 ), target.pll.pll_outfreq_get( PLL0 ), target.pll.pll_outslew_get( PLL0 ) ) )
    print( 'PLL1: {0:d}, {1:f}, {2:s}'.format( target.pll.pll_outenable_get( PLL1 ), target.pll.pll_outfreq_get( PLL1 ), target.pll.pll_outslew_get( PLL1 ) ) )
    print( 'PLL2: {0:d}, {1:f}, {2:s}'.format( target.pll.pll_outenable_get( PLL2 ), target.pll.pll_outfreq_get( PLL2 ), target.pll.pll_outslew_get( PLL2 ) ) )

  # disconnect

  target.dis()

# =============================================================================
