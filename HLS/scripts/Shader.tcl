set blockname [file rootname [file tail [info script] ]]

source scripts/common_no_mod.tcl

set type1 "sfp_11_22"
set type2 "fp_1_22"

directive set -DESIGN_HIERARCHY " 
    {Shader<${type1}, ${type2}>} 
"

go compile

source scripts/set_libraries.tcl

solution library add "\[Block\] MaterialScatter<${type1},${type2}>.v1"
solution library add "\[Block\] WorldHit<${type1},${type2}>.v1"

go libraries
directive set -CLOCKS $clocks

go assembly
directive set -EFFORT_LEVEL high

go architect

go allocate
go extract