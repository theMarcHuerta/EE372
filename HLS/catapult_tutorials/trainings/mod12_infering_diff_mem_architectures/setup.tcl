puts "Setup Catapult Online Training: Module 12"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
solution new -state initial
solution options defaults

# Setup options and flows
flow package require /SCVerify
flow package option set /SCVerify/DISABLE_EMPTY_INPUTS true
flow package option set /SCVerify/ENABLE_STALL_TOGGLE true

# Specify the design files
solution file add ${sfd}/src/mem_intf_tb.cpp -type C++ -exclude true
solution file add ${sfd}/src/mem_intf.cpp -type C++
go compile

# Specify Technology Library
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
solution library add ccs_sample_mem
go libraries

# Specify the Clock Period
directive set -CLOCKS {clk {-CLOCK_PERIOD 2.0}}
go assembly
