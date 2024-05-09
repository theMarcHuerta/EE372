set blockname [file rootname [file tail [info script] ]]

source scripts/common_no_mod.tcl

directive set -DESIGN_HIERARCHY " 
    {Pathtracer} 
"

go compile

source scripts/set_libraries.tcl

solution library add "\[Block\] QuadBuffer<80>.v1"
solution library add "\[Block\] RendererWrapper.v1"
solution library add "\[Block\] PixelAccumulator.v1"

go libraries
directive set -CLOCKS $clocks

directive set /Pathtracer/QuadBuffer<80> -MAP_TO_MODULE "\[Block\] QuadBuffer<80>.v1"
directive set /Pathtracer/RendererWrapper -MAP_TO_MODULE "\[Block\] RendererWrapper.v1"
directive set /Pathtracer/PixelAccumulator -MAP_TO_MODULE "\[Block\] PixelAccumulator.v1"

directive set /Pathtracer -FIFO_DEPTH 3

go assembly
# directive set -EFFORT_LEVEL high

go architect

go allocate
go extract