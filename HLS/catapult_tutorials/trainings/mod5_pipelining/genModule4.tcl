puts "Catapult On-Demand Training: Regenerate Module 4 Results"

# Establish the location of this script and use it to reference all
# other files in this example
set sfd [file dirname [info script]]

# Reset the options to the factory defaults
solution new -state initial
solution options defaults
flow package require /SCVerify

# Compile the Design
solution file add ${sfd}/src/fir_filter.cpp
solution file add ${sfd}/src/csvparser.cpp -exclude true
solution file add ${sfd}/src/fir_filter_tb.cpp -exclude true
go analyze

directive set -DESIGN_HIERARCHY fir_filter
go compile

# Specify Technology Library
solution library add nangate-45nm_beh -- -rtlsyntool OasysRTL -vendor Nangate -technology 045nm
go libraries

# Synthesize the Design
directive set -CLOCKS {clk {-CLOCK_PERIOD 10}}
go extract
