set blockname [file rootname [file tail [info script] ]]

source scripts/common_no_mod.tcl

directive set -DESIGN_HIERARCHY " 
    {RendererWrapper} 
"

go compile

source scripts/set_libraries.tcl

solution library add "\[Block\] ShaderCores.v1"
solution library add "\[Block\] RayGeneration.v1"

go libraries
directive set -CLOCKS $clocks

directive set /RendererWrapper/ShaderCores -MAP_TO_MODULE "\[Block\] ShaderCores.v1"
directive set /RendererWrapper/RayGeneration -MAP_TO_MODULE "\[Block\] RayGeneration.v1"

directive set /RendererWrapper -FIFO_DEPTH 3

go assembly
directive set -EFFORT_LEVEL high

go architect

go allocate
go extract