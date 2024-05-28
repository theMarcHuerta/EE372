#=========================================================================
# main.tcl
#=========================================================================
# Run the foundation flow step
#
# Author : Christopher Torng
# Date   : January 13, 2020

# set_spacing_rule -name "FixVia4Spacing" -obj [get_cells -filter "cellType == VIA && name == 'Via4'"] -minSpacing 0.42
# setMetalFillSpacingTable -layer {5} -fill_to_active {{1.7}}
# -place_detail_check_cut_spacing {true | false}
# ecoRoute -modifyOnlyLayers 4:5 
# ecoRoute -fix_drc
# addFiller -cell filler_cell_list -prefix FILLER
# setMetalFillSpacingTable -layer {5} -fill_to_fill {{1.7 2.1}}
# trimMetalFill -layer 5
source -verbose innovus-foundation-flow/INNOVUS/run_postroute.tcl


