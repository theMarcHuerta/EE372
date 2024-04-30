set blockname [file rootname [file tail [info script] ]]

source scripts/common_no_mod.tcl

directive set -DESIGN_HIERARCHY " 
    {Shader} 
"

go compile

source scripts/set_libraries.tcl

solution library add "\[Block\] MaterialScatter.v1"
solution library add "\[Block\] WorldHit.v1"

go libraries
directive set -CLOCKS $clocks

go assembly
directive set -EFFORT_LEVEL high

go architect

go allocate
go extract