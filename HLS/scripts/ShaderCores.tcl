set blockname [file rootname [file tail [info script] ]]

source scripts/common_no_mod.tcl

directive set -DESIGN_HIERARCHY " 
    {ShaderCores} 
"

go compile

source scripts/set_libraries.tcl

# solution library add "\[Block\] Shader.v1"
# solution library add "\[Block\] ShaderFeedbackController.v1"

go libraries
directive set -CLOCKS $clocks

# directive set /ShaderCores/Shader -MAP_TO_MODULE "\[Block\] Shader.v1"
# directive set /ShaderCores/ShaderFeedbackController -MAP_TO_MODULE "\[Block\] ShaderFeedbackController.v1"

# directive set /ShaderCores -FIFO_DEPTH 3

go assembly
# directive set -EFFORT_LEVEL high

go architect

go allocate
go extract