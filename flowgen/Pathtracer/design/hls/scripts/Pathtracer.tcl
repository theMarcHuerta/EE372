set blockname [file rootname [file tail [info script] ]]

source scripts/common_no_mod.tcl

directive set -DESIGN_HIERARCHY " 
    {Pathtracer} 
"

go compile

source scripts/set_libraries.tcl

# solution library add "\[Block\] QuadBuffer<80>.v1"
# solution library add "\[Block\] RendererWrapper.v1"
# solution library add "\[Block\] PixelAccumulator.v1"

go libraries
directive set -CLOCKS $clocks

# directive set /Pathtracer/QuadBuffer<80> -MAP_TO_MODULE "\[Block\] QuadBuffer<80>.v1"
# directive set /Pathtracer/RendererWrapper -MAP_TO_MODULE "\[Block\] RendererWrapper.v1"
# directive set /Pathtracer/PixelAccumulator -MAP_TO_MODULE "\[Block\] PixelAccumulator.v1"

# directives for using register memory
directive set /Pathtracer/QuadBuffer<64>/run/buffer_bank0.data:rsc -MAP_TO_MODULE {[Register]}
directive set /Pathtracer/QuadBuffer<64>/run/buffer_bank1.data:rsc -MAP_TO_MODULE {[Register]}
directive set /Pathtracer/QuadBuffer<64>/run/buffer_bank2.data:rsc -MAP_TO_MODULE {[Register]}
directive set /Pathtracer/QuadBuffer<64>/run/buffer_bank3.data:rsc -MAP_TO_MODULE {[Register]}
directive set /Pathtracer/QuadBuffer<64>/run/buffer_bank4.data:rsc -MAP_TO_MODULE {[Register]}
directive set /Pathtracer/QuadBuffer<64>/run/buffer_bank5.data:rsc -MAP_TO_MODULE {[Register]}
directive set /Pathtracer/QuadBuffer<64>/run/buffer_bank6.data:rsc -MAP_TO_MODULE {[Register]}
directive set /Pathtracer/QuadBuffer<64>/run/buffer_bank7.data:rsc -MAP_TO_MODULE {[Register]}
directive set /Pathtracer/QuadBuffer<64>/run/buffer_bank8.data:rsc -MAP_TO_MODULE {[Register]}
directive set /Pathtracer/QuadBuffer<64>/run/buffer_bank9.data:rsc -MAP_TO_MODULE {[Register]}
directive set /Pathtracer/QuadBuffer<64>/run/buffer_bank10.data:rsc -MAP_TO_MODULE {[Register]}
directive set /Pathtracer/QuadBuffer<64>/run/buffer_bank11.data:rsc -MAP_TO_MODULE {[Register]}

# directive set /Pathtracer -FIFO_DEPTH 3

directive set /Pathtracer/WorldHit/hit/div<34,14,AC_TRN,AC_WRAP,34,11,AC_TRN,AC_WRAP,60,17,AC_TRN,AC_WRAP>#1:for -UNROLL yes
directive set /Pathtracer/WorldHit/hit/div<34,14,AC_TRN,AC_WRAP,34,11,AC_TRN,AC_WRAP,60,17,AC_TRN,AC_WRAP>#3:for -UNROLL yes

go assembly
directive set -EFFORT_LEVEL high

go architect

go allocate
go extract