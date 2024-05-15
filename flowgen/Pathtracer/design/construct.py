#! /usr/bin/env python
#=========================================================================
# construct.py
#=========================================================================
# Flow specification for HLS design
#
# Author : Priyanka Raina & Marc Huerta
#

import os
import sys

from mflowgen.components import Graph, Step

def construct():

  g = Graph()
  g.sys_path.append( '/farmshare/home/classes/ee/272' )

  #-----------------------------------------------------------------------
  # Parameters
  #-----------------------------------------------------------------------
  
  adk_name = 'skywater-130nm-adk.v2021'
  adk_view = 'view-standard'

  parameters = {
    'construct_path' : __file__,
    'design_name'    : 'Pathtracer',
    'clock_period'   : 9.0,
    'adk'            : adk_name,
    'adk_view'       : adk_view,
    'topographical'  : True,
    # 'testbench_name' : 'Conv_tb',
    'strip_path'     : 'scverify_top/Pathtracer',
    'saif_instance'  : 'scverify_top/Pathtracer'
  }

  #-----------------------------------------------------------------------
  # Create nodes
  #-----------------------------------------------------------------------

  this_dir = os.path.dirname( os.path.abspath( __file__ ) )

  # ADK step

  g.set_adk( adk_name )
  adk = g.get_adk_step()

  # Custom steps

  # sram          = Step( this_dir + '/sram'          )
  # rtl           = Step( this_dir + '/rtl'           )
  hls           = Step( this_dir + '/hls'           )
  # testbench     = Step( this_dir + '/testbench'     )
  constraints   = Step( this_dir + '/constraints'   )

  # Default steps

  info         = Step( 'info',                          default=True )
  dc           = Step( 'synopsys-dc-synthesis',         default=True )
  # rtl_sim      = Step( 'synopsys-vcs-sim',              default=True )
  gen_saif     = Step( 'synopsys-vcd2saif-convert',     default=True )
  gen_saif_rtl = gen_saif.clone()
  gen_saif_rtl.set_name( 'gen-saif-rtl' )

  #-----------------------------------------------------------------------
  # Graph -- Add nodes
  #-----------------------------------------------------------------------

  g.add_step( info         )
  # g.add_step( sram         )
  # g.add_step( rtl          )
  g.add_step( hls          )
  # g.add_step( testbench    )
  g.add_step( constraints  )
  g.add_step( dc           )
  # g.add_step( rtl_sim      )
  g.add_step( gen_saif_rtl )

  #-----------------------------------------------------------------------
  # Graph -- Add edges
  #-----------------------------------------------------------------------
  
  # Dynamically add edges

  # dc.extend_inputs(['sram1_tt_1p8V_25C.db'])
  # dc.extend_inputs(['sram2_tt_1p8V_25C.db'])
  # rtl_sim.extend_inputs(['sram.v'])

  # Connect by name

  g.connect_by_name( adk,          dc           )
  # g.connect_by_name( sram,         dc           )
  g.connect_by_name( hls,          dc           )
  g.connect_by_name( constraints,  dc           )
  # g.connect_by_name( hls,          rtl_sim      ) 
  # g.connect_by_name( testbench,    rtl_sim      ) 
  # g.connect_by_name( sram,         rtl_sim      ) 
  # g.connect( rtl_sim.o( 'run.vcd' ), gen_saif_rtl.i( 'run.vcd' ) ) # FIXME: VCS sim node generates a VCD file but gives it a VPD extension
  g.connect( hls.o( 'run.vcd' ), gen_saif_rtl.i( 'run.vcd' ) )
  g.connect_by_name( gen_saif_rtl, dc           ) # run.saifs

  #-----------------------------------------------------------------------
  # Parameterize
  #-----------------------------------------------------------------------

  g.update_params( parameters )
  g.param_space( 'synopsys-dc-synthesis', 'clock_period', [20, 10, 9, 6 ] )

  return g

if __name__ == '__main__':
  g = construct()
  g.plot()