puts "Catapult OnDemand Training: Setup Module 14"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
solution new -state initial
solution options defaults
flow package require /SCVerify
flow package option set /SCVerify/USE_CCS_BLOCK true

# Specify the design files
solution file add ${sfd}/src/vector_mult_tb.cpp -type C++ -exclude true
solution file add ${sfd}/src/vector_mult.cpp -type C++
go analyze 

# Specify Base Technology and RAM Library
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
solution library add ccs_sample_mem
solution library add ram_nangate-45nm-singleport_beh
go libraries

# Specify the Clock Period
directive set -CLOCKS {clk {-CLOCK_PERIOD 1}}
go assembly 

#Select Memory Resource
directive set /vector_mult/core/op1_mem:rsc -MAP_TO_MODULE ram_nangate-45nm-singleport_beh.RAM
directive set /vector_mult/core/op2_mem:rsc -MAP_TO_MODULE ram_nangate-45nm-singleport_beh.RAM
directive set /vector_mult/core/result_mem:rsc -MAP_TO_MODULE ram_nangate-45nm-singleport_beh.RAM

