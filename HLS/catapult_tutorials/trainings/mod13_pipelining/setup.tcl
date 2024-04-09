puts "Catapult Online Training: Module 13"
puts "In this lab, you will pipeline the design"
puts "  and examine various stalling modes"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
solution new -state initial
solution options defaults

# Setup options and flows
flow package require /SCVerify
flow package option set /SCVerify/USE_CCS_BLOCK true

# Read design source files
options set Input/CppStandard c++11
solution file add ${sfd}/src/StallModes_tb.cpp -exclude true
solution file add ${sfd}/src/StallModes.cpp
go compile

directive set -CLOCKS {clk {-CLOCK_PERIOD 1}}
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
go assembly
